// =============================================================================
// 1. 背景
//    我们想实现的是一棵支持"点查询、范围查询"的有序索引结构。
//    B 树(B-Tree)把所有数据放在每个结点里；而 B+ 树(B+ Tree)做了更彻底的
//    分工，这也是绝大多数数据库 / 文件系统索引真正采用的结构：
//       · 只有最底层的【叶子结点】保存真正的数据 (key, value)；
//       · 叶子之间用 prev / next 串成一个有序的双向链表，方便范围扫描；
//       · 上层的【内部结点】只保存"分隔键"(也叫路由键)，负责告诉查找过程
//         应该走进哪棵子树，本身不携带业务数据；
//       · 所有叶子都位于同一层(整棵树高度一致)。
//
// 2. 本实现遵循的不变量（请牢记，后面每个函数都在维护这些规则）
//    (1) 树的"阶" M = kOrder = 4：每个内部结点最多有 M 个孩子；
//    (2) 每个结点最多保存 M-1 = 3 个键；
//    (3) 除根结点外，每个结点至少保存 ⌈M/2⌉ - 1 = 1 个键
//        （对内部结点等价于至少有 2 个孩子）；
//    (4) 内部结点满足：children.size() == keys.size() + 1，
//        且 keys[i] 是"分隔键"：孩子 i 中所有键 < keys[i] ≤ 孩子 i+1 中所有键；
//    (5) 键值在全树唯一（重复插入等价于更新 value），叶子链表全局有序。
//
// 3. 两个容易混淆的"上升/拷贝"细节
//    · 叶子分裂：被提升到父结点的键是"右半叶子的最小值"，它会继续留在叶子
//      里（键发生了向上的"拷贝"）。
//    · 内部结点分裂：被提升的键是原结点中间位置的分隔键，它只属于父结点，
//      不再留在原来的两个半结点中。
//    删除某个叶子最小键之后，内部结点里指向它的旧分隔键会变成一个"陈旧"
//    分隔键——但它仍然小于右子树里剩余的每个键，因此查找路由依然正确，
//    这是一种被广泛使用的简化（真实数据库也允许内部键不是真实数据键）。
//
// =============================================================================

#include <algorithm>  // std::lower_bound / std::upper_bound / std::sort
#include <climits>    // INT_MIN，用于自检函数里的哨兵值
#include <iostream>
#include <map>      // 仅供 main() 的对照测试使用
#include <random>   // 仅供 main() 的随机测试使用
#include <vector>

namespace b_plus_tree {

using namespace std;

// -----------------------------------------------------------------------------
// 常量定义：上面第 2 节里的几个数字集中管理，避免魔法数字
// -----------------------------------------------------------------------------
// 树的阶 M：内部结点最多有 M 个孩子（这里 M=4，便于手工画出分裂过程）
constexpr int kOrder = 4;
// 每个结点最多保存的键数 = M - 1
constexpr int kMaxKeys = kOrder - 1;
// 除根外每个结点最少保存的键数 = ⌈M/2⌉ - 1
constexpr int kMinKeys = (kOrder + 1) / 2 - 1;
// 内部结点最少孩子数 = 最少键数 + 1
constexpr int kMinChildren = kMinKeys + 1;

// -----------------------------------------------------------------------------
// 数据结构定义
// -----------------------------------------------------------------------------

// 一条真正的数据记录：叶子结点里"键值对"的载体
struct LeafData {
  int key;    // 排序用的键
  int value;  // 键关联的值（可以是任意业务数据，这里用 int 演示）
  LeafData(int k, int v) : key(k), value(v) {}
};

// 结点基类：叶子结点与内部结点的公共部分。
// 注意 keys 的双重语义：
//   · 在 LeafNode 中：keys 是真实数据键，与 values 一一对应；
//   · 在 InternalNode 中：keys 只是分隔键，用于查找时选择子树。
struct BPlusNode {
  bool is_leaf;        // 标记实际类型（LeafNode 还是 InternalNode）
  vector<int> keys;    // 详见上面的双重语义说明
  BPlusNode* parent;   // 指向父结点；根结点的 parent == nullptr
  BPlusNode(bool leaf) : is_leaf(leaf), parent(nullptr) {}
  virtual ~BPlusNode() = default;
};

// 内部结点：只负责"路由"。孩子数恒等于 keys.size() + 1。
// 设 keys = {10, 20}，则有 3 个孩子：
//   孩子0 装所有 <10 的键，孩子1 装 [10,20)，孩子2 装 ≥20 的键。
struct InternalNode : public BPlusNode {
  vector<BPlusNode*> children;
  InternalNode() : BPlusNode(false) {}
};

// 叶子结点：真正存放 (key, value)，并通过 prev/next 串成有序双向链表，
// 这正是 B+ 树能高效做范围查询(RangeQuery)的原因。
struct LeafNode : public BPlusNode {
  vector<LeafData> values;  // values[i] 与 keys[i] 下标一一对应
  LeafNode* prev;           // 左邻叶子
  LeafNode* next;           // 右邻叶子
  LeafNode() : BPlusNode(true), prev(nullptr), next(nullptr) {}
};

// -----------------------------------------------------------------------------
// BPlusTree 类
// -----------------------------------------------------------------------------
class BPlusTree {
 public:
  // 构造：初始的树只有一片空叶子作为根（不建内部结点，懒加载）
  BPlusTree() { root = new LeafNode(); }
  // 析构：递归释放以 root 为根的整棵树，防止内存泄漏
  ~BPlusTree() { DestroySubtree(root); }
  BPlusTree(const BPlusTree&) = delete;
  BPlusTree& operator=(const BPlusTree&) = delete;

