#include <iostream>
#include <thread>
#include <string>
#include <memory>

void show_sth(std::string &sth) {
    std::cout << "Hello, " << sth << std::endl;
}

inline void plus_one(int &a) { a++; }

class demo {
public: // 修改为 public，确保可以从外部调用
    void say_hello() { std::cout << "Hello World!" << std::endl; }

    // 如果想要这个私有的成语暗变亮
private:
    friend void thread_func();

    void inside() { std::cout << "Hello World! but inside" << std::endl; }
};


// 使用这个类的有元函数，访问类内函数
void thread_func() {
    auto p2 = std::make_unique<demo>();

    std::thread t(&demo::inside, p2.get());

    t.join();
}


int main() {
    std::string a = "Wiki";

    auto p1 = std::make_unique<demo>(); // p1 is a pointer to demo

    // 正确传递 p1 对象给线程
    std::thread t1(&demo::say_hello, p1.get()); // 传递裸指针

    thread_func();

    t1.join();

    return 0;
}
