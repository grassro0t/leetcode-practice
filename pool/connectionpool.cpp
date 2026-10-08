#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <mysql/mysql.h>
#include <string>

struct MysqlConn {
    MYSQL* mysql;
    MysqlConn() {
        mysql = mysql_init(nullptr);
    }
    ~MysqlConn() {
        if(mysql) {
            mysql_close(mysql);
        }
    }
    // 禁止拷贝
    MysqlConn(const MysqlConn&) = delete;
    MysqlConn& operator=(const MysqlConn&) = delete;
};

class MysqlConnPool {
public:
    // 构造：初始化N个连接
    MysqlConnPool(size_t max_conn, const std::string& host, int port,
                  const std::string& user, const std::string& passwd, const std::string& db)
        : max_conn_(max_conn), stop_(false)
    {
        for(size_t i = 0; i < max_conn_; ++i) {
            auto conn = new MysqlConn;
            if (!mysql_real_connect(conn->mysql, host.c_str(),
                                    user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0))
            {
                std::cerr << "connect fail: " << mysql_error(conn->mysql) << "\n";
                delete conn;
                continue;
            }
            free_conns_.push(conn);
        }
    }

    // 获取连接：阻塞等待直到拿到空闲连接
    MysqlConn* get() {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this](){
            return stop_ || !free_conns_.empty();
        });
        if(stop_) return nullptr;

        MysqlConn* c = free_conns_.front();
        free_conns_.pop();
        return c;
    }

    // 归还连接
    void release(MysqlConn* c) {
        if(!c) return;
        std::lock_guard<std::mutex> lock(mtx_);
        free_conns_.push(c);
        cv_.notify_one();
    }

    // 析构：停止，销毁所有连接
    ~MysqlConnPool() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_all();

        std::lock_guard<std::mutex> lock(mtx_);
        while(!free_conns_.empty()) {
            auto c = free_conns_.front();
            free_conns_.pop();
            delete c;
        }
    }

    MysqlConnPool(const MysqlConnPool&) = delete;
    MysqlConnPool& operator=(const MysqlConnPool&) = delete;
private:
    size_t max_conn_;
    bool stop_;
    std::queue<MysqlConn*> free_conns_;
    std::mutex mtx_;
    std::condition_variable cv_;
};

// ===== 测试示例 =====
int main() {
    MysqlConnPool pool(4, "127.0.0.1", 3306, "root", "123456", "testdb");

    auto conn = pool.get();
    if(conn) {
        MYSQL_RES* res;
        mysql_query(conn->mysql, "select 1");
        res = mysql_store_result(conn->mysql);
        MYSQL_ROW row = mysql_fetch_row(res);
        std::cout << row[0] << "\n";
        mysql_free_result(res);

        pool.release(conn);
    }
    return 0;
}