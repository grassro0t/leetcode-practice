#include <iostream>
#include <string>
#include <vector>

namespace lesson10 {
using namespace std;
class Solution {
public:
  // 122｜买卖股票的最佳时机 II｜中等
  int maxProfit(vector<int> &prices) {
    int res = 0;
    for (int i = 1; i < prices.size(); i++) {
      int temp = prices[i] - prices[i - 1];
      if (temp > 0)
        res += temp;
    }
    return res;
  }
  // 55｜跳跃游戏｜中等
  bool canJump(vector<int> &nums) {
    int r = 0;
    for (int i = 0; i < nums.size(); i++) {
      if (i > r)
        return false;
      r = max(r, i + nums[i]);
    }
    return true;
  }
  // 45｜跳跃游戏 II｜中等
  int jump(vector<int> &nums) {
    int r = 0;
    int end = 0;
    int res = 0;
    for (int i = 0; i < nums.size() - 1; i++) {
      r = max(r, nums[i] + i);
      if (i == end) {
        end = r;
        res++;
      }
    }
    return res;
  }
  // 763｜划分字母区间｜中等
  vector<int> partitionLabels(string s) {
    unordered_map<char, int> rborder;
    for (int i = 0; i < s.size(); i++) {
      rborder[s[i]] = i;
    }
    int l = 0;
    int mx = 0;
    vector<int> res;
    for (int i = 0; i < s.size(); i++) {
      int tempr = rborder[s[i]];
      mx = max(mx, rborder[s[i]]);
      if (i == mx) {
        res.emplace_back(mx - l + 1);
        l = i + 1;
      }
    }
    return res;
  }
};
} // namespace lesson10

int main() {
  using namespace lesson10;
  return 0;
}