#include "include/stdc++.h"

std::queue<int> que;
std::condition_variable g_cv;
std::mutex mtx;
bool stop = false; // 停止标志

void producer() {
    // 加任务
    for (int i = 0; i < 10; i++) {
        {
            std::lock_guard<std::mutex> lg(mtx);
            que.push(i);
            g_cv.notify_one(); // 通知可以不需要等待了
            std::cout << "Producer: " << i << std::endl;
        }
        // 延迟时间可以调整，减少甚至去掉
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 可根据需要调整
    }

    // 生产结束后，通知消费者退出
    {
        std::lock_guard<std::mutex> lg(mtx);
        stop = true;
        g_cv.notify_all(); // 通知所有消费者
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> ul(mtx);
        // 两种情况不等待，一个是由任务要做，另一个是彻底没有任务了
        g_cv.wait(ul, []() { return !que.empty() || stop; });

        if (!que.empty()) {
            const int val = que.front();
            que.pop();
            std::cout << "Consumer: " << val << std::endl;
        }
        // 虽然队列为空，但是那是因为没有任务了
        else if (stop && que.empty())   break;

        // 调整消费的延迟，根据实际场景需要
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); // 延迟也可以去掉或改短
    }
}

int main() {
    std::thread t_producer(producer);
    std::thread t_consumer(consumer);

    t_producer.join();
    t_consumer.join();

    return 0;
}
