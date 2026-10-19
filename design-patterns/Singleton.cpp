#include <iostream>

class Singleton{
    private:
    Singleton() = default;
    public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    static Singleton& Instance(){
        static Singleton s;
        return s;
    }
};

class SingletonStarve{
    private:
    static SingletonStarve s;
    SingletonStarve() = default;
    public:
    SingletonStarve(const SingletonStarve&) = delete;
    SingletonStarve& operator=(const SingletonStarve&) = delete;
    static SingletonStarve& Instance(){
        return s;
    }
};

int main(){
    Singleton& obj = Singleton::Instance();
    return 0;
}