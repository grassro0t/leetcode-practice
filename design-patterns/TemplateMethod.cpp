#include <iostream>
#include <vector>

using namespace std;

class Cooker {
protected:
  virtual void wash() = 0;
  virtual void fire() = 0;
  virtual void cook() = 0;

public:
  virtual ~Cooker() = default;
  void CookDinner() {
    cout << "开始做晚饭" << endl;
    wash();
    fire();
    cook();
    cout << "做好啦" << endl;
  }
};

class ChineseCooker : public Cooker {
protected:
  void wash() override { cout << "中国厨师先洗牛肉" << endl; }
  void fire() override { cout << "开大火" << endl; }
  void cook() override { cout << "猛火爆炒成干炒牛河" << endl; }
};

class FrenchCooker : public Cooker {
protected:
  void wash() override { cout << "法国厨师先洗洋葱" << endl; }
  void fire() override { cout << "小火慢煨" << endl; }
  void cook() override { cout << "慢炖成了奶油鸡" << endl; }
};

int main() {
  ChineseCooker ccooker;
  FrenchCooker fcooker;
  ccooker.CookDinner();
  fcooker.CookDinner();
  return 0;
}