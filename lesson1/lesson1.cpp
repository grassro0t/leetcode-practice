#include <iostream>
#include <string>
#include <vector>

namespace lesson1 {
using namespace std;
class Solution {
public:
  // 283｜移动零｜简单
  void moveZeroes(vector<int> &nums) {
    if (nums.size() == 0 || nums.size() == 1)
      return;
    auto slow = nums.begin();
    auto fast = slow;
    while (fast != nums.end()) {
      if (*fast != 0) {
        *slow = *fast;
        ++fast;
        ++slow;
      } else {
        ++fast;
      }
    }
    while (slow != nums.end()) {
      *slow = 0;
      ++slow;
    }
  }
  // 169｜多数元素｜简单
  int majorityElement(vector<int> &nums) {
    unordered_map<int, int> num_map;
    for (auto &&num : nums) {
      ++num_map[num];
    }
    int max = -1;
    int max_key = -1;
    for (auto &&item : num_map) {
      int val = item.second;
      if (val > max) {
        max = val;
        max_key = item.first;
      }
    }
    return max_key;
  }
  // 11｜盛最多水的容器｜中等
  int maxArea(vector<int> &height) {
    int l = 0;
    int r = height.size() - 1;
    int water = -1;
    while (l < r) {
      water = max(water, (r - l) * min(height[l], height[r]));
      if (height[l] > height[r])
        r--;
      else
        l++;
    }
    return water;
  }
  // 15｜三数之和｜中等
  vector<vector<int>> threeSum(vector<int> &nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    int n = nums.size();
    for (int i = 0; i < n - 2; i++) {
      if (i - 1 >= 0 && nums[i - 1] == nums[i])
        continue;
      int j = i + 1;
      int k = n - 1;
      while (j < k) {
        int s = nums[i] + nums[j] + nums[k];
        if (s == 0) {
          res.emplace_back(vector<int>{nums[i], nums[j], nums[k]});
          j++;
          k--;
          while (j < k && nums[j - 1] == nums[j])
            j++;
          while (j < k && nums[k + 1] == nums[k])
            k--;
        } else if (s < 0) {
          j++;
        } else {
          k--;
        }
      }
    }
    return res;
  }
  // 75｜颜色分类｜中等
  void sortColors(vector<int> &nums) {
    int less = 0;
    int equal = 0;
    int more = nums.size() - 1;
    while (equal <= more) {
      int val = nums[equal];
      if (val == 0) {
        swap(nums[less], nums[equal]);
        less++;
        equal++;
      } else if (val == 1) {
        equal++;
      } else {
        swap(nums[equal], nums[more]);
        more--;
      }
    }
  }
  // 56｜合并区间｜中等
  vector<vector<int>> merge(vector<vector<int>> &intervals) {
    vector<vector<int>> res;
    sort(intervals.begin(), intervals.end());
    int left = -1;
    int right = -1;
    bool flag = false;
    for (auto &&item : intervals) {
      if (item[0] <= right) {
        right = max(right, item[1]);
      } else {
        res.emplace_back(vector<int>{left, right});
        left = item[0];
        right = item[1];
      }
    }
    res.erase(res.begin(), res.begin() + 1);
    res.emplace_back(vector<int>{left, right});
    return res;
  }
  // 189｜旋转数组｜中等
  void rotate(vector<int> &nums, int k) {
    k %= nums.size();
    ranges::reverse(nums);
    ranges::reverse(nums.begin(), nums.begin() + k);
    ranges::reverse(nums.begin() + k, nums.end());
  }
  // 238｜除自身以外数组的乘积｜中等
  vector<int> productExceptSelf(vector<int> &nums) {
    int len = nums.size();
    if (len == 0)
      return {};
    vector<int> ans(len, 1);
    ans[0] = 1;
    int tmp = 1;
    for (int i = 1; i < len; i++) {
      ans[i] = ans[i - 1] * nums[i - 1];
    }
    for (int i = len - 2; i >= 0; i--) {
      tmp *= nums[i + 1];
      ans[i] *= tmp;
    }
    return ans;
  }
  // 53｜最大子数组和｜中等
  int maxSubArray(vector<int> &nums) {
    int temp = nums[0];
    int max_val = temp;
    for (int i = 0; i < nums.size(); i++) {
      if (i - 1 >= 0)
        temp = max(temp + nums[i], nums[i]);
      max_val = max(max_val, temp);
    }
    return max_val;
  }
  // 42｜接雨水｜困难
  int trap(vector<int> &height) {
    int left_max = -1;
    int right_max = -1;
    int l = 0;
    int r = height.size() - 1;
    int water = 0;
    while (l < r) {
      if (height[l] < height[r]) {
        left_max = max(left_max, height[l]);
        water += left_max - height[l];
        l++;
      } else {
        right_max = max(right_max, height[r]);
        water += right_max - height[r];
        r--;
      }
    }
    return water;
  }
  // 41｜缺失的第一个正数｜困难
  int firstMissingPositive(vector<int> &nums) {
    int n = nums.size();
    for (auto &&num : nums) {
      if (num <= 0)
        num = n + 1;
    }
    for (auto &&num : nums) {
      int v = abs(num);
      if (v > 0 && v < n + 1)
        nums[v - 1] = -abs(nums[v - 1]);
    }
    for (int i = 0; i < n; i++) {
      if (nums[i] > 0)
        return i + 1;
    }
    return n + 1;
  }
};
} // namespace lesson1

int main() { return 0; }