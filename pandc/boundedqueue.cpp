#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class BoundedBlockingQueue {
public:
    explicit BoundedBlockingQueue(size_t cap) : capacity(cap), stop(false) {}

    // 入队：返回true成功；false代表队列已停止，不再入队
    bool enqueue(T val) {
        std::unique_lock<std::mutex> lock(mtx);
        // 队列满 并且 没有停止，则等待not_full
        cv_not_full.wait(lock, [this](){
            return stop || q.size() < capacity;
        });
        if(stop) return false;
        q.push(std::move(val));
        cv_not_empty.notify_one(); // 通知有数据可取
        return true;
    }

    // 出队：返回true拿到元素；false代表队列停止，无数据
    bool dequeue(T& out) {
        std::unique_lock<std::mutex> lock(mtx);
        // 队空 并且 没有停止，则等待not_empty
        cv_not_empty.wait(lock, [this](){
            return stop || !q.empty();
        });
        if(stop) return false;
        out = std::move(q.front());
        q.pop();
        cv_not_full.notify_one(); // 通知有空位可放
        return true;
    }

    // 优雅关闭：设置stop标记，唤醒全部等待线程
    void shutdown() {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;
        cv_not_full.notify_all();
        cv_not_empty.notify_all();
    }

private:
    std::queue<T> q;
    size_t capacity;
    std::mutex mtx;
    std::condition_variable cv_not_full;
    std::condition_variable cv_not_empty;
    bool stop;
};