 private:
  BPlusNode* root;  // 根结点指针：初始是一个空叶子，也可能长成内部结点

  void DestroySubtree(BPlusNode* node) {
    if (node == nullptr) return;
    if (!node->is_leaf) {
      InternalNode* internal = static_cast<InternalNode*>(node);
      for (BPlusNode* child : internal->children) {
        DestroySubtree(child);
      }
    }
    delete node;
  }

  // 返回某个内部结点中 child 所在的下标（children[pos] == child）。
  // M 很小（=4），顺序查找足够，且比 std::find 更直观，适合教学。
  int IndexOfChild(InternalNode* parent, BPlusNode* child) {
    for (size_t i = 0; i < parent->children.size(); ++i) {
      if (parent->children[i] == child) return static_cast<int>(i);
    }
    return -1;  // 正常流程不会走到这里
  }

  // 从根出发，根据 key 找到"它应该落在哪一片叶子"。
  // 查找规则：在内部结点中找第一个 > key 的分隔键 upper_bound，
  // 对应下标即应进入的孩子；若 key ≥ 所有分隔键则进最后一个孩子。
  // 用 upper_bound 而不是 lower_bound 的原因：
  //   分隔键 keys[i] 往往恰好等于右孩子子树的最小键，等于该键的 key
  //   必须被导向右孩子，而 upper_bound 恰好会跳过"相等"的分隔键。
  LeafNode* FindLeaf(int key) {
    BPlusNode* cur = root;
    while (!cur->is_leaf) {  // 一路向下，直到落在叶子上
      InternalNode* internal = static_cast<InternalNode*>(cur);
      // upper_bound：返回第一个大于 key 的位置的迭代器
      auto it = upper_bound(internal->keys.begin(), internal->keys.end(), key);
      int index = static_cast<int>(it - internal->keys.begin());
      cur = internal->children[index];  // 走向应进入的孩子
    }
    return static_cast<LeafNode*>(cur);
  }


  // ---------- 插入 / 分裂 ----------

  // 把一个 (key, value) 有序地插入叶子（keys 与 values 两个并行数组同步操作）。
  // 若 key 已存在则只更新 value，维持"全树键唯一"这一不变量。
  void InsertIntoLeaf(LeafNode* leaf, int key, int value) {
    // lower_bound 找到"第一个不小于 key"的位置，即 key 应插入的下标
    int pos = static_cast<int>(
        lower_bound(leaf->keys.begin(), leaf->keys.end(), key) -
        leaf->keys.begin());
    if (pos < static_cast<int>(leaf->keys.size()) &&
        leaf->keys[pos] == key) {  // 键已存在 → 直接改值
      leaf->values[pos].value = value;
      return;
    }
    // keys / values 在同一个位置插入，保证一一对应
    leaf->keys.insert(leaf->keys.begin() + pos, key);
    leaf->values.insert(leaf->values.begin() + pos, LeafData(key, value));
  }

