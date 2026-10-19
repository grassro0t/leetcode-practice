#include <iostream>
#include <vector>

namespace double_linked_list {
using namespace std;
struct DListNode {
  int val;
  DListNode* prev;
  DListNode* next;
  DListNode(int v) : val(v), prev(nullptr), next(nullptr) {}
};
class DoubleLinkedList {
 private:
  DListNode* dummy_{nullptr};
  DListNode* head_{nullptr};
  DListNode* tail_{nullptr};

 public:
  DoubleLinkedList() { dummy_ = new DListNode(-1); }
  ~DoubleLinkedList() {
    clear();
    delete dummy_;
    dummy_ = nullptr;
  }
  void clear() {
    DListNode* cur = dummy_->next;
    while (cur != nullptr) {
      DListNode* nxt = cur->next;
      delete cur;
      cur = nxt;
    }
    dummy_->next = nullptr;
    head_ = nullptr;
    tail_ = nullptr;
  }
  DoubleLinkedList(const DoubleLinkedList&) = delete;
  DoubleLinkedList& operator=(const DoubleLinkedList&) = delete;
  DoubleLinkedList(DoubleLinkedList&&) noexcept = delete;
  DoubleLinkedList& operator=(DoubleLinkedList&&) noexcept = delete;
  bool empty() const { return dummy_->next == nullptr; }
  void insertAfter(DListNode* cur, int val) {
    if (cur == nullptr) {
      return;
    }
    DListNode* new_node = new DListNode(val);
    new_node->prev = cur;
    new_node->next = cur->next;
    if (cur->next != nullptr) {
      cur->next->prev = new_node;
    }
    cur->next = new_node;
  }
  void pushFront(int val) {
    insertAfter(dummy_, val);
    head_ = dummy_->next;
    if (tail_ == nullptr) {
      tail_ = head_;
    }
  }
  void pushBack(int val) {
    if (tail_ == nullptr) {
      pushFront(val);
      return;
    }
    insertAfter(tail_, val);
    tail_ = tail_->next;
  }
  void eraseNode(DListNode* del) {
    if (del == nullptr) {
      return;
    }
    DListNode* pre = del->prev;
    DListNode* nxt = del->next;
    if (pre != nullptr) {
      pre->next = nxt;
    }
    if (nxt != nullptr) {
      nxt->prev = pre;
    }
    if (del == head_) {
      head_ = nxt;
    }
    if (del == tail_) {
      tail_ = pre;
    }
    // 删成空链表时 head_/tail_ 需同时复位，避免 tail_ 指向 dummy_
    if (head_ == nullptr) {
      tail_ = nullptr;
    }
    delete del;
  }
  void popFront() {
    if (head_ == nullptr) {
      return;
    }
    eraseNode(head_);
  }
  void popBack() {
    if (tail_ == nullptr) {
      return;
    }
    eraseNode(tail_);
  }
  void eraseByVal(int val) {
    auto p = findByVal(val);
    if (p != nullptr) {
      eraseNode(p);
    }
  }
  DListNode* findByVal(int target) {
    auto cur = head_;
    while (cur != nullptr) {
      if (cur->val == target) {
        return cur;
      }
      cur = cur->next;
    }
    return nullptr;
  }
  DListNode& operator[](const size_t& index) {
    auto cur = head_;
    size_t count = 0;
    while (cur != nullptr) {
      if (count == index) {
        return *cur;
      }
      cur = cur->next;
      ++count;
    }
    throw std::out_of_range("Index out of range");
  }
  size_t size() const {
    size_t n = 0;
    DListNode* cur = dummy_->next;
    while (cur != nullptr) {
      ++n;
      cur = cur->next;
    }
    return n;
  }
  bool update(int old_val, int new_val) {
    auto node = findByVal(old_val);
    if (node != nullptr) {
      node->val = new_val;
      return true;
    }
    return false;
  }
};
}  // namespace double_linked_list
int main() {
  double_linked_list::DoubleLinkedList dlist;
  dlist.pushBack(10);
  dlist.pushFront(8);
  auto dnode = dlist.findByVal(8);
  dlist.insertAfter(dnode, 9);
  // 遍历打印全部节点，避免 erase 后索引越界导致 out_of_range 异常
  auto print = [&dlist]() {
    std::cout << "dlist is :";
    const size_t len = dlist.size();
    for (size_t i = 0; i < len; ++i) {
      if (i > 0) {
        std::cout << " ";
      }
      std::cout << dlist[i].val;
    }
    std::cout << std::endl;
  };
  print();
  dlist.update(8, 7);
  print();
  dlist.eraseByVal(9);
  print();
}