#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

std::timed_mutex mtx;

void try_to_get_lock(int thread_id) {
    std::unique_lock<std::timed_mutex> lg(mtx, std::defer_lock); 	// defer_lock 避免自动上锁
    // 尝试在 1 秒内获取锁
    if (mtx.try_lock_for(std::chrono::seconds(1))) {
        std::cout << "Thread " << thread_id << " got the lock." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟工作
        // 这里不需要手动释放了
    }
    // 没在制定时间拿到锁的后续操作
    else {
        std::cout << "Thread " << thread_id <<
            " could not get the lock within 1 second." << std::endl;
    }
}


int main() {
    std::thread t1(try_to_get_lock, 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));  // 确保线程1先运行
    // 第一个线程结束之后会休眠一会儿的，所以第二个线程肯定做不了 1s 内上锁
    std::thread t2(try_to_get_lock, 2);

    t1.join();
    t2.join();

    return 0;
}
