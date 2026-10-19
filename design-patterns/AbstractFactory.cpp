#include <iostream>
#include <memory>

using namespace std;

class HumanFactory {
public:
  ~HumanFactory();
  virtual unique_ptr<Hand> get_hand() = 0;
  virtual unique_ptr<Face> get_face() = 0;
};

class Hand {
public:
  ~Hand();
  virtual void WashHand() = 0;
};

class Face {
public:
  ~Face();
  virtual void ShowFace() = 0;
};

class BlackHumanFactory : public HumanFactory {
  unique_ptr<Hand> get_hand() override {
    return make_unique<BlackHand>();
  }
  unique_ptr<Face> get_face() override {
    return make_unique<BlackFace>();
  }
};

class YellowHumanFactory : public HumanFactory {
  unique_ptr<Hand> get_hand() override {
    return make_unique<YellowHand>();
  }
  unique_ptr<Face> get_face() override {
    return make_unique<YellowFace>();
  }
};

class BlackHand : public Hand {
public:
  void WashHand() override {
    cout<<"wash BlackHand"<<endl;
  }
};

class BlackFace : public Face {
public:
  void ShowFace() override {
    cout<<"show BlackFace"<<endl;
  }
};

class YellowHand : public Hand {
public:
  void WashHand() override {
    cout<<"wash YellowHand"<<endl;
  }
};

class YellowFace : public Face {
public:
  void ShowFace() override {
    cout<<"show YellowFace"<<endl;
  }
};

int main(){
    unique_ptr<HumanFactory> bf = make_unique<BlackHumanFactory>();
    unique_ptr<HumanFactory> yf = make_unique<YellowHumanFactory>();
    unique_ptr<Hand> bhand = bf->get_hand();
    unique_ptr<Face> bface = bf->get_face();
    bhand->WashHand();
    bface->ShowFace();
    return 0;
}