#include <bitset>
#include <iostream>
#include <unordered_set>
#include <vector>

namespace bloomfilter {

using namespace std;
template <size_t BIT_SIZE, size_t HASH_SIZE>
class StaticBloomFilter {
 private:
  bitset<BIT_SIZE> bits_;
  size_t hash(const string& key, size_t seed) const {
    std::hash<std::string> h;
    // return different value according to seed
    uint64_t val =
        static_cast<uint64_t>(h(key)) ^ (seed * 0x9e3779b97f4a7c15ULL);
    return val % BIT_SIZE;
  }

 public:
  void insert(const string& key) {
    for (size_t i = 0; i < HASH_SIZE; i++) {
      bits_.set(hash(key, i));
    }
  }
  bool contains(const string& key) const {
    for (size_t i = 0; i < HASH_SIZE; i++) {
      size_t pos = hash(key, i);
      if (!bits_.test(pos)) {
        return false;
      }
    }
    return true;
  }
  void clear() { bits_.reset(); }
};
class DynamicBloomFilter {
 private:
  size_t bit_size_;
  size_t hash_size_;
  vector<bool> filter_;
  size_t hash(const string& key, size_t seed) const {
    std::hash<std::string> h;
    // return different value according to seed
    uint64_t val =
        static_cast<uint64_t>(h(key)) ^ (seed * 0x9e3779b97f4a7c15ULL);
    return val % bit_size_;
  }

 public:
  // 手写带参构造，编译器不再生成默认构造函数
  DynamicBloomFilter(const size_t& bit_size, const size_t& hash_size)
      : bit_size_(bit_size), hash_size_(hash_size), filter_(bit_size_, false) {}
  void insert(const string& key) {
    for (size_t i = 0; i < hash_size_; i++) {
      filter_[hash(key, i)] = true;
    }
  }
  bool contains(const string& key) const {
    for (size_t i = 0; i < hash_size_; i++) {
      size_t pos = hash(key, i);
      if (!filter_.at(pos)) {
        return false;
      }
    }
    return true;
  };
  void clear() { std::fill(filter_.begin(), filter_.end(), false); }
};
}  // namespace bloomfilter
int main(int argc, char** argv) {
  bloomfilter::StaticBloomFilter<16, 2> filter1;
  bloomfilter::DynamicBloomFilter filter2(16, 2);
  std::unordered_set<std::string> value_sets{"apple", "blueberry", "orange"};
  for (auto& item : value_sets) {
    filter1.insert(item);
    filter2.insert(item);
  }
  std::cout << "Is apple in set? filter1:" << std::boolalpha
            << filter1.contains("apple") << std::endl;
  std::cout << "Is apple in set? filter2:" << std::boolalpha
            << filter2.contains("apple") << std::endl;
  std::cout << "Is orange in set? filter1:" << std::boolalpha
            << filter1.contains("orange") << std::endl;
  std::cout << "Is orange in set? filter2:" << std::boolalpha
            << filter2.contains("orange") << std::endl;
  // false‑positive
  std::cout << "Is pear in set? filter1:" << std::boolalpha
            << filter1.contains("pear") << std::endl;
  std::cout << "Is pear in set? filter2:" << std::boolalpha
            << filter2.contains("pear") << std::endl;
  return 0;
}