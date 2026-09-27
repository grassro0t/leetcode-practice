#include <iostream>
#include <vector>

namespace lesson7 {
using namespace std;
class Solution {
public:
  // 20｜有效的括号｜简单
  bool isValid(string s) {
    stack<char> st;
    for (auto &&c : s) {
      if (c == ')') {
        if (st.empty())
          return false;
        if (st.top() == '(')
          st.pop();
        else
          return false;
      } else if (c == ']') {
        if (st.empty())
          return false;
        if (st.top() == '[')
          st.pop();
        else
          return false;
      } else if (c == '}') {
        if (st.empty())
          return false;
        if (st.top() == '{')
          st.pop();
        else
          return false;
      } else {
        st.push(c);
      }
    }
    return st.empty() ? true : false;
  }
  // 739｜每日温度｜中等
  vector<int> dailyTemperatures(vector<int> &temperatures) {
    stack<int> less_st;
    int n = temperatures.size();
    vector<int> res(n, 0);
    for (int i = 0; i < n; i++) {
      while (!less_st.empty() &&
             temperatures[i] > temperatures[less_st.top()]) {
        res[less_st.top()] = i - less_st.top();
        less_st.pop();
      }
      less_st.push(i);
    }
    return res;
  }
  // 84｜柱状图中最大的矩形｜困难
  int largestRectangleArea(vector<int> &heights) {
    int max_val = 0;
    stack<int> st;
    for (int i = 0; i < heights.size(); i++) {
      while (!st.empty() && heights[i] < heights[st.top()]) {
        int r = i;
        int h = heights[st.top()];
        st.pop();
        int l = st.empty() ? -1 : st.top();
        int w = r - l - 1;
        max_val = max(max_val, h * w);
      }
      st.push(i);
    }
    int n = heights.size();
    while (!st.empty()) {
      int h = heights[st.top()];
      st.pop();
      int l = st.empty() ? -1 : st.top();
      int w = n - l - 1;
      max_val = max(max_val, h * w);
    }
    return max_val;
  }
};
// 155｜最小栈｜简单
class MinStack {
private:
  stack<int> st_;
  stack<int> min_st_;

public:
  MinStack() {}

  void push(int value) {
    st_.push(value);
    if (min_st_.empty()) {
      min_st_.push(value);
    } else {
      if (value <= min_st_.top()) {
        min_st_.push(value);
      }
    }
  }

  void pop() {
    int val = st_.top();
    st_.pop();
    if (val == min_st_.top()) {
      min_st_.pop();
    }
  }

  int top() { return st_.top(); }

  int getMin() { return min_st_.top(); }
};
} // namespace lesson7

int main() {
  using namespace lesson7;
  return 0;
}