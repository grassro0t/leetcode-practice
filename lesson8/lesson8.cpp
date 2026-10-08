#include <iostream>
#include <vector>

namespace lesson8 {
using namespace std;
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};
class Solution {
public:
  // 94｜二叉树的中序遍历｜简单
  vector<int> inorderTraversal(TreeNode *root) {
    vector<int> res;
    stack<TreeNode *> st;
    TreeNode *cur = root;
    while (cur || !st.empty()) {
      while (cur) {
        st.push(cur);
        cur = cur->left;
      }
      cur = st.top();
      res.emplace_back(cur->val);
      st.pop();
      cur = cur->right;
    }
    return res;
  }
  vector<int> preorderTraversal(TreeNode *root) {
    vector<int> res;
    if (!root)
      return res;
    stack<TreeNode *> st;
    st.push(root);
    while (!st.empty()) {
      auto cur = st.top();
      st.pop();
      res.push_back(cur->val);
      if (cur->right)
        st.push(cur->right);
      if (cur->left)
        st.push(cur->left);
    }
    return res;
  }
  vector<int> postorderTraversal(TreeNode *root) {
    vector<int> res;
    if (!root)
      return res;
    stack<TreeNode *> st;
    st.push(root);
    while (!st.empty()) {
      auto cur = st.top();
      st.pop();
      res.push_back(cur->val);
      if (cur->left)
        st.push(cur->left);
      if (cur->right)
        st.push(cur->right);
    }
    reverse(res.begin(), res.end());
    return res;
  }
  // 104｜二叉树的最大深度｜简单
  int maxDepth(TreeNode *root) {
    if (!root)
      return 0;
    int deep = 0;
    vector<TreeNode *> vec;
    vec.emplace_back(root);
    while (!vec.empty()) {
      vector<TreeNode *> temp;
      for (auto &&node : vec) {
        if (node->left)
          temp.emplace_back(node->left);
        if (node->right)
          temp.emplace_back(node->right);
      }
      vec = temp;
      deep++;
    }
    return deep;
  }
  // 101｜对称二叉树｜简单
  bool isSymmetric(TreeNode *root) { return check(root->left, root->right); }

  bool check(TreeNode *l, TreeNode *r) {
    if (!l && !r)
      return true;
    if (!l || !r)
      return false;
    return l->val == r->val && check(l->left, r->right) &&
           check(l->right, r->left);
  }
  // 543｜二叉树的直径｜简单
  int max_val = 0;
  int dfs(TreeNode *root) {
    if (!root)
      return 0;
    int l = dfs(root->left);
    int r = dfs(root->right);
    max_val = max(max_val, l + r);
    return max(l, r) + 1;
  }
  int diameterOfBinaryTree(TreeNode *root) {
    dfs(root);
    return max_val;
  }
  // 108｜将有序数组转换为二叉搜索树｜简单
  TreeNode *dfs(vector<int> &nodes, int l, int r) {
    if (l > r)
      return nullptr;
    int mid = l + (r - l) / 2;
    TreeNode *cur = new TreeNode(nodes[mid]);
    cur->left = dfs(nodes, l, mid - 1);
    cur->right = dfs(nodes, mid + 1, r);
    return cur;
  }
  TreeNode *sortedArrayToBST(vector<int> &nums) {
    return dfs(nums, 0, nums.size() - 1);
  }
  // 102｜二叉树的层序遍历｜中等
  vector<vector<int>> levelOrder(TreeNode *root) {
    vector<TreeNode *> st;
    vector<vector<int>> res;
    if (!root)
      return res;
    st.emplace_back(root);
    while (!st.empty()) {
      vector<TreeNode *> temp;
      vector<int> subres;
      for (int i = 0; i < st.size(); i++) {
        subres.emplace_back(st[i]->val);
        if (st[i]->left)
          temp.emplace_back(st[i]->left);
        if (st[i]->right)
          temp.emplace_back(st[i]->right);
      }
      st = temp;
      res.emplace_back(subres);
    }
    return res;
  }
  // 98｜验证二叉搜索树｜中等
  long long pre = LONG_MIN;
  bool res = true;
  bool isValidBST(TreeNode *root) {
    inorder(root);
    return res;
  }
  void inorder(TreeNode *root) {
    if (!root)
      return;
    inorder(root->left);
    if (pre >= root->val)
      res = false;
    pre = root->val;
    inorder(root->right);
  }
  // 230｜二叉搜索树中第 K 小的元素｜中等
  int k;
  int res = -1;
  int kthSmallest(TreeNode *root, int k) {
    this->k = k;
    inorder(root);
    return res;
  }

