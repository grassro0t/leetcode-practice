#include <iostream>
#include <memory>

using namespace std;

class SortAlgorithem {
public:
  virtual ~SortAlgorithem() = default;
  virtual void run() = 0;
};

class QuickSort : public SortAlgorithem {
public:
  void run() override { cout << "使用快速排序" << endl; }
};

class MergeSort : public SortAlgorithem {
public:
  void run() override { cout << "使用归并排序" << endl; }
};

class LeetCodeProducer {
private:
  shared_ptr<SortAlgorithem> sort_strategy;

public:
  LeetCodeProducer(shared_ptr<SortAlgorithem> strategy)
      : sort_strategy(strategy) {}
  void SeekK() {
    cout << "前置准备" << endl;
    if (sort_strategy) {
      sort_strategy->run();
    } else {
      cout << "未设置排序策略" << endl;
    }
    cout << "完成计算" << endl;
  }
};

int main() {
  shared_ptr<SortAlgorithem> qs = make_shared<QuickSort>();
  shared_ptr<SortAlgorithem> ms = make_shared<MergeSort>();
  LeetCodeProducer l1(qs);
  LeetCodeProducer l2(ms);
  l1.SeekK();
  l2.SeekK();
  return 0;
}