#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace lesson3 {
using namespace std;
class Solution {
  struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
  };
  struct Node {
    int val;
    Node *next;
    Node *random;
    Node() : val(0), next(nullptr), random(nullptr) {}
    Node(int _val) : val(_val), next(nullptr), random(nullptr) {}
    Node(int _val, Node *_next, Node *_random)
        : val(_val), next(_next), random(_random) {}
  };
  // 160｜相交链表｜简单
  ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    ListNode *A = headA;
    ListNode *B = headB;
    while (A != B) {
      A = A == nullptr ? headB : A->next;
      B = B == nullptr ? headA : B->next;
    }
    return A;
  }
  // 206｜反转链表｜简单
  ListNode *reverseList(ListNode *head) {
    ListNode *cur = head;
    ListNode *pre = nullptr;
    while (cur != nullptr) {
      ListNode *nex = cur->next;
      cur->next = pre;
      pre = cur;
      cur = nex;
    }
    return pre;
  }
  // 234｜回文链表｜简单
  bool isPalindrome(ListNode *head) {
    ListNode *fast = head;
    ListNode *slow = head;
    while (fast != nullptr) {
      if (fast->next == nullptr) {
        fast = nullptr;
      } else {
        fast = fast->next->next;
        slow = slow->next;
      }
    }
    ListNode *bk_head = ReverseList(slow);
    while (bk_head != nullptr) {
      if (bk_head->val != head->val) {
        return false;
      }
      bk_head = bk_head->next;
      head = head->next;
    }
    return true;
  }
  ListNode *ReverseList(ListNode *head) {
    ListNode *cur = head;
    ListNode *pre = nullptr;
    while (cur != nullptr) {
      ListNode *temp = cur->next;
      cur->next = pre;
      pre = cur;
      cur = temp;
    }
    return pre;
  }
  // 141｜环形链表｜简单
  bool hasCycle(ListNode *head) {
    ListNode *fast = head;
    ListNode *slow = head;
    while (fast && fast->next) {
      fast = fast->next->next;
      slow = slow->next;
      if (fast == slow)
        return true;
    }
    return false;
  }
  // 21｜合并两个有序链表｜简单
  ListNode *mergeTwoLists(ListNode *list1, ListNode *list2) {
    ListNode *p1 = list1;
    ListNode *p2 = list2;
    ListNode *head = new ListNode(-1);
    ListNode *cur = head;
    while (p1 && p2) {
      if (p1->val < p2->val) {
        cur->next = p1;
        p1 = p1->next;
      } else {
        cur->next = p2;
        p2 = p2->next;
      }
      cur = cur->next;
    }
    cur->next = p1 ? p1 : p2;
    ListNode *res = head->next;
    delete (head);
    return res;
  }
  // 142｜环形链表 II｜中等
  ListNode *detectCycle(ListNode *head) {
    ListNode *fast = head;
    ListNode *slow = head;
    ListNode *connection = nullptr;
    while (fast && fast->next) {
      fast = fast->next->next;
      slow = slow->next;
      if (fast == slow) {
        connection = fast;
        while (connection != head) {
          connection = connection->next;
          head = head->next;
        }
        return connection;
      }
    }
    return nullptr;
  }
  // 2｜两数相加｜中等
  ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    ListNode *cur = new ListNode(-1);
    ListNode *head = cur;
    int count = 0;
    while (l1 && l2) {
      int sum = l1->val + l2->val + count;
      count = sum / 10;
      cur->next = new ListNode(sum % 10);
      ;
      cur = cur->next;
      l1 = l1->next;
      l2 = l2->next;
    }
    while (l1) {
      int sum = l1->val + count;
      count = sum / 10;
      cur->next = new ListNode(sum % 10);
      cur = cur->next;
      l1 = l1->next;
    }
    while (l2) {
      int sum = l2->val + count;
      count = sum / 10;
      cur->next = new ListNode(sum % 10);
      cur = cur->next;
      l2 = l2->next;
    }
    if (count)
      cur->next = new ListNode(1);
    return head->next;
  }
  // 19｜删除链表的倒数第 N 个结点｜中等
  ListNode *removeNthFromEnd(ListNode *head, int n) {
    ListNode dummy(-1, head);
    ListNode *ppre = nullptr;
    ListNode *pre = &dummy;
    ListNode *cur = pre;
    for (int i = 0; i < n; i++) {
      cur = cur->next;
    }
    while (cur) {
      ppre = pre;
      pre = pre->next;
      cur = cur->next;
    }
    ppre->next = pre->next;
    pre->next = nullptr;
    return dummy.next;
  }
  // 24｜两两交换链表中的节点｜中等
  ListNode *swapPairs(ListNode *head) {
    if (!head)
      return nullptr;
    ListNode *cur = head->next;
    ListNode *pre = head;
    ListNode dummy = ListNode(-1, head);
    ListNode *ppre = &dummy;
    while (pre && cur) {
      pre->next = cur->next;
      cur->next = pre;
      ppre->next = cur;
      ppre = pre;
      pre = pre->next;
      cur = pre ? pre->next : nullptr;
    }
    return dummy.next;
  }
  // 138｜复制带随机指针的链表｜中等
  Node *copyRandomList(Node *head) {
    Node *cur = head;
    while (cur) {
      cur->next = new Node(cur->val, cur->next, nullptr);
      cur = cur->next->next;
    }
    cur = head;
    while (cur) {
      if (cur->random) {
        cur->next->random = cur->random->next;
      }
      cur = cur->next->next;
    }
    Node dummy(-1);
    Node *new_head = &dummy;
    cur = head;
    while (cur) {
      new_head->next = cur->next;
      new_head = new_head->next;
      cur->next = cur->next->next;
      cur = cur->next;
    }
    return dummy.next;
  }
  // 148｜排序链表｜中等
  ListNode *sortList(ListNode *head) {
    if (!head || !head->next)
      return head;
    ListNode *middle = middleList(head);
    ListNode *left = sortList(head);
    ListNode *right = sortList(middle);
    return mergeList(left, right);
  }
  ListNode *middleList(ListNode *head) {
    ListNode *fast = head;
    ListNode *slow = head;
    ListNode *pre = head;
    while (fast && fast->next) {
      pre = slow;
      slow = slow->next;
      fast = fast->next->next;
    }
    pre->next = nullptr;
    return slow;
  }
  ListNode *mergeList(ListNode *list1, ListNode *list2) {
    ListNode dummy(-1);
    ListNode *cur = &dummy;
    while (list1 && list2) {
      if (list1->val < list2->val) {
        cur->next = list1;
        list1 = list1->next;
      } else {
        cur->next = list2;
        list2 = list2->next;
      }
      cur = cur->next;
    }
    cur->next = list1 ? list1 : list2;
    return dummy.next;
  }
  // 25｜K 个一组翻转链表｜困难
  ListNode *reverseKGroup(ListNode *head, int k) {
    ListNode dummy(0, head);
    ListNode *last_tail = &dummy; // 上一组翻转后的尾节点

    // k 个一组处理
    while (true) {
      // 看看这一组是否有 k 个节点
      ListNode *cur = last_tail;
      for (int i = 0; i < k; i++) {
        cur = cur->next;
        if (cur == nullptr) { // 不足 k 个节点
          return dummy.next;
        }
      }

      ListNode *pre = nullptr;
      cur = last_tail->next;
      for (int i = 0; i < k; i++) { // 同 92 题
        ListNode *nxt = cur->next;
        cur->next = pre; // 每次循环只修改一个 next，方便大家理解
        pre = cur;
        cur = nxt;
      }

      // 请结合视频中的图理解
      // 翻转后：
      // pre 是当前组的头节点
      // cur 是下一组的起始节点
      // last_tail 是上一组的尾节点
      // last_tail->next 是当前组的尾节点
      ListNode *tail = last_tail->next;
      tail->next = cur;      // 当前组的尾节点指向下一组的起始节点
      last_tail->next = pre; // 上一组的尾节点指向当前组的头节点
      last_tail = tail;
    }
    return dummy.next;
  }
};
// 146｜LRU 缓存机制｜中等
class LRUCache {
private:
  int capacity_;
  list<pair<int, int>> l_;
  unordered_map<int, list<pair<int, int>>::iterator> m_;

public:
  LRUCache(int capacity) : capacity_(capacity) {}

  int get(int key) {
    if (m_.contains(key)) {
      auto it = m_[key];
      l_.splice(l_.begin(), l_, it);
      return m_[key]->second;
    } else {
      return -1;
    }
  }

  void put(int key, int value) {
    if (m_.contains(key)) {
      auto it = m_[key];
      it->second = value;
      l_.splice(l_.begin(), l_, it);
    } else {
      if (l_.size() + 1 > capacity_) {
        auto listend = prev(l_.end());
        m_.erase(listend->first);
        l_.erase(listend);
      }
      l_.emplace_front(key, value);
      m_.emplace(key, l_.begin());
    }
  }
};
} // namespace lesson3

int main() {
  using namespace lesson3;
  return 0;
}