#include <iostream>
#include <memory>

using namespace std;

class Drink {
public:
  virtual ~Drink() {}
  virtual void make() = 0;
};

class Coffee : public Drink {
public:
  void make() override { cout << "冲泡咖啡" << endl; }
};

class Decorator : public Drink {
protected:
  shared_ptr<Drink> drink_;

public:
  explicit Decorator(shared_ptr<Drink> drink) : drink_(drink) {}
};

class SugerDecorator : public Decorator {
public:
  explicit SugerDecorator(shared_ptr<Drink> drink) : Decorator(drink) {}
  void make() override {
    cout << "先放糖" << endl;
    drink_->make();
    cout << "搅一搅" << endl;
  }
};

class MilkDecorator : public Decorator {
public:
  explicit MilkDecorator(shared_ptr<Drink> drink) : Decorator(drink) {}
  void make() override {
    cout << "先放奶" << endl;
    drink_->make();
    cout << "搅一搅" << endl;
  }
};

int main() {
  shared_ptr<Drink> coffee = make_shared<Coffee>();
  shared_ptr<Drink> sugar_coffee = make_shared<SugerDecorator>(coffee);
  shared_ptr<Drink> milk_coffee = make_shared<MilkDecorator>(coffee);
  sugar_coffee->make();
  milk_coffee->make();
  return 0;
}