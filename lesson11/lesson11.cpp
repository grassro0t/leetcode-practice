#include <iostream>
#include <string>
#include <vector>

namespace lesson11 {
using namespace std;
class Solution {
public:
  // 215｜数组中的第 K 个最大元素｜中等
  int findKthLargest(vector<int> &nums, int k) {
    priority_queue<int> q(nums.begin(), nums.end());
    int res = 0;
    while (k) {
      res = q.top();
      q.pop();
      k--;
    }
    return res;
  }
  // 347｜前 K 个高频元素｜中等
  vector<int> topKFrequent(vector<int> &nums, int k) {
    unordered_map<int, int> um;
    auto cmp = [](pair<int, int> a, pair<int, int> b) -> bool {
      return a.second < b.second;
    };
    for (int i = 0; i < nums.size(); i++) {
      if (um.contains(nums[i]))
        um[nums[i]]++;
      else
        um.emplace(nums[i], 1);
    }
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> q(
        um.begin(), um.end());
    vector<int> res;
    while (k) {
      res.emplace_back(q.top().first);
      q.pop();
      k--;
    }
    return res;
  }
};
// 295｜数据流的中位数｜困难
class MedianFinder {
public:
  priority_queue<int, vector<int>, greater<int>> q1;
  priority_queue<int> q2;
  MedianFinder() {}

  void addNum(int num) {
    if (q1.size() == q2.size()) {
      q2.push(num);
      q1.push(q2.top());
      q2.pop();
    } else {
      q1.push(num);
      q2.push(q1.top());
      q1.pop();
    }
  }

  double findMedian() {
    if (q1.size() == q2.size()) {
      return (q1.top() + q2.top()) / 2.0;
    } else {
      return q1.top();
    }
  }
};
} // namespace lesson11

int main() {
  using namespace lesson11;
  return 0;
}