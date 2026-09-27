#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace lesson5 {
using namespace std;
class Solution {
public:
  // 20｜有效的括号｜简单
  bool isValid(string s) {
    vector<char> st;

    for (auto &&c : s) {
      switch (c) {
      case '(':
      case '[':
      case '{':
        st.emplace_back(c);
        break;
      case ')': {
        if (st.empty())
          return false;
        char tail = st.back();
        if (tail == '(')
          st.pop_back();
        else
          return false;
        break;
      }
      case ']': {
        if (st.empty())
          return false;
        char tail = st.back();
        if (tail == '[')
          st.pop_back();
        else
          return false;
        break;
      }
      case '}': {
        if (st.empty())
          return false;
        char tail = st.back();
        if (tail == '{')
          st.pop_back();
        else
          return false;
        break;
      }
      default:
        break;
      }
    }
    return st.empty();
  }
  // 3｜无重复字符的最长子串｜中等
  int lengthOfLongestSubstring(string s) {
    unordered_set<char> us;
    int res = 0;
    int left = 0;
    int right = 0;
    for (; right < s.size(); right++) {
      while (us.contains(s[right])) {
        us.erase(s[left]);
        left++;
      }
      us.emplace(s[right]);
      res = max(res, right - left + 1);
    }
    return res;
  }
  // 5｜最长回文子串｜中等
  string longestPalindrome(string s) {
    int max_left = 0;
    int max_right = 0;
    int max_len = 0;
    int n = s.size();
    for (int i = 0; i < n; i++) {
      int l = i - 1;
      int r = i + 1;
      while (l >= 0 && r <= n - 1 && s[l] == s[r]) {
        l--;
        r++;
      }
      if (r - l - 1 > max_len) {
        max_left = l + 1;
        max_right = r - 1;
        max_len = r - l - 1;
      }
    }
    for (int i = 0; i < n; i++) {
      int l = i;
      int r = i + 1;
      while (l >= 0 && r <= n - 1 && s[l] == s[r]) {
        l--;
        r++;
      }
      if (r - l - 1 > max_len) {
        max_left = l + 1;
        max_right = r - 1;
        max_len = r - l - 1;
      }
    }
    return s.substr(max_left, max_right - max_left + 1);
  }
  // 438｜找到字符串中所有字母异位词｜中等
  vector<int> findAnagrams(string s, string p) {
    int n = p.size();
    array<int, 26> arr_p{};
    for (int i = 0; i < p.size(); i++) {
      arr_p[p[i] - 'a']++;
    }
    array<int, 26> arr_s{};
    vector<int> res;
    for (int right = 0; right < s.size(); right++) {
      arr_s[s[right] - 'a']++;
      int left = right - n + 1;
      if (left < 0)
        continue;
      if (arr_s == arr_p)
        res.emplace_back(left);
      arr_s[s[left] - 'a']--;
    }
    return res;
  }
  // 394｜字符串解码｜中等
  string decodeString(string s) {
    stack<int> nums;
    stack<string> strs;
    int num = 0;
    string res = "";
    for (int i = 0; i < s.size(); i++) {
      if (s[i] >= 'a' && s[i] <= 'z') {
        res += s[i];
      } else if (s[i] >= '0' && s[i] <= '9') {
        num = num * 10 + (s[i] - '0');
      } else if (s[i] == '[') {
        nums.push(num);
        strs.push(res);
        num = 0;
        res = "";
      } else if (s[i] == ']') {
        int k = nums.top();
        nums.pop();
        string temp = strs.empty() ? "" : strs.top();
        for (int j = 0; j < k; j++) {
          temp += res;
        }
        res = temp;
        strs.pop();
      } else {
      }
    }
    return res;
  }
  // 76｜最小窗口子串｜困难
  string minWindow(string s, string t) {
    array<int, 52> arr, arr_t;
    auto getIdx = [](char c) -> int {
      if (isupper(c))
        return c - 'A';
      else
        return 26 + (c - 'a');
    };
    for (auto &&tt : t) {
      arr_t[getIdx(tt)]++;
    }
    auto isCover = [&arr, &arr_t]() -> bool {
      for (int i = 0; i < 52; i++) {
        if (arr[i] < arr_t[i])
          return false;
      }
      return true;
    };
    int l = 0;
    int r = 0;
    int cover_l = 0;
    int cover_r = 0;
    int min_len = s.size() + 1;
    while (r < s.size()) {
      arr[getIdx(s[r])]++;
      while (isCover()) {
        if (r - l + 1 < min_len) {
          cover_l = l;
          cover_r = r;
          min_len = r - l + 1;
        }
        arr[getIdx(s[l])]--;
        l++;
      }
      r++;
    }
    string res =
        min_len == s.size() + 1 ? "" : s.substr(cover_l, cover_r - cover_l + 1);
    return res;
  }
};
} // namespace lesson5

int main() {
  using namespace lesson5;
  return 0;
}