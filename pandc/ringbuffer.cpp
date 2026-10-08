#include <atomic>
#include <vector>

template<typename T>
class SpscRingQueue
{
public:
    explicit SpscRingQueue(size_t capacity)
        : cap_(capacity + 1), buf_(cap_)
    {
        write_idx_.store(0, std::memory_order_relaxed);
        read_idx_.store(0, std::memory_order_relaxed);
    }

    // 生产者调用：仅单个线程调用
    bool enqueue(const T& val)
    {
        size_t w = write_idx_.load(std::memory_order_relaxed);
        size_t next_w = (w + 1) % cap_;
        if(next_w == read_idx_.load(std::memory_order_acquire))
        {
            // 队列满
            return false;
        }
        buf_[w] = val;
        write_idx_.store(next_w, std::memory_order_release);
        return true;
    }

    // 消费者调用：仅单个线程调用
    bool dequeue(T& out)
    {
        size_t r = read_idx_.load(std::memory_order_relaxed);
        if(r == write_idx_.load(std::memory_order_acquire))
        {
            // 队空
            return false;
        }
        out = buf_[r];
        size_t next_r = (r + 1) % cap_;
        read_idx_.store(next_r, std::memory_order_release);
        return true;
    }

private:
    const size_t cap_;
    std::vector<T> buf_;
    std::atomic<size_t> write_idx_;
    std::atomic<size_t> read_idx_;
};