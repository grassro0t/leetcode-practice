#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class ObjectPool {
public:
    // 预创建count个对象
    explicit ObjectPool(size_t count) : stop(false) {
        for(size_t i = 0; i < count; ++i) {
            // 堆分配对象放入空闲队列
            free_objs.push(new T());
        }
    }

    // 获取对象，无空闲则阻塞等待
    T* acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this](){
            return stop || !free_objs.empty();
        });
        if(stop) return nullptr;

        T* obj = free_objs.front();
        free_objs.pop();
        return obj;
    }

    // 归还对象
    void release(T* obj) {
        if(!obj) return;
        std::lock_guard<std::mutex> lock(mtx);
        free_objs.push(obj);
        cv.notify_one();
    }

    // 析构：销毁所有缓存对象
    ~ObjectPool() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();

        std::lock_guard<std::mutex> lock(mtx);
        while(!free_objs.empty()) {
            T* p = free_objs.front();
            free_objs.pop();
            delete p;
        }
    }

    // 禁止拷贝
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
private:
    std::queue<T*> free_objs;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
};

// RAII自动归还Guard（强烈推荐，防止忘记release）
template<typename T>
class ObjGuard {
public:
    ObjGuard(ObjectPool<T>& pool, T* o) : pool_(pool), obj_(o) {}
    ~ObjGuard() {
        if(obj_) pool_.release(obj_);
    }
    T* get() { return obj_; }
private:
    ObjectPool<T>& pool_;
    T* obj_;
};

// 测试
#include <iostream>
struct Foo {
    Foo() { std::cout << "Foo construct\n"; }
    ~Foo() { std::cout << "Foo destroy\n"; }
    void hello() { std::cout << "hello from Foo\n"; }
};

int main() {
    ObjectPool<Foo> pool(2); // 预先创建2个Foo

    {
        ObjGuard<Foo> g1(pool, pool.acquire());
        ObjGuard<Foo> g2(pool, pool.acquire());
        g1.get()->hello();
        g2.get()->hello();
        // 出作用域自动release归还
    }
    std::cout << "objects returned to pool\n";
    return 0;
}