  void inorder(TreeNode *root) {
    if (!root || res != -1)
      return;
    inorder(root->left);
    k--;
    if (k == 0) {
      res = root->val;
      return;
    }
    inorder(root->right);
  }
  // 199｜二叉树的右视图｜中等
  vector<int> rightSideView(TreeNode *root) {
    vector<TreeNode *> vec;
    vector<int> res;
    if (!root)
      return res;
    vec.emplace_back(root);
    while (!vec.empty()) {
      vector<TreeNode *> temp;
      res.emplace_back(vec.back()->val);
      for (int i = 0; i < vec.size(); i++) {
        if (vec[i]->left)
          temp.emplace_back(vec[i]->left);
        if (vec[i]->right)
          temp.emplace_back(vec[i]->right);
      }
      vec = temp;
    }
    return res;
  }
  // 114｜二叉树展开为链表｜中等
  void flatten(TreeNode *root) {
    stack<TreeNode *> st;
    if (!root)
      return;
    st.push(root);
    vector<TreeNode *> res;
    while (!st.empty()) {
      TreeNode *temp = st.top();
      res.emplace_back(temp);
      st.pop();
      if (temp->right)
        st.push(temp->right);
      if (temp->left)
        st.push(temp->left);
    }
    for (int i = 0; i < res.size() - 1; i++) {
      res[i]->left = nullptr;
      res[i]->right = res[i + 1];
    }
    res[res.size() - 1]->left = nullptr;
    res[res.size() - 1]->right = nullptr;
  }
  // 105｜从前序与中序遍历序列构造二叉树｜中等
  vector<int> preorder_;
  unordered_map<int, int> um;
  TreeNode *recure(int ro, int l, int r) {
    if (l > r)
      return nullptr;
    TreeNode *root = new TreeNode(preorder_[ro]);
    int root_idx = um[root->val];
    root->left = recure(ro + 1, l, root_idx - 1);
    root->right = recure(ro + 1 + root_idx - l, root_idx + 1, r);
    return root;
  }
  TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
    preorder_ = preorder;
    for (int i = 0; i < inorder.size(); i++) {
      um[inorder[i]] = i;
    }
    return recure(0, 0, preorder_.size() - 1);
  }
  // 437｜路径总和 III｜中等
  unordered_map<long long, int> um;
  int pathSum(TreeNode *root, int targetSum) {
    um[0] = 1;
    return dfs(root, 0, targetSum);
  }

  int dfs(TreeNode *root, long long preSum, int targetSum) {
    if (!root)
      return 0;
    preSum += root->val;
    int res = 0;
    if (um.contains(preSum - targetSum))
      res += um[preSum - targetSum];
    um[preSum]++;
    res += dfs(root->left, preSum, targetSum);
    res += dfs(root->right, preSum, targetSum);
    um[preSum]--;
    if (um[preSum] == 0)
      um.erase(preSum);
    preSum -= root->val;
    return res;
  }
  // 236｜二叉树的最近公共祖先｜中等
  TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    if (!root || root == p || root == q)
      return root;
    TreeNode *l = lowestCommonAncestor(root->left, p, q);
    TreeNode *r = lowestCommonAncestor(root->right, p, q);
    if (!l && !r)
      return nullptr;
    if (l && !r)
      return l;
    if (!l && r)
      return r;
    return root;
  }
  // 46｜全排列｜中等
  vector<vector<int>> myres;
  void dfs(vector<int> &nums, int x) {
    if (x == nums.size() - 1)
      myres.emplace_back(nums);
    for (int i = x; i < nums.size(); i++) {
      swap(nums[i], nums[x]);
      dfs(nums, x + 1);
      swap(nums[i], nums[x]);
    }
  }
  vector<vector<int>> permute(vector<int> &nums) {
    dfs(nums, 0);
    return myres;
  }
  // 78｜子集｜中等
  vector<vector<int>> sub;
  vector<vector<int>> subsets(vector<int> &nums) {
    vector<int> before;
    dfs(nums, 0, before);
    return sub;
  }
  void dfs(vector<int> &nums, int x, vector<int> &before) {
    if (x == nums.size()) {
      sub.emplace_back(before);
      return;
    }
    dfs(nums, x + 1, before);
    before.emplace_back(nums[x]);
    dfs(nums, x + 1, before);
    before.pop_back();
  }
  // 39｜组合总和｜中等
  vector<vector<int>> vec;
  vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
    vector<int> temp;
    dfs(candidates, target, 0, 0, temp);
    return vec;
  }

  void dfs(vector<int> &nums, int target, int sumVal, int x,
           vector<int> &sumVec) {
    if (x > nums.size() - 1 || sumVal > target)
      return;
    if (sumVal == target) {
      vec.emplace_back(sumVec);
      return;
    }
    sumVec.emplace_back(nums[x]);
    dfs(nums, target, sumVal + nums[x], x, sumVec);
    sumVec.pop_back();
    dfs(nums, target, sumVal, x + 1, sumVec);
  }
  // 17｜电话号码的字母组合｜中等
  unordered_map<int, vector<char>> um;
  vector<string> res17;
  vector<string> letterCombinations(string digits) {
    um[2] = {'a', 'b', 'c'}, um[3] = {'d', 'e', 'f'}, um[4] = {'g', 'h', 'i'};
    um[5] = {'j', 'k', 'l'}, um[6] = {'m', 'n', 'o'},
    um[7] = {'p', 'q', 'r', 's'};
    um[8] = {'t', 'u', 'v'}, um[9] = {'w', 'x', 'y', 'z'};
    string temp;
    dfs(digits, 0, temp);
    return res17;
  }

  void dfs(string &digits, int x, string &sum) {
    if (x == digits.size()) {
      res17.emplace_back(sum);
      return;
    }
    auto ums = um[digits[x] - '0'];
    for (auto &&c : ums) {
      sum += c;
      dfs(digits, x + 1, sum);
      sum.pop_back();
    }
  }
  // 124｜二叉树中的最大路径和｜困难
  int max_len = INT_MIN;
  int maxPathSum(TreeNode *root) {
    dfs(root);
    return max_len;
  }

  int dfs(TreeNode *root) {
    if (!root)
      return 0;
    int l = dfs(root->left);
    int r = dfs(root->right);
    max_len = max(max_len, l + r + root->val);
    return max(max(l, r) + root->val, 0);
  }
  // 51｜N 皇后｜困难
  vector<vector<string>> res;
  int n;
  vector<vector<string>> solveNQueens(int n) {
    this->n = n;
    vector<string> board(n, string(n, '.'));
    bt(board, 0);
    return res;
  }

  void bt(vector<string> &board, int x) {
    if (x == n) {
      res.emplace_back(board);
      return;
    }
    for (int y = 0; y < n; y++) {
      if (isvalid(board, x, y)) {
        board[x][y] = 'Q';
        bt(board, x + 1);
        board[x][y] = '.';
      }
    }
  }

  bool isvalid(vector<string> &board, int x, int y) {
    for (int i = 0; i < x; i++) {
      if (board[i][y] == 'Q')
        return false;
      for (int j = 0; j < n; j++) {
        if (board[i][j] == 'Q' && abs(i - x) == abs(j - y))
          return false;
      }
    }
    return true;
  }
};
} // namespace lesson8

int main() {
  using namespace lesson8;
  return 0;
}