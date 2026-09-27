#include <iostream>
#include <vector>

namespace sort {
using namespace std;
// 912｜排序数组｜中等
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        QuickSort(nums,0,nums.size()-1);
        return nums;
    }
    void QuickSort(vector<int>& arr,int l,int r){
        if(l>=r) return;
        int rand_id = l+rand()%(r-l+1);
        swap(arr[rand_id],arr[r]);
        int middle = Partition(arr,l,r);
        QuickSort(arr,l,middle-1);
        QuickSort(arr,middle+1,r);
    }
    int Partition(vector<int> &arr, int l, int r){
        int val = arr[r];
        int i = l;
        int j = r;
        while(i<j){
            while(i<j&&arr[i]<=val) i++;
            while(i<j&&arr[j]>=val) j--;
            if(i<j) swap(arr[i],arr[j]);
        }
        swap(arr[i],arr[r]);
        return i;
    }
};
} // namespace sort

int main() {
  using namespace sort;
  return 0;
}