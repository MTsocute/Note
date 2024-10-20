#include "include/stdc++.h"

int func() {
    int ret = 0;

    for (size_t i = 0; i < 1000; i++) {
        ret ++;
    }
    return ret;
}

class Solution {
public:
    static constexpr double mySqrt(double x) {
        if (x == 0) return 0;
        double t0 = 10;	double X = x;
        
        auto newtown = [](double t0, double X)->double {
            double t1 = 0;
            while (1) {
                t1 = t0 - (t0 * t0 - X) / (2.0 * t0);
                if (fabs(t1 - t0) < 1e-7) break;
                t0 = t1;
            }
            return t1;
        };

        return newtown(t0 ,X);
    }

};


int main() {
    std::packaged_task<int(void)> task(func);

    std::future<int> result = task.get_future();

    std::thread task_thread(std::move(task));

    int out = result.get();     // result.get() 会阻塞线程一直到结束

    std::cout << "result: " << out << std::endl;

    if (task_thread.joinable()) task_thread.join();

    return 0;
}