  // 分裂叶子结点。调用前提：叶子已经装满了 kOrder 个键（发生溢出）。
  // 做法：把键一分为二，原结点保留左半成为"左叶"，新建右叶装右半；
  // 同时把新叶插入到叶子链表里（这也是 B+ 树区别于 B 树的重要一步）。
  // 返回值是新右叶，而"要拷贝到父结点的键"通过出参 promote_key 返回。
  LeafNode* SplitLeaf(LeafNode* leaf, int& promote_key) {
    int mid = static_cast<int>(leaf->keys.size()) / 2;  // 本例恒为 2

    // 1. 右半（keys[mid..] 与 values[mid..]）搬进新建的右叶
    LeafNode* right = new LeafNode();
    right->keys.assign(leaf->keys.begin() + mid, leaf->keys.end());
    for (size_t i = mid; i < leaf->values.size(); ++i) {
      right->values.push_back(leaf->values[i]);
    }
    leaf->keys.resize(mid);      // 原叶子只保留左半
    // 注意：vector<LeafData> 没有默认构造函数，不能用 resize 缩容，
    // 用 erase 把右半从原叶子中删掉。
    leaf->values.erase(leaf->values.begin() + mid, leaf->values.end());

    // 2. 维护叶子双向链表：leaf <-> right <-> (原 next)
    right->next = leaf->next;
    if (right->next != nullptr) right->next->prev = right;
    right->prev = leaf;
    leaf->next = right;

    // 3. 关键！被提升的键 = 右叶的最小键，且该键"同时留在右叶里"，
    //    这就是 B+ 树把叶子键向上拷贝到内部结点的过程（拷贝上升）。
    promote_key = right->keys.front();
    return right;
  }

  // 分裂内部结点。调用前提：内部结点已有 kOrder 个键（即 kOrder+1 个孩子）。
  // 与叶子分裂不同：中间那个分隔键 keys[mid] 直接"提升"给父结点，
  // 不再保留在本层任何一侧（因为它在原结点里只是分隔两个孩子的钥匙）。
  // 原结点保留左半孩子（含被提升键左侧的那个孩子），新建右结点装右半。
  InternalNode* SplitInternal(InternalNode* node, int& promote_key) {
    int mid = static_cast<int>(node->keys.size()) / 2;  // 本例恒为 2

    promote_key = node->keys[mid];  // 要向上提升的分隔键

    // 右结点得到 keys[mid+1..] 以及对应的孩子们 children[mid+1..]
    InternalNode* right = new InternalNode();
    right->keys.assign(node->keys.begin() + mid + 1, node->keys.end());
    right->children.assign(node->children.begin() + mid + 1,
                           node->children.end());
    for (BPlusNode* child : right->children) {
      child->parent = right;  // 被搬走的孩子要改认新父亲
    }

    // 原结点收缩成左结点：保留 keys[0..mid) 与 children[0..mid]
    node->keys.resize(mid);
    node->children.resize(mid + 1);
    return right;
  }

public:
  // 对外插入接口。算法是"先下后上"：
  //   第 1 步：找到应插入的叶子并把键值对放进去（此时树可能已经局部超载）；
  //   第 2 步：从该叶子开始，凡是"键数 > kMaxKeys"的结点就分裂，
  //           并把分裂产生的新键 + 新结点塞给它的父结点；
  //   第 3 步：把"当前结点"上移为父结点，循环直到没有溢出或到达树根。
  // 由于每个结点都带 parent 指针，这种自底向上的迭代写法非常自然。
  void Insert(int key, int value) {
    LeafNode* leaf = FindLeaf(key);   // 1. 先定位到目标叶子
    InsertIntoLeaf(leaf, key, value); // 2. 插入（重复键则更新值，不会溢出）

    // 3. 自底向上"分裂并向上传播"。
    //    cur 是刚刚发生溢出的结点（一开始是叶子，之后可能是内部结点）
    BPlusNode* cur = leaf;
    while (cur->keys.size() > static_cast<size_t>(kMaxKeys)) {
      int promote_key = 0;   // 本次分裂需要交给父结点的键
      BPlusNode* right;      // 本次分裂产生的新右结点
      if (cur->is_leaf) {
        right = SplitLeaf(static_cast<LeafNode*>(cur), promote_key);
      } else {
        right = SplitInternal(static_cast<InternalNode*>(cur), promote_key);
      }

      InternalNode* parent = static_cast<InternalNode*>(cur->parent);
      if (parent == nullptr) {
        // 分裂的是根：树增高一层，新建内部结点作根，包含左半和右半
        InternalNode* new_root = new InternalNode();
        new_root->keys.push_back(promote_key);
        new_root->children.push_back(cur);    // 左半
        new_root->children.push_back(right);  // 右半
        cur->parent = new_root;
        right->parent = new_root;
        root = new_root;
        return;  // 新根只有 1 个键，不可能再溢出
      }

      // 把 (promote_key, right) 塞进父结点。
      // 设 cur 在父结点孩子中的下标是 pos，那么新右结点应紧挨在它右边，
      // 两者之间的分隔键正是 promote_key。
      int pos = IndexOfChild(parent, cur);
      parent->keys.insert(parent->keys.begin() + pos, promote_key);
      parent->children.insert(parent->children.begin() + pos + 1, right);
      right->parent = parent;

      cur = parent;  // 上一层的父结点可能也溢出了，继续循环检查
    }
  }


private:
  // ---------- 删除 / 下溢修复 ----------

