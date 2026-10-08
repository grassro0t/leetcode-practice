#include <coroutine>
#include <iostream>

// 协程返回对象，对外暴露接口，持有协程句柄
struct Gen {
    // 必须内嵌 promise_type，编译器自动识别
    struct promise_type {
        int out_val; // 用来保存 co_yield 产出的值

        // 1. 协程创建第一步：构造promise后调用，返回给调用者的对象
        Gen get_return_object() {
            return Gen{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        // 2. initial_suspend：协程函数体开始前，是否挂起
        std::suspend_always initial_suspend() { return {}; }

        // 3. co_yield val 时调用；返回awaiter，决定yield后是否暂停
        std::suspend_always yield_value(int v) {
            out_val = v;
            return {};
        }

        // 4. 协程正常结束，无co_return返回值，必须实现return_void
        void return_void() {}

        // 5. final_suspend：协程执行完毕之后，是否挂起
        std::suspend_always final_suspend() noexcept { return {}; }

        // 6. 协程内部抛出未捕获异常时调用
        void unhandled_exception() { std::terminate(); }
    };

    // 协程句柄：操作协程帧（resume/done/destroy）
    std::coroutine_handle<promise_type> h;

    // 构造：接收句柄
    Gen(std::coroutine_handle<promise_type> handle) : h(handle) {}
    // 析构：销毁堆上协程帧，防止内存泄漏
    ~Gen() { if (h) h.destroy(); }

    // 对外接口：恢复协程执行，返回是否还有值
    bool next() {
        h.resume();        // 恢复协程，运行到下一个co_yield或者结束
        return !h.done();  // done()==true 代表协程执行完毕
    }

    // 获取co_yield产出的值
    int value() {
        return h.promise().out_val;
    }
};

// 协程函数：存在 co_yield → 编译器识别为协程，返回Gen
Gen count_up(int limit) {
    for (int i = 0; i < limit; ++i) {
        co_yield i; // 产出i，暂停协程；下次resume从这里继续
    }
    // 循环结束，隐式调用 co_return;
}

int main() {
    auto gen = count_up(3); // 创建协程，此时还没执行函数体（initial_suspend会挂起）

    while (gen.next()) {
        std::cout << gen.value() << '\n';
    }
    return 0;
}