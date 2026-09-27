#include <iostream>
#include <string>
#include <vector>

namespace lesson4 {
using namespace std;
struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
  // 20｜有效的括号｜简单
  bool isValid(string s) {
    vector<char> st;

    for (auto &&c : s) {
      switch (c) {
      case '(':
      case '[':
      case '{':
        st.emplace_back(c);
        break;
      case ')': {
        if (st.empty())
          return false;
        char tail = st.back();
        if (tail == '(')
          st.pop_back();
        else
          return false;
        break;
      }
      case ']': {
        if (st.empty())
          return false;
        char tail = st.back();
        if (tail == '[')
          st.pop_back();
        else
          return false;
        break;
      }
      case '}': {
        if (st.empty())
          return false;
        char tail = st.back();
        if (tail == '{')
          st.pop_back();
        else
          return false;
        break;
      }
      default:
        break;
      }
    }
    return st.empty();
  }
  // 23｜合并 K 个升序链表｜困难
  ListNode *mergeKLists(vector<ListNode *> &lists) {
    ListNode dummy(-1);
    ListNode *cur = &dummy;
    ListNode *head;
    auto cmp = [](ListNode *a, ListNode *b) { return a->val > b->val; };
    priority_queue<ListNode *, vector<ListNode *>, decltype(cmp)> minheap;
    for (auto &&node : lists) {
      if (!node)
        continue;
      minheap.push(node);
    }
    while (!minheap.empty()) {
      cur->next = minheap.top();
      minheap.pop();
      cur = cur->next;
      if (cur->next)
        minheap.push(cur->next);
    }
    return dummy.next;
  }
};
} // namespace lesson4

int main() {
  using namespace lesson4;
  return 0;
}