  // 叶子结点下溢修复。调用前提：该叶子不是根，且键数已小于 kMinKeys。
  // 整体策略与 B 树如出一辙："先借后并"：
  //   1) 若某个兄弟有多余的键（> kMinKeys），向它借一个键；
  //      借键只是把键值对在两片相邻叶子之间搬动，不改变结点数量；
  //   2) 若两边兄弟都刚好处于最少状态、谁也借不出来，就与兄弟合并。
  void FixLeafUnderflow(LeafNode* leaf) {
    InternalNode* parent = static_cast<InternalNode*>(leaf->parent);
    int pos = IndexOfChild(parent, leaf);

    // 方案一：向右兄弟借。此时本叶为空，把右兄弟的最小键值对搬到本叶末尾。
    if (pos + 1 < static_cast<int>(parent->children.size())) {
      LeafNode* right = static_cast<LeafNode*>(parent->children[pos + 1]);
      if (right->keys.size() > static_cast<size_t>(kMinKeys)) {
        leaf->keys.push_back(right->keys.front());
        leaf->values.push_back(right->values.front());
        right->keys.erase(right->keys.begin());    // keys 与 values 同步删除
        right->values.erase(right->values.begin());
        // 右兄弟的最小键变了，必须更新父结点中隔开二者的分隔键，
        // 否则查找会顺着旧分隔键跑进已经失去该键的右兄弟（假阴性）。
        parent->keys[pos] = right->keys.front();
        return;
      }
    }
    // 方案二：向左兄弟借。把左兄弟的最大键值对搬到本叶头部。
    if (pos > 0) {
      LeafNode* left = static_cast<LeafNode*>(parent->children[pos - 1]);
      if (left->keys.size() > static_cast<size_t>(kMinKeys)) {
        leaf->keys.insert(leaf->keys.begin(), left->keys.back());
        leaf->values.insert(leaf->values.begin(), left->values.back());
        left->keys.pop_back();
        left->values.pop_back();
        // 本叶的最小键发生了变化，父结点分隔键同步更新。
        parent->keys[pos - 1] = leaf->keys.front();
        return;
      }
    }

    // 方案三：两侧都借不出 → 合并。
    if (pos > 0) {
      // 3a. 本叶(右)并入左兄弟(左)：把本叶的键值拷给左兄弟，删除本叶。
      LeafNode* left = static_cast<LeafNode*>(parent->children[pos - 1]);
      for (size_t i = 0; i < leaf->values.size(); ++i) {
        left->keys.push_back(leaf->keys[i]);
        left->values.push_back(leaf->values[i]);
      }
      // 双向链表摘下本叶：left->next 直接指向本叶的右邻
      left->next = leaf->next;
      if (left->next != nullptr) left->next->prev = left;
      // 父结点里同时去掉"孩子指针 + 对应的分隔键"，保持键数 = 孩子数 - 1
      parent->children.erase(parent->children.begin() + pos);
      parent->keys.erase(parent->keys.begin() + pos - 1);
      delete leaf;
    } else {
      // 3b. 本叶是最左孩子、没有左兄弟 → 把右兄弟并入本叶。
      LeafNode* right = static_cast<LeafNode*>(parent->children[pos + 1]);
      for (size_t i = 0; i < right->values.size(); ++i) {
        leaf->keys.push_back(right->keys[i]);
        leaf->values.push_back(right->values[i]);
      }
      // 链表摘下右兄弟
      leaf->next = right->next;
      if (leaf->next != nullptr) leaf->next->prev = leaf;
      parent->children.erase(parent->children.begin() + pos + 1);
      parent->keys.erase(parent->keys.begin() + pos);
      delete right;
    }
    // 父结点少了一个孩子，可能连带下溢，交给公共收尾函数处理。
    HandleChildRemoved(parent);
  }


