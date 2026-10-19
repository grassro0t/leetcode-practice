#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

class AbstractFactory{
    public:
    ~AbstractFactory() = default;
    virtual unique_ptr<AbstractProduct> Create() = 0;
};

class AbstractProduct{
    public:
    ~AbstractProduct() = default;
    virtual void PrintDescription() = 0;
};

class AProduct: public AbstractProduct{
    public:
    void PrintDescription() override{
        cout<<"AProduct"<<endl;
    }
};

class BProduct: public AbstractProduct{
    public:
    void PrintDescription() override{
        cout<<"BProduct"<<endl;
    }
};

class AFactory: public AbstractFactory{
    public:
    unique_ptr<AbstractProduct> Create() override{
        return make_unique<AProduct>();
    }
};

class BFactory: public AbstractFactory{
    public:
    unique_ptr<AbstractProduct> Create() override{
        return make_unique<BProduct>();
    }
};

int main(){
    unique_ptr<AbstractFactory> af = make_unique<AFactory>();
    unique_ptr<AbstractFactory> bf = make_unique<BFactory>();
    unique_ptr<AbstractProduct> a = af->Create();
    unique_ptr<AbstractProduct> b = bf->Create();
    a->PrintDescription();
    b->PrintDescription();
    return 0;
}