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
    int rand_id = l + rand() % (r - l + 1);
    swap(arr[rand_id], arr[r]);
    int middle = Partition(arr, l, r);
    QuickSort(arr, l, middle - 1);
    QuickSort(arr, middle + 1, r);
  }
  int Partition(vector<int> &arr, int l, int r) {
    int val = arr[r];
    int i = l;
    int j = r - 1;
    while (i <= j) {
      while (i <= j && arr[i] < val)
        i++;
      while (i <= j && arr[j] > val)
        j--;
      if (i <= j) {
        swap(arr[i], arr[j]);
        i++;
        j--;
      }
    }
    swap(arr[i], arr[r]);
    return i;
  }
  // 归并排序
  void MergeSort(vector<int> &arr, int l, int r) {
    if (l >= r)
      return;
    int mid = l + (r - l) / 2;
    MergeSort(arr, l, mid);
    MergeSort(arr, mid + 1, r);
    vector<int> temp;
    int i = l;
    int j = mid + 1;
    while (i <= mid && j <= r) {
      if (arr[i] < arr[j])
        temp.emplace_back(arr[i++]);
      else
        temp.emplace_back(arr[j++]);
    }
    while (i <= mid)
      temp.emplace_back(arr[i++]);
    while (j <= r)
      temp.emplace_back(arr[j++]);
    for (int k = 0; k < temp.size(); k++)
      arr[l + k] = temp[k];
  }
  // 堆排序
  void HeapSort(vector<int> &arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--)
      Heapify(arr, n, i);
    for (int i = n - 1; i >= 0; i--) {
      swap(arr[0], arr[i]);
      Heapify(arr, i, 0);
    }
  }
  void Heapify(vector<int> &arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest])
      largest = l;
    if (r < n && arr[r] > arr[largest])
      largest = r;
    if (largest != i) {
      swap(arr[i], arr[largest]);
      Heapify(arr, n, largest);
    }
  }
  // 冒泡排序
  void bubbleSort(std::vector<int> &arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
      for (int j = 0; j < n - i - 1; ++j) {
        if (arr[j] > arr[j + 1]) {
          std::swap(arr[j], arr[j + 1]);
        }
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