  // 内部结点下溢修复。调用前提：node 不是根，且孩子数已少于 kMinChildren
  //（对本实现即只剩 1 个孩子、0 个键）。修复方式同样是"先借后并"，
  // 只是这次结点之间流动的不是键值对，而是"分隔键 + 子树指针"。
  // 这个函数可能向上递归：下层合并会导致上层也下溢。
  void FixInternalUnderflow(InternalNode* node) {
    InternalNode* parent = static_cast<InternalNode*>(node->parent);
    int pos = IndexOfChild(parent, node);

    // 方案一：向右兄弟借（右兄弟孩子数 > kMinChildren 才借得起）。
    // 借法：把父结点里的分隔键"下沉"到 node 末尾，同时把右兄弟的
    // 第一个孩子"过继"给 node；随后父结点分隔键更新为右兄弟新的第一个键
    //（也就是右兄弟新的子树最小值）。
    if (pos + 1 < static_cast<int>(parent->children.size())) {
      InternalNode* right = static_cast<InternalNode*>(parent->children[pos + 1]);
      if (right->children.size() > static_cast<size_t>(kMinChildren)) {
        node->keys.push_back(parent->keys[pos]);            // 分隔键下沉
        BPlusNode* borrowed = right->children.front();      // 借走右兄弟
        node->children.push_back(borrowed);                 // 最左孩子
        borrowed->parent = node;                            // 别忘了改认父结点
        parent->keys[pos] = right->keys.front();            // 分隔键上移
        right->keys.erase(right->keys.begin());
        right->children.erase(right->children.begin());
        return;
      }
    }

    // 方案二：向左兄弟借（对称操作，方向相反）。
    if (pos > 0) {
      InternalNode* left = static_cast<InternalNode*>(parent->children[pos - 1]);
      if (left->children.size() > static_cast<size_t>(kMinChildren)) {
        node->keys.insert(node->keys.begin(), parent->keys[pos - 1]);  // 下沉
        BPlusNode* borrowed = left->children.back();  // 借走左兄弟最右孩子
        node->children.insert(node->children.begin(), borrowed);
        borrowed->parent = node;                     // 别忘了改认父结点
        parent->keys[pos - 1] = left->keys.back();   // 上移为左兄弟新最大键
        left->keys.pop_back();
        left->children.pop_back();
        return;
      }
    }

    // 方案三：两侧都借不出 → 合并两个内部结点。
    // 合并时父结点里隔开二者的分隔键要"下沉"进合并结果的正中间，
    // 因为两个孩子合并后，中间少掉的那把"钥匙"必须由合并结点自己保管。
    if (pos > 0) {
      // 3a. 把 node 并入左兄弟 left（node 被删除）。
      InternalNode* left = static_cast<InternalNode*>(parent->children[pos - 1]);
      left->keys.push_back(parent->keys[pos - 1]);  // 分隔键下沉
      for (size_t i = 0; i < node->keys.size(); ++i) {
        left->keys.push_back(node->keys[i]);
      }
      for (size_t i = 0; i < node->children.size(); ++i) {
        left->children.push_back(node->children[i]);
        node->children[i]->parent = left;  // 过继的孩子改认新父亲
      }
      parent->children.erase(parent->children.begin() + pos);
      parent->keys.erase(parent->keys.begin() + pos - 1);
      delete node;
    } else {
      // 3b. node 是最左孩子 → 把右兄弟 right 并入 node。
      InternalNode* right = static_cast<InternalNode*>(parent->children[pos + 1]);
      node->keys.push_back(parent->keys[pos]);  // 分隔键下沉
      for (size_t i = 0; i < right->keys.size(); ++i) {
        node->keys.push_back(right->keys[i]);
      }
      for (size_t i = 0; i < right->children.size(); ++i) {
        node->children.push_back(right->children[i]);
        right->children[i]->parent = node;
      }
      parent->children.erase(parent->children.begin() + pos + 1);
      parent->keys.erase(parent->keys.begin() + pos);
      delete right;
    }
    HandleChildRemoved(parent);  // 上层少了一个孩子，可能继续下溢
  }

  // 公共收尾：某内部结点刚被删掉一个孩子后调用，分两种情况处理：
  //   · 它就是根且只剩 1 个孩子 → 让那个孩子直接当根，树高减 1；
  //   · 它不是根且孩子数跌破下限 → 递归修复内部下溢。
  void HandleChildRemoved(InternalNode* parent) {
    if (parent == root) {
      if (parent->children.size() == 1) {  // 根收缩，树矮一层
        root = parent->children.front();
        root->parent = nullptr;
        delete parent;
      }
      return;
    }
    if (parent->children.size() < static_cast<size_t>(kMinChildren)) {
      FixInternalUnderflow(parent);
    }
  }

public:
  // 对外删除接口：删除成功返回 true，键不存在返回 false。
  bool Remove(int key) {
    LeafNode* leaf = FindLeaf(key);
    int pos = static_cast<int>(
        lower_bound(leaf->keys.begin(), leaf->keys.end(), key) -
        leaf->keys.begin());

    // 键不存在，直接失败
    if (pos == static_cast<int>(leaf->keys.size()) ||
        leaf->keys[pos] != key) {
      return false;
    }

    // 删掉键值对（两个并行数组同步擦除）
    leaf->keys.erase(leaf->keys.begin() + pos);
    leaf->values.erase(leaf->values.begin() + pos);

    // 若被删的键恰好是某内部结点的分隔键副本，它留在内部结点里当
    // "陈旧分隔键"即可（见文件头第 3 节），无需向上清理。

    if (leaf == root) return true;  // 根是叶子时允许它为空（空树）
    if (leaf->keys.size() >= static_cast<size_t>(kMinKeys)) return true;

    FixLeafUnderflow(leaf);  // 跌破最少键数 → 借或并
    return true;
  }


public:
  // ---------- 查询与打印 ----------

