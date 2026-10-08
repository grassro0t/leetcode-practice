#include <iostream>
#include <string>
#include <vector>

namespace lesson12 {
using namespace std;
class Solution {
public:
  // 200｜岛屿数量｜中等
  int numIslands(vector<vector<char>> &grid) {
    int m = grid.size();
    int n = grid[0].size();
    int res = 0;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == '1') {
          dfs(grid, i, j);
          res++;
        }
      }
    }
    return res;
  }

  void dfs(vector<vector<char>> &grid, int x, int y) {
    if (x >= grid.size() || x < 0 || y >= grid[0].size() || y < 0)
      return;
    if (grid[x][y] != '1')
      return;
    grid[x][y] = '2';
    dfs(grid, x + 1, y);
    dfs(grid, x, y + 1);
    dfs(grid, x - 1, y);
    dfs(grid, x, y - 1);
  }
  // 994｜腐烂的橘子｜中等
  int orangesRotting(vector<vector<int>> &grid) {
    int fresh = 0;
    int m = grid.size();
    int n = grid[0].size();
    queue<pair<int, int>> q;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == 2) {
          q.push({i, j});
        } else if (grid[i][j] == 1) {
          fresh++;
        }
      }
    }
    vector<int> direction_x{0, -1, 0, 1};
    vector<int> direction_y{-1, 0, 1, 0};
    int res = 0;
    while (fresh && !q.empty()) {
      vector<pair<int, int>> temp;
      while (!q.empty()) {
        temp.emplace_back(q.front());
        q.pop();
      }
      for (auto &&e : temp) {
        int x = 0;
        int y = 0;
        for (int i = 0; i < 4; i++) {
          x = e.first + direction_x[i];
          y = e.second + direction_y[i];
          if (x < 0 || x >= m || y < 0 || y >= n || grid[x][y] == 2 ||
              grid[x][y] == 0)
            continue;
          grid[x][y] = 2;
          fresh--;
          q.push({x, y});
        }
      }
      res++;
    }
    return fresh ? -1 : res;
  }
  // 207｜课程表｜中等
  bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
    vector<vector<int>> graph(numCourses);
    vector<int> indegree(numCourses, 0);
    for (int i = 0; i < prerequisites.size(); i++) {
      graph[prerequisites[i][1]].emplace_back(prerequisites[i][0]);
      indegree[prerequisites[i][0]]++;
    }
    queue<int> q;
    for (int i = 0; i < numCourses; i++) {
      if (indegree[i] == 0)
        q.push(i);
    }
    while (!q.empty()) {
      int temp = q.front();
      q.pop();
      vector<int> next = graph[temp];
      for (int i = 0; i < next.size(); i++) {
        indegree[next[i]]--;
        if (indegree[next[i]] == 0)
          q.push(next[i]);
      }
    }
    for (auto &&e : indegree) {
      if (e != 0)
        return false;
    }
    return true;
  }
};
// 208｜实现 Trie (前缀树)｜中等
class Trie {
public:
  struct TreeNode {
    bool isEnd;
    array<TreeNode *, 26> son;
    TreeNode() : isEnd(false), son() {};
  };

  TreeNode *root;
  Trie() : root(new TreeNode()) {}

  void insert(string word) {
    TreeNode *cur = root;
    for (auto &&c : word) {
      if (cur->son[c - 'a'] == nullptr) {
        cur->son[c - 'a'] = new TreeNode();
      }
      cur = cur->son[c - 'a'];
    }
    cur->isEnd = true;
  }

  bool search(string word) {
    TreeNode *cur = root;
    for (auto &&c : word) {
      if (cur->son[c - 'a'] == nullptr)
        return false;
      cur = cur->son[c - 'a'];
    }
    return cur->isEnd;
  }

  bool startsWith(string prefix) {
    TreeNode *cur = root;
    for (auto &&c : prefix) {
      if (cur->son[c - 'a'] == nullptr)
        return false;
      cur = cur->son[c - 'a'];
    }
    return true;
  }
};
} // namespace lesson12

int main() {
  using namespace lesson12;
  return 0;
}