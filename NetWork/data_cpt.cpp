#include <iostream>
#include <thread>
#include <mutex>

std::mutex mutexA;
std::mutex mutexB;

void thread1() {
    // 手动先锁定 mutexA，再锁定 mutexB
    mutexA.lock();
    std::cout << "Thread 1 locked mutexA" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    mutexB.lock();
    std::cout << "Thread 1 locked mutexB" << std::endl;

    // 解锁
    mutexB.unlock();
    mutexA.unlock();
}

void thread2() {
    // 手动先锁定 mutexB，再锁定 mutexA
    mutexB.lock();
    std::cout << "Thread 2 locked mutexB" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    mutexA.lock();
    std::cout << "Thread 2 locked mutexA" << std::endl;

    // 解锁
    mutexA.unlock();
    mutexB.unlock();
}

int main() {
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    return 0;
}