  // 点查询：返回 key 对应的 value，找不到返回 -1。
  // 流程：先沿内部结点路由到叶子（FindLeaf），再在叶子里二分精确匹配。
  int Search(int key) {
    LeafNode* leaf = FindLeaf(key);  // 路由到可能含 key 的叶子
    auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (it != leaf->keys.end() && *it == key) {  // 精确命中
      int index = static_cast<int>(it - leaf->keys.begin());
      return leaf->values[index].value;
    }
    return -1;  // 找不到（注意：如果业务上允许 value == -1 需另换哨兵）
  }


  // 范围查询 [start_key, end_key]，闭区间，按 key 升序打印所有 key:value。
  // 这是 B+ 树最亮眼的操作：先路由到起点，然后顺着叶子链表线性扫描，
  // 输出 m 条记录的时间复杂度仅为 O(log n + m)。
  void RangeQuery(int start_key, int end_key) {
    if (start_key > end_key) {
      cout << "（区间非法：start > end）" << endl;
      return;
    }
    LeafNode* leaf = FindLeaf(start_key);  // 定位扫描起点所在的叶子
    bool found_any = false;
    while (leaf != nullptr) {
      for (size_t i = 0; i < leaf->keys.size(); ++i) {
        int k = leaf->keys[i];
        if (k < start_key) continue;    // 本叶前半段比起点小，跳过
        if (k > end_key) {              // 键已升序，遇到更大的即可提前收工
          if (!found_any) cout << "（区间内没有记录）" << endl;
          return;
        }
        cout << k << " : " << leaf->values[i].value << endl;
        found_any = true;
      }
      leaf = leaf->next;  // 顺着叶子链表向右扫
    }
    if (!found_any) cout << "（区间内没有记录）" << endl;
  }


  // 按"层"打印整棵树（BFS 层序遍历），方便观察结构变化。
  // 约定：内部结点打印为  内[key0|key1|...]
  //       叶子结点打印为  叶[key0:v0|key1:v1|...]
  // 配合插入/删除演示可以直观看到：键如何从叶子向上拷贝、树何时长高/变矮。
  void PrintTree() {
    if (root == nullptr) return;
    vector<BPlusNode*> level;      // 当前层的所有结点
    level.push_back(root);
    int depth = 0;
    while (!level.empty()) {
      ++depth;
      cout << "第 " << depth << " 层：";
      vector<BPlusNode*> next_level;  // 下一层（内部结点的孩子们）
      for (BPlusNode* node : level) {
        if (node->is_leaf) {
          LeafNode* leaf = static_cast<LeafNode*>(node);
          cout << "叶[";
          for (size_t i = 0; i < leaf->keys.size(); ++i) {
            cout << leaf->keys[i] << ":" << leaf->values[i].value;
            if (i + 1 < leaf->keys.size()) cout << "|";
          }
          cout << "]  ";
        } else {
          InternalNode* internal = static_cast<InternalNode*>(node);
          cout << "内[";
          for (size_t i = 0; i < internal->keys.size(); ++i) {
            cout << internal->keys[i];
            if (i + 1 < internal->keys.size()) cout << "|";
          }
          cout << "]  ";
          // 把孩子们收集起来作为下一层打印
          next_level.insert(next_level.end(), internal->children.begin(),
                            internal->children.end());
        }
      }
      cout << endl;
      if (next_level.empty()) break;  // 到叶子层就结束（叶子没有孩子）
      level.swap(next_level);
    }
  }

private:
  // -------------------------------------------------------------------------
  // 结构自检（教学辅助，不属于 B+ 树基本操作）
  // -------------------------------------------------------------------------

