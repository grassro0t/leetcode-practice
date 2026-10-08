#include <coroutine>
#include <iostream>

// 任务对象，对外接口，持有协程句柄
struct Task {
    struct promise_type {
        // 创建协程时，返回Task对象给调用者
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        // initial_suspend：suspend_always → 创建协程立刻挂起（惰性执行）
        std::suspend_always initial_suspend() noexcept { return {}; }
        // 协程执行完毕后挂起，需要手动destroy
        std::suspend_always final_suspend() noexcept { return {}; }
        // co_return; 无返回值版本
        void return_void() {}
        // 异常处理
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> h;
    Task(std::coroutine_handle<promise_type> handle) : h(handle) {}
    ~Task() { if(h) h.destroy(); }

    // 让Task本身可以被 co_await，需要实现awaiter接口
    bool await_ready() noexcept {
        return false; // false：一定要挂起，去调用await_suspend
    }
    void await_suspend(std::coroutine_handle<> caller_h) noexcept {
        // caller_h：等待当前Task的上层协程句柄
        // 恢复本任务，本任务跑完后，这里简单直接恢复上层协程（极简调度）
        h.resume();
        caller_h.resume();
    }
    void await_resume() noexcept {
        // co_await 表达式完成后返回的值，这里无返回
    }

    // 手动启动任务
    void start() {
        h.resume();
    }
};

// 子协程：被await的任务
Task sub_task() {
    std::cout << "sub_task: start\n";
    co_return;
}

// 主协程：await子协程
Task main_task() {
    std::cout << "main_task: before await\n";
    co_await sub_task(); // 暂停main_task，等待sub_task完成
    std::cout << "main_task: after await\n";
    co_return;
}

int main() {
    auto t = main_task();
    t.start();
    return 0;
}