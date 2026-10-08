#include <iostream>
#include <string>
#include <vector>

namespace lesson13 {
using namespace std;
class Solution {
public:
  // 136｜只出现一次的数字｜简单
  int singleNumber(vector<int> &nums) {
    int res = 0;
    for (int i = 0; i < nums.size(); i++) {
      res = res ^ nums[i];
    }
    return res;
  }
  // 31 | 下一个排列 | 简单
  void nextPermutation(vector<int> &nums) {
    int n = nums.size();
    int l = -1;
    int r = l;
    for (int i = n - 1; i >= 0; i--) {
      if (i + 1 < n && nums[i] < nums[i + 1]) {
        l = i;
        break;
      }
    }
    if (l == -1) {
      ranges::reverse(nums);
      return;
    }
    for (int i = n - 1; i > l; i--) {
      if (nums[i] > nums[l]) {
        r = i;
        break;
      }
    }
    swap(nums[l], nums[r]);
    ranges::reverse(nums.begin() + l + 1, nums.end());
  }
  // 287 | 寻找重复数 | 简单
  int findDuplicate(vector<int> &nums) {
    int fast = 0;
    int slow = 0;
    while (true) {
      slow = nums[slow];
      fast = nums[nums[fast]];
      if (fast == slow)
        break;
    }
    int cur = 0;
    while (fast != cur) {
      fast = nums[fast];
      cur = nums[cur];
    }
    return cur;
  }
  // 268｜丢失的数字｜简单
  int missingNumber(vector<int> &nums) {
    int n = nums.size();
    for (int i = 0; i < n; i++) {
      if (nums[i] == 0)
        nums[i] = n + 1;
    }
    for (int i = 0; i < n; i++) {
      int idx = abs(nums[i]);
      if (idx == n + 1)
        continue;
      nums[idx - 1] = -abs(nums[idx - 1]);
    }
    for (int i = 0; i < n; i++) {
      if (nums[i] > 0) {
        return i + 1;
      }
    }
    return 0;
  }
  // 9｜回文数｜简单
  bool isPalindrome(int x) {
    if (x < 0)
      return false;
    int temp = x;
    long long y = 0;
    while (x) {
      y = 10 * y + x % 10;
      x /= 10;
    }
    return temp == y ? true : false;
  }
  // 69｜x 的平方根｜简单
  int mySqrt(int x) {
    long long res = 0;
    for (int i = 0; i <= x / 2; i++) {
      if (res * res <= x && (res + 1) * (res + 1) > x)
        return res;
      res++;
    }
    return res;
  }
  // 137｜只出现一次的数字 II｜中等
  int singleNumber(vector<int> &nums) {
    int res = 0;
    for (int i = 0; i < 32; i++) {
      int sum_val = 0;
      for (auto &&num : nums) {
        sum_val += num >> i & 1;
      }
      res |= sum_val % 3 << i;
    }
    return res;
  }
  int singleNumber2(vector<int> &nums) {
    int ones, twos = 0;
    for (auto &&num : nums) {
      ones = ones ^ num & ~twos;
      twos = twos ^ num & ~ones;
    }
    return ones;
  }
  // 172｜阶乘后的零｜中等
  int trailingZeroes(int n) {
    int res = 0;
    while (n) {
      n /= 5;
      res += n;
    }
    return res;
  }
};
} // namespace lesson13

int main() {
  using namespace lesson13;
  return 0;
}