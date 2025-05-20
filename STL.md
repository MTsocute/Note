# STL

---



## 1. 智能指针

---

> 在 <memory> 这个类中

### 1.1 unique_pointer

> 这个指针指向的对象不可以改变
>
> make_unique<指针的类型>(值)，如果指针的类型是类的话，传入的参数可以是其对应的构造函数需要的值 

```c++
int main() {
    // 创建 unique_ptr 并指向一个动态分配的 int
    std::unique_ptr<int> p1 = std::make_unique<int>(10);	// 在内部就构造一个 new int(10) 会自己释放
    std::cout << *p1 << std::endl;

    // 转移所有权
    std::unique_ptr<int> p2 = std::move(p1);  // p1 不再拥有资源
    if (!p1) {
        std::cout << "p1 is null after move" << std::endl;
    }
    return 0;  // p2 离开作用域时，自动销毁指向的对象
}
```

> 指针指针的 .get() 方法，返回存储在内部的裸指针，而非智能指针类

```c++
void show_sth(std::string &sth) {
    std::cout << "Hello, " << sth << std::endl;
}

inline void plus_one(int & a) { a ++; }

class demo {
    public:  // 修改为 public，确保可以从外部调用
    void say_hello() { std::cout << "Hello World!" << std::endl; }
};

int main() {
    std::string a = "Wiki";
    int b = 2;

    auto p1 = std::make_unique<demo>();  // p1 is a pointer to demo

    // 正确传递 p1 对象给线程
    std::thread t1(&demo::say_hello, p1.get());  // 传递裸指针
    std::thread t2(plus_one, std::ref(b));

    t1.join();
    t2.join();

    return 0;
}
```



## 2. lambda

---

> 用于记录函数变成的内容

### 2.1 std::bind()

> 绑定普通函数

```c++
// 一个普通的函数
int add(int a, int b) { return a + b; }

int main() {
    // 使用 std::bind 绑定 add 函数的第一个参数为 10
    // std::placeholders::_x 占位符，你想传入的数据先留到起
    auto bind_add = std::bind(add, 10, std::placeholders::_1);

    // 调用 bind_add，传入第二个参数
    std::cout << "10 + 5 = " << bind_add(5) << std::endl; // 输出 10 + 5 = 15

    return 0;
}
```

> 绑定成员函数

```c++
#include <iostream>
#include <functional> // for std::bind

class MyClass {
public:
    void print(int a, int b) {
        std::cout << "a: " << a << ", b: " << b << std::endl;
    }
};

int main() {
    MyClass obj;

    // 绑定成员函数，第一个参数是对象的引用
    auto bind_print = std::bind(&MyClass::print, &obj, std::placeholders::_1, 20);

    // 调用 bind_print，传入第一个参数
    bind_print(10);  // 输出 a: 10, b: 20

    return 0;
}
```

<br>

## 3. template

---

### 3.1 模板 delete 禁止某种数据的模板

```c++
// 通用模板函数
template <typename T>
// 一般这里的 T 可以是任意的数据类型
void process(T value) {
    std::cout << "Processing value: " << value << std::endl;
}

// 但是在这里：如果你传递的参数是 int,函数是用不了的
template <>
void process(int) = delete;  // 禁止 int 类型的实例化
```

### ==3.2 可变参数函数模板==

> 第一个 first 参数每一次都会拿走一个参数，然后传递下去的 args 就会越来越小，递归结束

```c++
// 定义一个可以接收任意数量参数的函数模板
template<class F, class... Args>
void print(F first, Args... args) {
    std::cout << first << std::endl;
    if constexpr (sizeof...(args) > 0) {  // 判断是否还有参数
        print(args...);  // 递归调用，继续处理剩余的参数
    }
}

int main() {
    print(1, 2.5, "hello", std::string("world"));
    return 0;
}
```

> 当然对于 `if constexpr` 也可以简化

```c++
// 可变参数函数模板
template<class... Args>
void print(Args... args) {
    // 使用折叠表达式打印参数
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}
```



### 3.3 函数模版的 && 引用是万能引用

> 使用 模板 作为参数类型声明的时候，&& 会自动推导是左右值引用

