#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>


namespace skip_list {
using namespace std;
struct SkipNode {
  int key;
  int val;
  vector<SkipNode*> forward;
  SkipNode(int k, int v, int level) : key(k), val(v), forward(level, nullptr) {}
};
class SkipList {
 private:
  static constexpr int kMaxLevel = 16;
  static constexpr double kP = 0.5;
  SkipNode* head;
  int cur_level;
  int Radomlevel() {
    int level = 1;
    while ((rand() / double(RAND_MAX)) < kP && level < kMaxLevel) {
      ++level;
    }
    return level;
  }

 public:
  SkipList() {
    srand((unsigned)time(nullptr));
    head = new SkipNode(-1, -1, kMaxLevel);
    cur_level = 0;
  }
  ~SkipList() {
    SkipNode* cur = head;
    while (cur) {
      SkipNode* next = cur->forward[0];
      delete cur;
      cur = next;
    }
  }
  int Find(int key) {
    SkipNode* cur = head;
    for (int i = cur_level; i >= 0; --i) {
      while (cur->forward[i] && cur->forward[i]->key < key) {
        cur = cur->forward[i];
      }
    }
    cur = cur->forward[0];
    if (cur && cur->key == key) return cur->val;
    return -1;
  }
  void Insert(int key, int val) {
    vector<SkipNode*> update(kMaxLevel, nullptr);
    SkipNode* cur = head;
    for (int i = cur_level; i >= 0; --i) {
      while (cur->forward[i] && cur->forward[i]->key < key) {
        cur = cur->forward[i];
      }
      update[i] = cur;
    }
    int new_lv = Radomlevel();
    if (new_lv > cur_level) {
      for (size_t i = cur_level + 1; i < new_lv; ++i) {
        update[i] = head;
      }
      cur_level = new_lv;
    }
    SkipNode* new_node = new SkipNode(key, val, new_lv);
    for (size_t i = 0; i < cur_level; ++i) {
      new_node->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = new_node;
    }
  }
  void Remove(int key) {
    vector<SkipNode*> update(kMaxLevel, nullptr);
    SkipNode* cur = head;
    for (int i = cur_level; i >= 0; --i) {
      while (cur->forward[i] && cur->forward[i]->key < key) {
        cur = cur->forward[i];
      }
      update[i] = cur;
    }
    cur = cur->forward[0];
    if (cur && cur->key == key) {
      for (size_t i = 0; i < cur_level; ++i) {
        if (update[i]->forward[i] == cur) {
          update[i]->forward[i] = cur->forward[i];
        }
      }
      delete cur;
      // 降低最高层
      while (cur_level > 0 && head->forward[cur_level-1] == nullptr) {
        --cur_level;
      }
    }
  }
  void Print() {
    SkipNode* cur = head->forward[0];
    while (cur) {
      cout << cur->key << ":" << cur->val << " ";
      cur = cur->forward[0];
    }
    cout << endl;
  }
};
}  // namespace skip_list
int main() {
  skip_list::SkipList sl;
  sl.Insert(3, 30);
  sl.Insert(1, 10);
  sl.Insert(7, 70);
  sl.Insert(5, 50);
  std::cout << "底层链表：";
  sl.Print();

  std::cout << "find(5)=" << sl.Find(5) << std::endl;
  sl.Remove(5);
  std::cout << "删除5后：";
  sl.Print();
  return 0;
}