#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>
#include <iostream>
#include <chrono>

class ThreadPool {
public:
    explicit ThreadPool(size_t n) : stop(false) {
        for(size_t i = 0; i < n; ++i) {
            // 构造里只创建线程，线程入口绑定到成员函数 worker_loop
            workers.emplace_back([this](){ worker_loop(); });
        }
    }

    template<typename F>
    void submit(F&& f) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.emplace(std::forward<F>(f));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        for(auto& th : workers) {
            th.join();
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
private:
    // 【剥离出来的worker主循环】
    void worker_loop() {
        for(;;) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){
                    return stop || !tasks.empty();
                });
                if(stop && tasks.empty()) {
                    return;
                }
                task = std::move(tasks.front());
                tasks.pop();
            }
            task(); // 锁外执行任务
        }
    }

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
};

int main() {
    ThreadPool pool(4);
    for(int i = 0; i < 8; ++i) {
        pool.submit([i](){
            std::cout << "task " << i << " run on thread " << std::this_thread::get_id() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }
    return 0;
}