  // 递归校验一棵子树并回传其键值范围与高度。
  //   · is_root：当前结点是否是整棵树的根（根允许更宽松的下限）；
  //   · out_min / out_max：该子树中的最小 / 最大键；
  //   · out_height：从该结点到叶子的层数（叶子记 0）。
  bool CheckSubtree(BPlusNode* node, bool is_root, int& out_min, int& out_max,
                    int& out_height) {
    if (node->is_leaf) {
      LeafNode* leaf = static_cast<LeafNode*>(node);
      size_t n = leaf->keys.size();
      if (leaf->values.size() != n) {
        cerr << "[自检失败] 叶子 keys/values 数量不一致" << endl;
        return false;
      }
      if (n > static_cast<size_t>(kMaxKeys) ||
          (!is_root && n < static_cast<size_t>(kMinKeys))) {
        cerr << "[自检失败] 叶子键数越界：" << n << endl;
        return false;
      }
      for (size_t i = 1; i < n; ++i) {
        if (leaf->keys[i - 1] >= leaf->keys[i]) {
          cerr << "[自检失败] 叶子内键未严格递增" << endl;
          return false;
        }
      }
      out_height = 0;
      if (n == 0) return true;  // 只有空树（根为叶子）会走到这里
      out_min = leaf->keys.front();
      out_max = leaf->keys.back();
      return true;
    }
    InternalNode* internal = static_cast<InternalNode*>(node);
    if (internal->keys.size() + 1 != internal->children.size()) {
      cerr << "[自检失败] 内部结点 children 数 != keys 数 + 1" << endl;
      return false;
    }
    size_t children_n = internal->children.size();
    if (children_n > static_cast<size_t>(kOrder) ||
        (!is_root && children_n < static_cast<size_t>(kMinChildren))) {
      cerr << "[自检失败] 内部结点孩子数越界：" << children_n << endl;
      return false;
    }
    for (size_t i = 1; i < internal->keys.size(); ++i) {
      if (internal->keys[i - 1] >= internal->keys[i]) {
        cerr << "[自检失败] 内部结点分隔键未严格递增" << endl;
        return false;
      }
    }

    // 逐个校验孩子：parent 指针、子树合法性、子树高度一致，
    // 以及"路由括号"不变量 max(孩子i) < keys[i] ≤ min(孩子i+1)。
    int height = -1;                     // 各孩子子树的高度（应完全相同）
    bool first = true;                   // 是否正在处理第一个孩子
    int prev_child_max = INT_MIN;        // 上一孩子的最大值，用于括号校验
    for (size_t i = 0; i < children_n; ++i) {
      BPlusNode* child = internal->children[i];
      if (child->parent != internal) {
        cerr << "[自检失败] 孩子结点 parent 指针未指向父结点" << endl;
        return false;
      }
      int child_min = 0, child_max = 0, child_height = 0;
      if (!CheckSubtree(child, false, child_min, child_max, child_height)) {
        return false;
      }
      if (first) {
        first = false;
        out_min = child_min;
        height = child_height;
      } else {
        if (child_height != height) {
          cerr << "[自检失败] 各叶子所在层高度不一致" << endl;
          return false;
        }
        // 括号不变量：上一孩子全部键 < keys[i-1] ≤ 本孩子全部键
        int sep = internal->keys[i - 1];
        if (!(prev_child_max < sep && sep <= child_min)) {
          cerr << "[自检失败] 路由括号不变量被破坏，分隔键=" << sep << endl;
          return false;
        }
      }
      prev_child_max = child_max;
      out_max = child_max;
    }
    out_height = height + 1;
    return true;
  }


public:
  // 对外自检入口：返回整棵树是否满足所有 B+ 树不变量。
  // 先做"结构 + 路由括号"递归检查，再顺着叶子链表检查全局有序无重复。
  // 该函数主要用于教学演示与随机测试，不属于树的正常操作。
  bool ValidateTree() {
    if (root == nullptr) {
      cerr << "[自检失败] 根为空" << endl;
      return false;
    }

    // 1) 递归结构检查
    int dummy_min = 0, dummy_max = 0, dummy_height = 0;
    if (!CheckSubtree(root, true, dummy_min, dummy_max, dummy_height)) {
      return false;
    }

    // 2) 沿叶子链表检查：全局严格递增 + 无重复键。
    //    先下探到最左叶子。
    BPlusNode* cur = root;
    while (!cur->is_leaf) {
      cur = static_cast<InternalNode*>(cur)->children.front();
    }
    LeafNode* leaf = static_cast<LeafNode*>(cur);
    int prev_key = INT_MIN;  // 上一个遍历到的键
    int total = 0;           // 叶子键总数（仅用于计数演示）
    while (leaf != nullptr) {
      for (size_t i = 0; i < leaf->keys.size(); ++i) {
        if (leaf->keys[i] <= prev_key) {
          cerr << "[自检失败] 叶子链表未保持全局严格递增（重复或乱序）" << endl;
          return false;
        }
        prev_key = leaf->keys[i];
        ++total;
      }
      leaf = leaf->next;  // 向右走到下一片叶子
    }
    (void)total;
    cout << "[自检] 通过：树结构合法。" << endl;
    return true;
  }
};  // class BPlusTree

}  // namespace b_plus_tree

