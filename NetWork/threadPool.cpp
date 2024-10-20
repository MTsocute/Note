#include "include/stdc++.h"

class ThreadPool {
public:
    ThreadPool(const ThreadPool & other) = delete;
    ThreadPool operator=(const ThreadPool & other) = delete;

    static ThreadPool& get_instance(int thread_num) {
        std::call_once(once, [&thread_num]() {
            instance.reset(new ThreadPool(thread_num));
        });
        return *instance;
    }
    

    template<class F, class ...Args>
    void add_task(F && func, Args... args) {
        auto task = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
        {
            std::unique_lock lock(this->_mtx);
            tasks.emplace(std::move(task));
        }
        this->condition.notify_one();
    }

    ~ThreadPool() {
        this->stop = true;
        this->condition.notify_all();

        // 还在等待的线程全部都等他们执行完
        for (std::thread & t : threads) {
            if (t.joinable()) t.join();
        }
    }

private:    
    explicit ThreadPool(const int & thread_num) {
        // 往线程池的线程塞入线程函数
        for (int i = 0; i < thread_num; i++){
            // emplace 会生成一个 thread 的，所以我们填的参数其实是线程里的函数
            threads.emplace_back(&ThreadPool::thread_exec, this);       // 写法一
            // 感觉效率低一些，就是传入了 lambda 函数，lambda函数再使用 thread_exec 函数效率低
            // threads.emplace_back([this](){ thread_exec(); });          // 写法二
        }
    }

    void thread_exec() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock lock(_mtx);
                condition.wait(lock, [this]() {return stop.load() || !tasks.empty();});

                if (stop and tasks.empty()) return;

                if (!tasks.empty()) {
                    task = tasks.front();
                    tasks.pop();
                }
            }
            task();
        }
    }


    std::atomic<bool> stop = false;
    std::mutex _mtx;
    static std::once_flag once;
    static std::unique_ptr<ThreadPool> instance;
    std::condition_variable condition;

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
};

std::unique_ptr<ThreadPool> ThreadPool::instance = nullptr;
std::once_flag ThreadPool::once;



int main() {
    auto &p = ThreadPool::get_instance(2);
    std::mutex cout_mtx;
    
    for (int i = 0; i < 100; i++) {
        p.add_task([i, &cout_mtx]() {
            std::unique_lock lock(cout_mtx);
            std::cout << "The Task " << i << " is running!\n";
        });
        std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }

    return 0;
}