```c++
// 函数模板，使用万能引用
template<typename T>
void process(T&& arg) {
    // 判断 arg 是左值还是右值
    std::cout << "Inside process: ";
    if constexpr (std::is_lvalue_reference<T>::value)   std::cout << "Lvalue" << std::endl;
    else	std::cout << "Rvalue" << std::endl;
    
    // 对于数据的处理，需要完美转发
    std::cout << "Value: " << arg << std::endl;	// 其实是不行的，这里是告诉你想干嘛
}

int main() {
    int x = 10; // 左值

    process(x); 			// 传入左值
    process(std::move(x));	// 传入右值
    return 0;
}
```

### ==3.4 万能引用需要考虑的完美转发问题==

> 上面我们到 ：`std::cout << "Value: " << arg << std::endl;` 没有说完话
>
> 因为，是这样子的，我们接收到的数据可能是左 /右值，但是对于数据我们想拿去用的话，实际上要用 **完美转发** 才能拿去用
>
> 所以下面我们引入一个函数的介绍

```c++
template<typename T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept;
```

**arg**：实际传递给 `std::forward` 的参数。

- 当 `T` 是 lvalue 引用时，`std::forward` 会将参数作为 lvalue 转发。
- 当 `T` 是 rvalue 引用时，`std::forward` 会将参数作为 rvalue 转发。

**函数返回值：**返回的是一个与传入参数的值类别相匹配的引用。

- 如果传入的是 lvalue，则返回 lvalue 引用。
- 如果传入的是 rvalue，则返回 rvalue 引用。

`std::forward` 的主要作用：

- **完美转发**：保留参数的值类别，将其转发给另一个函数。避免了不必要的拷贝或移动。
- **解决重载问题**：帮助模板函数在参数传递时正确选择合适的重载函数。

```c++
#include <utility>  // std::forward

// 这是我们要调用的两个重载函数
void process(int &x) {
    std::cout << "Lvalue reference: " << x << std::endl;
}

void process(int &&x) {
    std::cout << "Rvalue reference: " << x << std::endl;
}

// 使用模板函数来完美转发参数
template<typename T>
void forwardToProcess(T &&arg) {
    // 这里用了 process 函数
    // 转发函数会告诉用的到底是左还是右值
    process(std::forward<T>(arg));  // 保留参数的值类别并进行转发
}

int main() {
    int a = 10;
    forwardToProcess(a);    // 传递的是 lvalue，调用 process(int&)
    forwardToProcess(20);   // 传递的是 rvalue，调用 process(int&&)
    return 0;
}
```

> 所以我们上面的代码应该修正为

```c++
// 函数模板，使用万能引用
template<typename T>
void process(T&& arg) {
    // 判断 arg 是左值还是右值
    std::cout << "Inside process: ";
    if constexpr (std::is_lvalue_reference<T>::value)   std::cout << "Lvalue" << std::endl;
    else	std::cout << "Rvalue" << std::endl;
    
    // 对于数据的处理，需要完美转发
	print_val(std::forward<T>(arg));	// print_val 自己实现
}
```

<br>

### ==3.5 可变参数函数模板与引用==

> 对于可变参数模板第一类写法，我们需要考虑传入的参数是左右值的问题
>
> 比原来的就多一个步骤，就是我们传送数据的时候多一个 `std::forward()`

```c++
template<class F, class... Args>
void print_a_lot(F &&f, Args &&... args) {
    // 对第一个参数调用
    std::cout << std::forward<F>(f);
    
    // 如果有更多的参数，递归调用print_a_lot
    if constexpr (sizeof...(args) > 0) {
        std::cout << ", ";
        // 递归传递剩余的，记得完美转发
        print_a_lot(std::forward<Args>(args)...);
    }
}

int main() {
    print_a_lot(1, 2.5, "Hello", 'A'); 
    return 0;
}
```

### 3.6 可变参数函数模板实现，传入任意函数和对应数量参数调用函数

