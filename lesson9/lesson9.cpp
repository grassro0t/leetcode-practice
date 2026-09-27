#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace lesson9 {
using namespace std;
class Solution {
public:
  // 70｜爬楼梯｜简单
  int climbStairs(int n) {
    if (n <= 1)
      return n;
    int temp0 = 1;
    int temp1 = 1;
    for (int i = 2; i <= n; i++) {
      int temp = temp1;
      temp1 = temp0 + temp1;
      temp0 = temp;
    }
    return temp1;
  }
  // 121｜买卖股票的最佳时机｜简单
  int maxProfit(vector<int> &prices) {
    vector<int> dp(prices.size(), 0);
    int cost = prices[0];
    for (int i = 1; i < prices.size(); i++) {
      dp[i] = max(dp[i - 1], prices[i] - cost);
      cost = min(cost, prices[i]);
    }
    return dp.back();
  }
  // 118｜杨辉三角｜简单
  vector<vector<int>> generate(int numRows) {
    vector<int> dp(numRows, 0);
    vector<vector<int>> res;
    for (int i = 0; i < numRows; i++) {
      int pre = 0;
      for (int j = 0; j <= i; j++) {
        if (j == 0 || j == i) {
          dp[j] = 1;
          pre = 1;
        } else {
          int temp = dp[j];
          dp[j] = dp[j] + pre;
          pre = temp;
        }
      }
      res.emplace_back(vector<int>(dp.begin(), dp.begin() + i + 1));
    }
    return res;
  }
  // 198｜打家劫舍｜中等
  int rob(vector<int> &nums) {
    int n = nums.size();
    if (n <= 2)
      return *max_element(nums.begin(), nums.end());
    vector<int> dp(n, 0);
    dp[0] = nums[0];
    dp[1] = max(nums[0], nums[1]);
    for (int i = 2; i < n; i++) {
      dp[i] = max(dp[i - 2] + nums[i], dp[i - 1]);
    }
    return max(dp[n - 1], dp[n - 2]);
  }
  // 279｜完全平方数｜中等
  int numSquares(int n) {
    vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; i++) {
      int k = 1;
      int min_sum = INT_MAX;
      while (i - k * k >= 0) {
        min_sum = min(min_sum, dp[i - k * k]);
        k++;
      }
      dp[i] = min_sum + 1;
    }
    return dp[n];
  }
  // 322｜零钱兑换｜中等
  int coinChange(vector<int> &coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX / 2);
    dp[0] = 0;
    sort(coins.begin(), coins.end(), greater<int>());
    for (int i = 1; i < amount + 1; i++) {
      for (int j = 0; j < coins.size(); j++) {
        if (i >= coins[j])
          dp[i] = min(dp[i], dp[i - coins[j]] + 1);
      }
    }
    return dp[amount] >= INT_MAX / 2 ? -1 : dp[amount];
  }
  // 139｜单词拆分｜中等
  bool wordBreak(string s, vector<string> &wordDict) {
    unordered_set<string> st(wordDict.begin(), wordDict.end());
    int n = s.size();
    vector<bool> dp(n + 1, false);
    dp[0] = true;
    for (int i = 1; i <= n; i++) {
      for (int j = 0; j < i; j++) {
        if (dp[j] && st.contains(s.substr(j, i - j))) {
          dp[i] = true;
          break;
        }
      }
    }
    return dp[n];
  }
  // 300｜最长递增子序列｜中等
  int lengthOfLIS(vector<int> &nums) {
    vector<int> g;
    for (auto &&num : nums) {
      auto it = ranges::lower_bound(g, num);
      if (it == g.end()) {
        g.emplace_back(num);
      } else {
        *it = num;
      }
    }
    return g.size();
  }
  // 62｜不同路径｜中等
  int uniquePaths(int m, int n) {
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        if (i == 1 || j == 1) {
          dp[i][j] = 1;
          continue;
        }
        dp[i][j] = dp[i][j - 1] + dp[i - 1][j];
      }
    }
    return dp[m][n];
  }
  // 64｜最小路径和｜中等
  int minPathSum(vector<vector<int>> &grid) {
    int m = grid.size();
    int n = grid[0].size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, INT_MAX));
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        if (i == 1 && j == 1) {
          dp[i][j] = grid[i - 1][j - 1];
          continue;
        }
        dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i - 1][j - 1];
      }
    }
    return dp[m][n];
  }
  // 221｜最大正方形｜中等
  int maximalSquare(vector<vector<char>> &matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    vector<vector<int>> dp(m, vector<int>(n, 0));
    int max_val = 0;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (i == 0 || j == 0) {
          dp[i][j] = matrix[i][j] - '0';
        } else {
          dp[i][j] =
              matrix[i][j] == '0'
                  ? 0
                  : min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]}) + 1;
        }
        max_val = max(max_val, dp[i][j]);
      }
    }
    return max_val * max_val;
  }
};
} // namespace lesson9

int main() {
  using namespace lesson9;
  return 0;
}