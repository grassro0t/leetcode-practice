#include <iostream>
#include <vector>

namespace rbtree {
using namespace std;
enum Color { BLACK, RED };
struct Node {
  int key;
  Color color;
  Node* left;
  Node* right;
  Node* parent;
  Node(int k)
      : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};
// 哨兵NIL，所有叶子都指向NIL
Node* NIL = nullptr;
class RBTree {
 private:
  Node* root;
  void LeftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    y->parent = x->parent;
    if (y->parent == nullptr)
      root = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;
    y->left = x;
    x->parent = y;
  }

  void RightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;
    y->parent = x->parent;
    if (y->parent == nullptr)
      root = y;
    else if (x == x->parent->right)
      x->parent->right = y;
    else
      x->parent->left = y;
    y->right = x;
    x->parent = y;
  }

  void InsertFixup(Node* z) {
    Node* parent = z->parent;
    while (parent != nullptr && parent->color == RED) {
      Node* grandparent = parent->parent;
      if (grandparent != nullptr && parent == grandparent->left) {
        Node* uncle = grandparent->right;
        // Case1：叔叔红色
        if (uncle != nullptr && uncle->color == RED) {
          parent->color = BLACK;
          uncle->color = BLACK;
          grandparent->color = RED;
          z = grandparent;
          parent = z->parent;
        } else {
          // Case2 LR
          if (z == parent->right) {
            z = parent;
            LeftRotate(z);
            parent = z->parent;
            grandparent = parent->parent;
          }
          // Case3 LL
          parent->color = BLACK;
          grandparent->color = RED;
          RightRotate(grandparent);
        }
      } else if (grandparent != nullptr && parent == grandparent->right) {
        Node* uncle = grandparent->left;
        if (uncle != nullptr && uncle->color == RED) {
          parent->color = BLACK;
          uncle->color = BLACK;
          grandparent->color = RED;
          z = grandparent;
          parent = z->parent;
        } else {
          if (z == parent->left) {
            z = parent;
            RightRotate(z);
            parent = z->parent;
            grandparent = parent->parent;
          }
          parent->color = BLACK;
          grandparent->color = RED;
          LeftRotate(grandparent);
        }
      }
    }
    root->color = BLACK;
  }
  void EraseFixup(Node* z) {
    while (z != root && z->color == BLACK) {
      if (z == z->parent->left) {
        Node* w = z->parent->right;
        // Case1 兄弟红色
        if (w->color == RED) {
          w->color = BLACK;
          z->parent->color = RED;
          LeftRotate(z->parent);
          w = z->parent->right;
        }
        // Case2 兄弟黑，两孩子都黑
        if (w->left->color == BLACK && w->right->color == BLACK) {
          w->color = RED;
          z = z->parent;
        } else {
          if (w->right->color == BLACK) {
            w->left->color = BLACK;
            w->color = RED;
            RightRotate(w);
            w = z->parent->right;
          }
          w->color = z->parent->color;
          z->parent->color = BLACK;
          w->right->color = BLACK;
          LeftRotate(z->parent);
          z = root;
        }
      } else {
        Node* w = z->parent->left;
        if (w->color == RED) {
          w->color = BLACK;
          z->parent->color = RED;
          RightRotate(z->parent);
          w = z->parent->left;
        }
        if (w->right->color == BLACK && w->left->color == BLACK) {
          w->color = RED;
          z = z->parent;
        } else {
          // Case3 内侧红
          if (w->left->color == BLACK) {
            w->right->color = BLACK;
            w->color = RED;
            LeftRotate(w);
            w = z->parent->left;
          }
          // Case4 外侧红
          w->color = z->parent->color;
          z->parent->color = BLACK;
          w->left->color = BLACK;
          RightRotate(z->parent);
          z = root;
        }
      }
    }
    z->color = BLACK;
  }
  // 删除辅助：u节点替换为v
  void transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
  }
  // 找子树最小节点
  Node* minimum(Node* p) {
    while (p->left != NIL) {
      p = p->left;
    }
    return p;
  }
  void inorder(Node* p) {
    if (p != NIL) {
      inorder(p->left);
      cout << p->key << " ";
      inorder(p->right);
    }
  }

 public:
  RBTree() {
    NIL = new Node(-1);
    NIL->color = BLACK;
    NIL->left = NIL;
    NIL->right = NIL;
    NIL->parent = nullptr;
    root = NIL;
  }
  ~RBTree() {
    destroy(root);
    delete NIL;
  }
  void destroy(Node* p) {
    if (p != NIL) {
      destroy(p->left);
      destroy(p->right);
      delete p;
    }
  }

  void insert(int key) {
    Node* z = new Node(key);
    Node* prev = nullptr;
    Node* cur = root;
    while (cur != NIL) {
      prev = cur;
      if (key < cur->key)
        cur = cur->left;
      else
        cur = cur->right;
    }
    z->parent = prev;
    if (prev == nullptr) {
      root = z;
    } else if (key < prev->key)
      prev->left = z;
    else
      prev->right = z;

    z->left = NIL;
    z->right = NIL;
    InsertFixup(z);
  }
  void remove(int key) {
    Node* p = find(key);
    if (p != NIL) {
      // 删除节点的颜色
      Color original_color = p->color;
      Node* x = nullptr;
      if (p->left == NIL) {
        x = p->right;
        transplant(p, p->right);
      } else if (p->right == NIL) {
        x = p->left;
        transplant(p, p->left);
      } else {
        Node* y = minimum(p->right);
        original_color = y->color;
        x = y->right;
        if (y->parent == p) {
          y->right->parent = y;
        } else {
          transplant(y, y->right);
          y->right = p->right;
          y->right->parent = y;
        }
        transplant(p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
      }
      delete p;
      if (original_color == BLACK) {
        EraseFixup(x);
      }
    }
  }
  Node* find(int key) {
    Node* cur = root;
    while (cur != NIL) {
      if (key == cur->key) {
        return cur;
      } else if (key < cur->key) {
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    return NIL;
  }
  void printInorder() {
    inorder(root);
    cout << endl;
  }
};
}  // namespace rbtree
int main() {
  rbtree::RBTree tree;
  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  tree.insert(15);
  std::cout << "Inorder traversal: ";
  tree.printInorder();
  std::cout << "Removing 20..." << std::endl;
  tree.remove(20);
  std::cout << "Inorder traversal after removal: ";
  tree.printInorder();
  if (tree.find(15)) {
    std::cout << "Find 15!"<<std::endl;
  } else {
    std::cout << "Not Find 15!"<<std::endl;
  }
  return 0;
}