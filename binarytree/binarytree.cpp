#include <iostream>
#include <ranges>
#include <stack>

namespace binarytree {
using namespace std;
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {};
};
class Solution {
public:
  // 前序遍历
  vector<int> preorder(TreeNode *root) {
    vector<int> res;
    if (!root)
      return res;
    stack<TreeNode *> st;
    st.push(root);
    while (!st.empty()) {
      TreeNode *cur = st.top();
      res.emplace_back(cur->val);
      st.pop();
      if (cur->right)
        st.push(cur->right);
      if (cur->left)
        st.push(cur->left);
    }
    return res;
  }
  vector<int> pre_res;
  void preorder2(TreeNode *root) {
    if (!root)
      return;
    pre_res.emplace_back(root->val);
    preorder2(root->left);
    preorder2(root->right);
  }
  // 中序遍历
  vector<int> inorder(TreeNode *root) {
    stack<TreeNode *> st;
    vector<int> res;
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
  vector<int> in_res;
  void inorder2(TreeNode *root) {
    if (!root)
      return;
    inorder2(root->left);
    in_res.emplace_back(root->val);
    inorder2(root->right);
  }
  // 后序遍历
  vector<int> preorder(TreeNode *root) {
    vector<int> res;
    if (!root)
      return res;
    stack<TreeNode *> st;
    st.push(root);
    while (!st.empty()) {
      TreeNode *cur = st.top();
      res.emplace_back(cur->val);
      st.pop();
      if (cur->left)
        st.push(cur->left);
      if (cur->right)
        st.push(cur->right);
    }
    ranges::reverse(res);
    return res;
  }
  vector<int> post_res;
  void postorder2(TreeNode *root) {
    if (!root)
      return;
    postorder2(root->left);
    postorder2(root->right);
    post_res.emplace_back(root->val);
  }
  // 层序遍历
  vector<int> LevelOrder(TreeNode *root) {
    vector<int> res;
    if (!root)
      return res;
    vector<TreeNode *> q;
    q.emplace_back(root);
    while (!q.empty()) {
      vector<TreeNode *> vec;
      while (!q.empty()) {
        TreeNode *temp = q.front();
        q.erase(q.begin());
        res.emplace_back(temp->val);
        if (temp->left)
          vec.emplace_back(temp->left);
        if (temp->right)
          vec.emplace_back(temp->right);
      }
      q = vec;
    }
    return res;
  }
};
} // namespace binarytree

int main() {
  using namespace binarytree;
  return 0;
}