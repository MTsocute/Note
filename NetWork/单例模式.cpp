#include <iostream>
#include <thread>
#include <mutex>

class Singleton {
public:
    // 禁止拷贝构造和赋值构造函数
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 获取单例实例
    static Singleton& getInstance() {
        // 使用 std::call_once 确保实例只会初始化一次
        std::call_once(initOnceFlag, []() {
            instance = new Singleton();
        });
        return *instance;
    }

    // 打印信息
    void printMessage(const std::string& msg) {
        std::cout << "Message: " << msg << " -- Instance Address: " << this << std::endl;
    }

private:
    Singleton() = default;
    static Singleton* instance;
    static std::once_flag initOnceFlag;
};

// 静态变量初始化
Singleton* Singleton::instance = nullptr;
std::once_flag Singleton::initOnceFlag;

void threadFunc() {
    Singleton::getInstance().printMessage("Hello from thread");
}

int main() {
    // 创建两个线程
    std::thread t1(threadFunc);
    std::thread t2(threadFunc);

    t1.join();
    t2.join();

    return 0;
}
