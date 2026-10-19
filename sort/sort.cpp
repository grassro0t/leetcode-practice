#include <iostream>
#include <vector>

namespace sort {
using namespace std;
// 912｜排序数组｜中等
class Solution {
public:
  vector<int> sortArray(vector<int> &nums) {
    QuickSort(nums, 0, nums.size() - 1);
    return nums;
  }
  // 快速排序
  void QuickSort(vector<int> &arr, int l, int r) {
    if (l >= r)
      return;
    int mid = Partition(arr, l, r);
    QuickSort(arr, l, mid - 1);
    QuickSort(arr, mid + 1, r);
  }

  int Partition(vector<int> &arr, int l, int r) {
    swap(arr[l + rand() % (r - l + 1)], arr[r]);
    int p = arr[r];
    int i = l;
    int j = r - 1;
    while (i <= j) {
      while (i <= j && arr[i] <= p)
        i++;
      while (i <= j && arr[j] >= p)
        j--;
      if (i <= j)
        swap(arr[i], arr[j]);
    }
    swap(arr[i], arr[r]);
    return i;
  }
  // 归并排序
  void MergeSort(vector<int> &arr, vector<int> &temp, int l, int r) {
    if (l >= r)
      return;
    int mid = l + (r - l) / 2;
    MergeSort(arr, temp, l, mid - 1);
    MergeSort(arr, temp, mid, r);
    Merge(arr, temp, l, r, mid);
  }

  void Merge(vector<int> &arr, vector<int> &temp, int l, int r, int mid) {
    int i = l;
    int j = mid;
    int k = l;
    while (i <= mid - 1 && j <= r) {
      if (arr[i] <= arr[j])
        temp[k++] = arr[i++];
      else
        temp[k++] = arr[j++];
    }
    while (i <= mid - 1)
      temp[k++] = arr[i++];
    while (j <= r)
      temp[k++] = arr[j++];
    for (int p = l; p <= r; p++)
      arr[p] = temp[p];
  }
  // 堆排序
  void HeapSort(vector<int> &arr) {
    priority_queue<int> pq;
    int n = arr.size();
    for (int i = 0; i < n; i++) {
      pq.push(arr[i]);
    }
    int idx = n - 1;
    while (!pq.empty()) {
      arr[idx--] = pq.top();
      pq.pop();
    }
  }
  // 冒泡排序
  void BubbleSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n - 1 - i; j++) {
        if (arr[j + 1] < arr[j])
          swap(arr[j + 1], arr[j]);
      }
    }
  }
  // 选择排序
  void selectSort(std::vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
      int minIdx = i; // 最小值下标，初始为未排序起点
      // 遍历未排序区间找最小
      for (int j = i + 1; j < n; ++j) {
        if (arr[j] < arr[minIdx]) {
          minIdx = j;
        }
      }
      // 交换最小元素到未排序头部
      std::swap(arr[i], arr[minIdx]);
    }
  }
  // 插入排序
  void insertSort(std::vector<int> &arr) {
    int n = arr.size();
    // 从第二个元素开始，第一个默认有序
    for (int i = 1; i < n; ++i) {
      int val = arr[i]; // 待插入的值
      int j = i - 1;
      // 向前移动：大于val的元素后移一位
      while (j >= 0 && arr[j] > val) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = val; // 插入到空位
    }
  }
  // 基数排序
  void radixSort(std::vector<int> &arr) {
    if (arr.empty())
      return;
    // 找最大值，确定最高位数
    int maxVal = arr[0];
    for (auto v : arr)
      if (v > maxVal)
        maxVal = v;

    // 依次按个位、十位、百位...处理
    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
      countSort(arr, exp);
    }
  }
  void countSort(std::vector<int> &arr, int exp) {
    int n = arr.size();
    std::vector<int> output(n);
    std::vector<int> cnt(10, 0); // 0~9

    // 统计当前位数字出现次数
    for (int i = 0; i < n; i++)
      cnt[(arr[i] / exp) % 10]++;

    // 前缀和，计算位置（保证稳定）
    for (int i = 1; i < 10; i++)
      cnt[i] += cnt[i - 1];

    // 倒序填充，维持稳定性
    for (int i = n - 1; i >= 0; i--) {
      int digit = (arr[i] / exp) % 10;
      output[cnt[digit] - 1] = arr[i];
      cnt[digit]--;
    }
    arr.swap(output);
  }
};
} // namespace sort

int main() {
  using namespace sort;
  return 0;
}