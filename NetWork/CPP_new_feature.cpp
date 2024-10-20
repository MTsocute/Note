#include <iostream>
#include <utility>

// 对每个参数调用 print_func
template<class F, class... Args>
void nice_print(F &&print_func, Args &&... args) {
    std::forward<F>(print_func)(std::forward<Args>(args)...);

}

int main() {
    nice_print([](const int & a, const std::string & name) {
        std::cout << name << " : " << a << std::endl;
    }, 520, "momo");


    return 0;
}