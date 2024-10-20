#include "include/stdc++.h"


class ThreadsPool {
public:
    ThreadsPool(const ThreadsPool &other) = delete;

    ThreadsPool &operator=(const ThreadsPool &other) = delete;

    // 单例开辟一个线程池实例
    static ThreadsPool &get_instance(int thread_nums) {
        std::call_once(once, [&thread_nums]() {
            instance.reset(new ThreadsPool(thread_nums));
        });
        return *instance;
    }

    // 添加需要用线程进行的函数
    void add_task(std::function<void()> func) { {
            std::unique_lock lock(this->mtx);
            tasks.emplace(std::move(func));
        }
        condition.notify_one();
    }

    // 停止线程池
    void stop_pool() {
        {
            std::unique_lock lock(mtx);
            stop = true; // 设置停止标志
        }
        condition.notify_all(); // 唤醒所有等待的线程
        for (auto &t : threads) {
            if (t.joinable()) {
                t.join(); // 等待所有线程完成
            }
        }
        threads.clear(); // 清空线程列表
    }

    // 清空残余的函数并析构
    ~ThreadsPool() {
        stop_pool(); // 在析构时调用停止方法
    }

private:
    explicit ThreadsPool(const int &thread_num) {
        instance = nullptr;
        for (int i = 0; i < thread_num; i++) {
            threads.emplace_back([this]() { thread_init(); });
        }
    }

    void thread_init() {
        while (true) {
            std::function<void()> task; {
                std::unique_lock lock(mtx);
                condition.wait(lock, [this]() { return stop || !tasks.empty(); });

                if (stop and tasks.empty()) { return; }

                if (!tasks.empty()) {
                    task = std::move(tasks.front());
                    tasks.pop();
                }
            }
            task();
        }
    }

    static std::unique_ptr<ThreadsPool> instance;
    static std::once_flag once;
    std::mutex mtx;
    std::condition_variable condition;
    bool stop = false;

    std::queue<std::function<void()> > tasks;
    std::vector<std::thread> threads;
};

// 单例模式之生成一次
std::unique_ptr<ThreadsPool> ThreadsPool::instance = nullptr;
std::once_flag ThreadsPool::once;


int main() {
    auto &p = ThreadsPool::get_instance(4);

    std::mutex cout_mtx;
    for (int i = 0; i < 10; i++) {
        p.add_task([i, &cout_mtx]() {
            {
                std::unique_lock lock(cout_mtx);
                std::cout << "The Task " << i << " is running" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        });
    }

    // 等待一段时间以确保任务执行
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