// =============================================================================
// 演示与测试：main()
//   演示 1：依次插入 1..16，在若干关键结点打印树结构（观察叶子分裂、
//           键向上拷贝、树长高）；
//   演示 2：删除若干键，观察“先借后并”与树高收缩；
//   演示 3：与 std::map 对照的随机压力测试（借助 ValidateTree 自检）。
// =============================================================================
int main() {
  using namespace b_plus_tree;

  // ---------------------------- 演示 1：插入 --------------------------------
  cout << "============================================================\n"
       << "演示 1：依次插入键 1..16，value = key*10\n"
       << "（阶 M=4 → 每叶最多 3 键，满 4 个就分裂）\n"
       << "============================================================\n";
  BPlusTree tree;
  for (int key = 1; key <= 16; ++key) {
    tree.Insert(key, key * 10);
    // 只在有“看点”的步骤打印：满叶、首次长高、根再次分裂……
    if (key == 3 || key == 4 || key == 6 || key == 8 || key == 10 ||
        key == 16) {
      cout << "---- 刚插入 " << key << " 后 ----" << endl;
      tree.PrintTree();
    }
  }
  tree.ValidateTree();

  cout << "\n[点查询] Search(7)  = " << tree.Search(7)
       << "（期望 70）\n";
  cout << "[点查询] Search(20) = " << tree.Search(20)
       << "（期望 -1：不存在）\n";

  cout << "\n[范围查询] RangeQuery(4, 12)：\n";
  tree.RangeQuery(4, 12);


  // ---------------------------- 演示 2：删除 --------------------------------
  cout << "\n============================================================\n"
       << "演示 2：删除若干键，观察“借键 / 合并 / 根收缩”\n"
       << "============================================================\n";
  // 先删 4、5、6：每次删除后打印，观察叶子借键、合并与分隔键更新
  for (int key : {4, 5, 6}) {
    cout << "---- 删除 " << key << " 后 ----" << endl;
    tree.Remove(key);
    tree.PrintTree();
  }

  // 再删 1、2、3、7、8、9、10，让左侧子树逐步收缩
  for (int key : {1, 2, 3, 7, 8, 9, 10}) {
    tree.Remove(key);
  }
  cout << "---- 继续删除 1、2、3、7、8、9、10 之后 ----" << endl;
  tree.PrintTree();
  // 删光剩余键，应回到空树（根为一片空叶子）
  for (int key : {11, 12, 13, 14, 15, 16}) {
    tree.Remove(key);
  }
  cout << "---- 全部删除之后（空树） ----" << endl;
  tree.PrintTree();
  cout << "Search(5) = " << tree.Search(5) << endl;
  tree.ValidateTree();





  // ---------------------------- 演示 3：随机对照 ----------------------------
  cout << "\n============================================================\n"
       << "演示 3：与 std::map 对照的随机压力测试（固定随机种子，可复现）\n"
       << "============================================================\n";
  BPlusTree t;
  std::map<int, int> ref;                    // “标准答案”参考实现
  std::mt19937 gen(20240904);                // 固定种子，结果可复现
  std::uniform_int_distribution<int> key_dist(0, 199);
  std::uniform_int_distribution<int> op_dist(0, 99);

  bool all_ok = true;
  for (int step = 0; step < 3000; ++step) {
    bool do_insert = op_dist(gen) < 55;  // 55% 概率插入
    int key = key_dist(gen);
    if (do_insert) {
      int value = key * 3 + 1;
      t.Insert(key, value);
      ref[key] = value;  // map：重复插入覆盖，与我们的语义一致
    } else {
      bool removed = t.Remove(key);
      if (removed != (ref.erase(key) == 1)) {  // 两边“是否存在”必须一致
        cout << "随机测试失败：Remove(" << key << ") 结果不一致" << endl;
        all_ok = false;
        break;
      }
    }
    // 每隔若干步做一次全面结构自检 + 抽样对照
    if (step % 100 == 0) {
      if (!t.ValidateTree()) {
        all_ok = false;
        break;
      }
      for (int i = 0; i < 50; ++i) {  // 随机抽 50 个键做点查询对照
        int k = key_dist(gen);
        int expect = ref.count(k) ? ref.at(k) : -1;
        if (t.Search(k) != expect) {
          cout << "随机测试失败：Search(" << k << ")=" << t.Search(k)
               << "，期望 " << expect << endl;
          all_ok = false;
          break;
        }
      }
      if (!all_ok) break;
    }
  }

  if (all_ok) {
    cout << "随机压力测试通过：3000 次随机插入/删除均与 std::map 一致，\n"
         << "且树结构每次自检都合法。" << endl;
  }
  return all_ok ? 0 : 1;
}

