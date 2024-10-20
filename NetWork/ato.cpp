#include <iostream>
#include <atomic>
#include <future>  // 包含 std::async, std::future 等

static std::pmr::unordered_map<char, int> pmr_map = {
    {'A', 20},
    {'B', 30}
};


// 函数：增加 atomic 变量
void func(std::atomic<int>& change_data) {
    for (size_t i = 0; i < 10000; i++) {
        change_data++;    // 原子递增
    }
}

// 定义全局原子变量
static std::atomic<int> shared_data(0);

int main() {
    // 启动两个异步任务，并获取它们的 future 对象
    std::future<void> f1 = std::async(std::launch::async, func, std::ref(shared_data));
    std::future<void> f2 = std::async(std::launch::async, func, std::ref(shared_data));

    // 等待两个异步任务完成
    f1.get();
    f2.get();

    // 输出结果
    std::cout << "Final shared_data value: " << shared_data.load() << std::endl;

    return 0;
}