```c++
#include <iostream>
#include <utility>

// 对每个参数调用 print_func
template<class F, class... Args>
void nice_print(F &&print_func, Args &&... args) {
    // 调用 print_func 处理当前参数
    auto func = std::bind(std::forward<F>(print_func), std::forward<Args>(args)...);
	// 调用函数
    func();
}


// 任意数量的参数和不同的函数，都可以通过 nice_print 调用
// 虽然说直接用更快一些
// 如果场景到线程池，很多不同的函数和不同的参数，循化调用，价值就体现出来了
int main() {
    nice_print([](int a, std::string name) {
        std::cout << name << " : " << a << std::endl;
    }, 1,"hello world");


    nice_print([](std::string name, int a, std::string name2) {
        std::cout << "hello " << a << " " << name2 << std::endl;
    }, "nintendo", 123.123, "momo");

    return 0;
}
```



## 4. string && stringstream

---

### 4.1 读取一个 string 所有的分隔字符

> 默认是按照空格来切分的

```c++
std::string text = "Hello world this is a C++ example";
std::stringstream ss(text);
std::string word;

while (ss >> word) {
    // word 会被上一次的内容覆盖
    std::cout << word << std::endl;  // 每次提取的单词都会被输出
}
```

> 按照其他符号来切分

```c++
int main() {
    std::string text = "Hello,world,this,is,C++,example";
    std::stringstream ss(text);
    std::string word;

    while (std::getline(ss, word, ',')) {  // 使用逗号作为分隔符
        std::cout << word << std::endl;  // 输出单词
    }

    return 0;
}
```

### 4.2 根据指定的开头和长度，获取子串

> 如果没有第二个参数，就是默认到最后

```c++
str.substr(start, length)
```

## 5. tuple

---

### 5.1 std::tie

```c++
int main() {
    // 创建一个元组，包含多个数据
    std::tuple<int, double, std::string> tup = std::make_tuple(1, 3.14, "hello");

    // 使用 std::tie 来解构元组中的值到多个变量中
    int i; double d; std::string s;
    std::tie(i, d, s) = tup;

    std::cout << "i: " << i << ", d: " << d << ", s: " << s << std::endl;
    return 0;
}
```

```shell
$ i: 1, d: 3.14, s: hello
```

> 但是在 C++ 17 有一个新的特性可以简化

```c++
int main() {
    // 创建一个元组，包含多个数据
    auto tup = std::make_tuple(1, 3.14, "hello");
    auto &[i,d,s] = tup;
z
    std::cout << "i: " << i << ", d: " << d << ", s: " << s << std::endl;
    return 0;
}
```

<br>

## 6. List

---

> `std::list` 是 C++ STL（标准模板库）中一个双向链表（doubly linked list）的实现。与 `std::vector` 不同，`std::list` 支持快速的插入和删除操作
>
> 除了插入和删除要指定 `front` 和 `back` 其余操作和 `vector` 没有区别

### 6.1 指定位置操作

```c++
auto it = myList.begin();
myList.insert(it, 20);  // 在头部插入 20
myList.erase(it);       // 删除头部元素
```

## 7. unordered_map

---

#### 7.1 哈希表的 `==` 比较

> 在这种情况下，`dict_s == dict_p` 将返回 `true`，因为它们的键和值完全一致。
>
> 如果 `dict_s` 中某个字符的频率不同或者包含额外的键值对（字符），则比较将返回 `false`

```c++
dict_p = { 'a': 1, 'b': 1, 'c': 1 }
dict_s = { 'c': 1, 'a': 1, 'b': 1 }
```

## 8. filesystem

---

### ==8.1 filestream==

> ### 读文件的操作
>
> 这个 `is_open()` 不需要文件是否存在与否，保证文件能够正常打开就行

```cpp
#include <fstream>

// 在对应路径创建一个文件并写入一些内容
std::ofstream outFile("/data.txt", std::ios::out | std::ios::trunc);

// 写入内容
if (outFile.is_open()) {
    outFile << "fuck you linux";
    outFile.close();
}

else { std::cerr << "Failed to open the file" << std::endl; }
```

> ### 读文件的操作

```cpp
#include <fstream>

std::ifstream inFile("data.txt"); // 打开文件

if (!inFile) {  // 检查文件是否打开成功
    std::cerr << "Failed to open file!" << std::endl;
    EXIT();
}

std::string line;		// 缓冲区

// 按行读取文件内容
while (std::getline(inFile, line)) {
    std::cout << line << std::endl;  // 输出每一行
}

inFile.close();  // 关闭文件
```

