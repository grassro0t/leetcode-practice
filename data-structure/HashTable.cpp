#include <iostream>
#include <vector>

namespace hash_table {
using namespace std;
struct HashNode {
  int key;
  int val;
  HashNode* next;
  HashNode(int k, int v) : key(k), val(v), next(nullptr) {}
};
class HashTable {
 private:
  vector<HashNode*> buckets_;
  size_t bucket_size_;
  size_t element_count_;
  static constexpr double kMaxLoadFactor = 0.75;
  size_t hashFunc(int key, int bucket_size) const {
    return static_cast<size_t>(key % bucket_size);
  }

 public:
  HashTable() : bucket_size_(8), element_count_(0) {
    buckets_.resize(bucket_size_, nullptr);
  }
  ~HashTable() {
    for (auto bucket : buckets_) {
      while (bucket != nullptr) {
        HashNode* next = bucket->next;
        delete bucket;
        bucket = next;
      }
    }
  }
  void rehash() {
    size_t new_bucket_size = bucket_size_ * 2;
    vector<HashNode*> new_buckets(new_bucket_size, nullptr);
    for (auto cur : buckets_) {
      while (cur != nullptr) {
        HashNode* next = cur->next;
        size_t new_index = hashFunc(cur->key, new_bucket_size);
        cur->next = new_buckets[new_index];
        new_buckets[new_index] = cur;
        cur = next;
      }
    }
    buckets_ = std::move(new_buckets);
    bucket_size_ = new_bucket_size;
  }
  HashNode* get_node(int key) const {
    size_t index = hashFunc(key, bucket_size_);
    HashNode* cur = buckets_[index];
    while (cur != nullptr) {
      if (cur->key == key) {
        return cur;
      }
      cur = cur->next;
    }
    return nullptr;
  }
  bool get(int key, int& val) const {
    HashNode* cur = get_node(key);
    if (cur != nullptr) {
      val = cur->val;
      return true;
    }
    return false;
  }
  void put(int key, int val) {
    HashNode* cur = get_node(key);
    if (cur != nullptr) {
      cur->val = val;
      return;
    }
    size_t index = hashFunc(key, bucket_size_);
    HashNode* new_node = new HashNode(key, val);
    new_node->next = buckets_[index];
    buckets_[index] = new_node;
    ++element_count_;

    double load_factor = static_cast<double>(element_count_ / bucket_size_);
    if (load_factor > kMaxLoadFactor) {
      rehash();
    }
  }
  bool remove(int key) {
    size_t index = hashFunc(key, bucket_size_);
    HashNode* cur = buckets_[index];
    HashNode* prev = nullptr;
    while (cur != nullptr) {
      if (cur->key == key) {
        if (prev == nullptr) {
          buckets_[index] = cur->next;
        } else {
          prev->next = cur->next;
        }
        delete cur;
        --element_count_;
        return true;
      }
      prev = cur;
      cur = cur->next;
    }
    return false;
  }
  size_t size() const { return element_count_; }
};
}  // namespace hash_table
int main() {
  hash_table::HashTable hashtbl;
  std::vector<int> source_vec{1, 2, 3};
  for (auto& item : source_vec) {
    hashtbl.put(item, item + 1);
  }
  for (size_t i = 0; i < hashtbl.size(); i++) {
    int val = 0;
    hashtbl.get(source_vec[i], val);
    std::cout << "key: " << source_vec[i] << "val: " << val << std::endl;
  }
  hashtbl.put(2, 5);
  hashtbl.remove(3);
  for (size_t i = 0; i < hashtbl.size(); i++) {
    int val = 0;
    hashtbl.get(source_vec[i], val);
    std::cout << "key: " << source_vec[i] << "val: " << val << std::endl;
  }
}
