#include <iostream>
#include <vector>

namespace lesson6 {
using namespace std;
class Solution {
public:
  // 35｜搜索插入位置｜简单
  int searchInsert(vector<int> &nums, int target) {
    int left = 0;
    int right = nums.size();
    while (left < right) {
      int mid = left + (right - left) / 2;
      if (nums[mid] < target) {
        left = mid + 1;
      } else {
        right = mid;
      }
    }
    return left;
  }
  // 34｜在排序数组中查找元素的第一个和最后一个位置｜中等
  vector<int> searchRange(vector<int> &nums, int target) {
    int idx = lowerBound(nums, target);
    if (idx >= nums.size() || nums[idx] != target) {
      return vector<int>{-1, -1};
    }
    int nxt_idx = lowerBound(nums, target + 1) - 1;
    return vector<int>{idx, nxt_idx};
  }

  int lowerBound(vector<int> &nums, int target) {
    int l = 0;
    int r = nums.size();
    while (l < r) {
      int mid = l + (r - l) / 2;
      if (nums[mid] < target) {
        l = mid + 1;
      } else {
        r = mid;
      }
    }
    return l;
  }
  // 74｜搜索二维矩阵｜中等
  bool searchMatrix(vector<vector<int>> &matrix, int target) {
    int m = matrix.size();
    if (m == 0)
      return false;
    int n = matrix[0].size();
    int l = 0;
    int r = m * n;
    while (l < r) {
      int mid = l + (r - l) / 2;
      if (matrix[mid / n][mid % n] < target)
        l = mid + 1;
      else
        r = mid;
    }
    return l < m * n && matrix[l / n][l % n] == target ? true : false;
  }
  // 153｜寻找旋转排序数组中的最小值｜简单
  int findMin(vector<int> &nums) {
    int l = 0;
    int r = nums.size() - 1;
    while (l < r) {
      int mid = l + (r - l) / 2;
      if (nums[mid] > nums[r]) {
        l = mid + 1;
      } else {
        r = mid;
      }
    }
    return nums[l];
  }
  // 33｜搜索旋转排序数组｜中等
  int search(vector<int> &nums, int target) {
    int l = 0;
    int r = nums.size() - 1;
    int n = nums.size();
    while (l < r) {
      int mid = l + (r - l) / 2;
      if (nums[mid] <= nums[r])
        r = mid;
      else
        l = mid + 1;
    }
    int offset = l;
    l = 0;
    r = nums.size();
    while (l < r) {
      int mid = l + (r - l) / 2;
      if (nums[(mid + offset) % n] < target)
        l = mid + 1;
      else
        r = mid;
    }
    return l < nums.size() && nums[(l + offset) % n] == target
               ? (l + offset) % n
               : -1;
  }
};
} // namespace lesson6

int main() {
  using namespace lesson6;
  return 0;
}