<br>

### 8.2 创建一个文件夹

> 使用 `fs::create_directory` 创建单一目录
>
> 使用 `fs::create_directories` 创建多层目录（如果父目录不存在，会一并创建）

### 8.3 `path`的一些操作

> 显示当前所在的路径

```cpp
std:: cout << fs::current_path() << std::endl;
```

> `path` 是 `fs` 的一个类，对 `/` 进行了重写，所以可以通过改写这个来改变路径 

```cpp
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // 获取当前工作路径并拼接上上一级目录
    fs::path dirPath = fs::current_path() / "..";
    std::cout << dirPath << std::endl;

    return 0;
}
```

### 8.4 遍历一个文件夹的所有内容

```cpp
 fs::directory_iterator dir(this->img_path);     // 一个文件夹的遍历器
// 这里的 auto 是 dir_enrty 类对象，具体是啥你可以看看官网
for (const auto & it : dir) {
    std::cout << it.path().filename().string() << std::endl;
}
```



# Algorithm

---

> 内置的算法库

## 1. std::rotate()

> `std::rotate(first, new_first, last)` 进行如下操作：
>
> - 把 `[first, new_first)` 这部分元素移动到后面。
> - 把 `[new_first, last)` 这部分元素移动到前面。

```c++
std::rotate(first, new_first, last);
```

- **左旋转**：将前面的元素移动到后面，相当于把序列头部的元素“旋转”到尾部

```c++
std::vector<int> arr = {1, 2, 3, 4, 5};

std::rotate(arr.begin(), arr.begin() + 2, arr.end());
```

```markdown
> 3 4 5 1 2
```

- **右旋转**：将后面的元素移动到前面，相当于把序列尾部的元素“旋转”到头部

```c++
std::rotate(arr.begin(), arr.end() - 2, arr.end());
```

```markdown
> 4 5 1 2 3
```

<br>

## 2. std::adjacent_find()

> `std::adjacent_find()` 是 C++ 标准库中的一个算法函数，用于查找容器中相邻的两个满足特定条件的元素
>
> - `first`, `last`：范围 `[first, last)`，表示要查找的区间
>
> - `p`：二元谓词（即一个接受两个参数的 lambda 或函数），用于判断相邻的元素是否满足特定条件

```c++
iterator adjacent_find(iterator first, iterator last);
// 带谓词的版本
iterator adjacent_find(iterator first, iterator last, BinaryPredicate p);
```



### 2.1 带谓词的版本：查找满足条件的相邻元素

>  `std::greater_equal<int>()` 作为条件，表示如果前一个元素大于或等于后一个元素，就返回这个位置的迭代器

```c++
bool is_increasing(const std::vector<int>& nums) {
    // 如果出现了不满足条件的地方返回的是哪个位置的迭代器
    // 如果都是满足条件的，那么最后一定是到结尾位置
    return std::adjacent_find(nums.begin(), nums.end(), std::greater_equal<int>()) == nums.end();
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    if (is_increasing(nums)) std::cout << "The vector is strictly increasing." << std::endl;
    else std::cout << "The vector is not strictly increasing." << std::endl;

    return 0;
}
```



### 2.2 不带谓词的版本

> **不带谓词时**，`std::adjacent_find()` 会自动检查每对相邻的元素，看它们是否**相等**。
>
> 只要找到两个**相邻且相等**的元素，搜索就停止，并返回指向第一个元素的迭代器。
>
> 如果整个范围内没有相邻相等的元素，返回的迭代器是 `end()`，表示查找失败

```c++
int main() {
    std::vector<int> vec = {1, 2, 3, 3, 5, 6};

    // 不带谓词的 adjacent_find 查找相邻相等的元素
    auto it = std::adjacent_find(vec.begin(), vec.end());

    if (it != vec.end()) {
        std::cout << "First adjacent pair of equal elements found at position: "
                  << std::distance(vec.begin(), it) << std::endl;
        std::cout << "Element: " << *it << std::endl;
    } else {
        std::cout << "No adjacent equal elements found." << std::endl;
    }

    return 0;
}
```

```shell
# vec 中的元素是 {1, 2, 3, 3, 5, 6}
First adjacent pair of equal elements found at position: 2
Element: 3
```

