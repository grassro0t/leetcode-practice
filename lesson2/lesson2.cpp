#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace lesson2 {
using namespace std;
class Solution {
  struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
  };

  // 1｜两数之和｜简单
  vector<int> twoSum(vector<int> &nums, int target) {
    unordered_map<int, int> um;
    vector<int> res;
    for (int j = 0; j < nums.size(); j++) {
      if (um.contains(target - nums[j])) {
        res.emplace_back(um[target - nums[j]]);
        res.emplace_back(j);
      }
      um[nums[j]] = j;
    }
    return res;
  }
  // 49｜字母异位词分组｜中等
  vector<vector<string>> groupAnagrams(vector<string> &strs) {
    unordered_map<string, vector<string>> strs_map;
    vector<vector<string>> res;
    for (int i = 0; i < strs.size(); i++) {
      string temp = strs[i];
      sort(temp.begin(), temp.end());
      if (strs_map.count(temp) == 0) {
        strs_map.emplace(temp, vector<string>{strs[i]});
      } else {
        strs_map[temp].emplace_back(strs[i]);
      }
    }
    for (auto &&s : strs_map) {
      res.push_back(s.second);
    }
    return res;
  }
  // 128｜最长连续序列｜困难
  int longestConsecutive(vector<int> &nums) {
    unordered_set<int> nums_set(nums.begin(), nums.end());
    int max_val = 0;
    for (auto &&x : nums_set) {
      if (nums_set.contains(x - 1))
        continue;
      int y = x + 1;
      while (nums_set.contains(y)) {
        y++;
      }
      max_val = max(max_val, y - x);
    }
    return max_val;
  }
  // 560｜和为K的子数组｜中等
  int subarraySum(vector<int> &nums, int k) {
    vector<int> prev(nums.size() + 1);
    prev[0] = 0;
    for (int i = 1; i < prev.size(); i++) {
      prev[i] = prev[i - 1] + nums[i - 1];
    }
    int sum = 0;
    for (int i = 0; i < prev.size(); i++) {
      for (int j = i + 1; j < prev.size(); j++) {
        if (prev[j] - prev[i] == k)
          sum++;
      }
    }
    return sum;
  }
  int subarraySum2(vector<int> &nums, int k) {
    int n = nums.size();
    vector<int> s(n + 1);
    for (int i = 0; i < n; i++) {
      s[i + 1] = s[i] + nums[i];
    }

    unordered_map<int, int> cnt;
    int ans = 0;
    for (int sj : s) {
      // 注意不要直接 += cnt[sj-k]，如果 sj-k 不存在，会插入 sj-k
      ans += cnt.contains(sj - k) ? cnt[sj - k] : 0;
      cnt[sj]++;
    }
    return ans;
  }
};
} // namespace lesson2

int main() {
  using namespace lesson2;
  return 0;
}