# C/C++ 语法

1. [move 运算符的作用](https://www.bilibili.com/video/BV16F411C7bd?spm_id_from=333.788.player.switch&vd_source=b47817c1aa0db593f452034d53d4273a&p=2)

<br>

## 1. 基础用法

---

### 1.1 C++ 使用 using 代替 typedef

> 在老版本的 C 中 `typedef` 使用起来其实有点不直观
>
> 虽然知道是 int 取名叫 myint，但是和我们平常的变量赋值就是有点出入

```c++
typedef int myint;
```

> 如果是用 using 的话，就和变量一样直观了

```c++
using myint = int;
```

### 1.2 `constexpr`

> `constexpr` ：用于修饰常量，确保这些变量的值在编译时就确定
>
> 对于可预估的结果，可以使用它

```c++
constexpr int a = 10;
constexpr int b = a + 20;  // 编译期计算
```

```c++
constexpr int square(int x) { return x * x; }

int main() {
    constexpr int result = square(5);  // 编译期计算
    int runtime_value = 10;	// 可以变，不可预估
    int runtime_result = square(runtime_value);  // 运行时计算
}
```

> `if constexpr` 经常配合**判断模板类型函数**一起使用

```c++
template<typename T>
void process(T value) {
    // 这个函数的返回值不是动态的，所以可以提前使用
    if constexpr (std::is_pointer<T>::value) {
        std::cout << "Pointer value: " << *value << std::endl;  // 仅在 T 是指针时才编译
    } else {
        std::cout << "Non-pointer value: " << value << std::endl;
    }
}

int main() {
    int x = 10;
    process(x);         // 非指针
    process(&x);        // 指针
    return 0;
}
```

> 不可以使用的情况

```c++
int x = 10;
if constexpr (x > 5) {   // 编译错误，x 不是编译期常量
    std::cout << "x is greater than 5" << std::endl;
}
```

### 1.3 数据类型的转换

| 转换类型               | 适用场景                           | 类型安全性       | 常见用途                          |
| ---------------------- | ---------------------------------- | ---------------- | --------------------------------- |
| **隐式转换**           | 类型兼容的场景                     | 高               | int -> double 等基本类型之间      |
| **C 风格转换**         | 任意类型之间，缺乏类型检查         | 低               | 较旧的 C/C++ 代码中常见           |
| **`static_cast`**      | 基本类型转换、类层次结构中的转换   | 高（编译时检查） | int <-> double, 类的向上/向下转换 |
| **`dynamic_cast`**     | 多态类型之间的转换，运行时检查     | 高（运行时检查） | 多态对象的基类到派生类转换        |
| **`const_cast`**       | 添加或移除 `const` 限定符          | 高               | 修改 `const` 对象                 |
| **`reinterpret_cast`** | 在不相关的指针类型或整数类型间转换 | 低               | 底层操作，如处理指针类型的转换    |

#### 1.3.1 C 自带风格

```c++
// 隐式转换
void implicit_conversion() {
    int a = 10;
    double b = a;  // 隐式转换 int -> double
    cout << "隐式转换: " << b << endl;
}

// C 风格转换
void c_style_conversion() {
    double a = 10.5;
    int b = (int)a;  // C 风格转换 double -> int
    cout << "C 风格转换: " << b << endl;
}

```

#### 1.3.2 static_cast

> 它通常用于基本类型之间的转换，*或者在继承体系中执行向上转换（基类指针/引用转换为派生类指针/引用）*

```c++
// 基本类型之间的转换
int a = 100;
double b = static_cast<double>(a);  // int -> double
cout << "int 转 double: " << b << endl;
```

#### 1.3.3 reinterpret_cast

> 用于在指针和其他非相关类型之间的转换。它不执行任何类型检查，通常用于底层指针操作

```c++
    int a = 65;
    // int 转换为 char 指针
    char* charPtr = reinterpret_cast<char*>(&a);
    cout << "reinterpret_cast: " << *charPtr << endl;  // 可能输出 'A'，因为 65 的 ASCII 值是 'A'

    // 指针和整数之间的转换
    void* voidPtr = reinterpret_cast<void*>(&a);
    int* intPtr = reinterpret_cast<int*>(voidPtr);
    cout << "reinterpret_cast 逆向转换: " << *intPtr << endl;
```

```shell
reinterpret_cast: A
reinterpret_cast 逆向转换: 65
```

#### 1.3.4 const_cast

```c++
// 移除 const 限定符
const int a = 10;
int* b = const_cast<int*>(&a);  // 移除 const
*b = 20;  // 修改 a 的值，这可能导致未定义行为
cout << "const_cast 之后修改值: " << *b << endl;

// 应用场景：修改函数中 const 参数
void modify_const_value(const int* p) {
    int* non_const_p = const_cast<int*>(p);
    *non_const_p = 30;
}

int x = 100;
modify_const_value(&x);
cout << "通过函数修改 const 参数: " << x << endl;
```

```sh
const_cast 之后修改值: 20
通过函数修改 const 参数: 30
```

### 1.4 联合体和结构体

![image-20250509131143317](https://raw.githubusercontent.com/MTsocute/New_Image/main/img/image-20250509131143317.png)

> 使用场景

```c
#include <stdio.h>
#include <string.h>

// 定义不同的数据包类型
typedef enum {
    TEMPERATURE,
    LOCATION,
    STATUS
} PacketType;

// 温度数据包结构
typedef struct {
    float temp_c;
    int timestamp;
} TemperaturePacket;

// 位置数据包结构
typedef struct {
    float latitude;
    float longitude;
    int timestamp;
} LocationPacket;

// 状态数据包结构
typedef struct {
    char status_code[4];
    int timestamp;
} StatusPacket;

// 使用union整合所有数据包类型
typedef union {
    TemperaturePacket temp;
    LocationPacket loc;
    StatusPacket status;
} DataPacket;

// 完整的网络数据包结构（类型标记 + union数据）
typedef struct {
    PacketType type;
    DataPacket data;
} NetworkPacket;

// 处理数据包的函数
void process_packet(NetworkPacket packet) {
    switch (packet.type) {
        case TEMPERATURE:
            printf("温度: %.1f°C, 时间: %d\n",
                   packet.data.temp.temp_c,
                   packet.data.temp.timestamp);
            break;
        case LOCATION:
            printf("位置: (%.2f, %.2f), 时间: %d\n",
                   packet.data.loc.latitude,
                   packet.data.loc.longitude,
                   packet.data.loc.timestamp);
            break;
        case STATUS:
            printf("状态码: %s, 时间: %d\n",
                   packet.data.status.status_code,
                   packet.data.status.timestamp);
            break;
    }
}

int main() {
    // 模拟接收温度数据包
    NetworkPacket temp_packet;
    temp_packet.type = TEMPERATURE;
    temp_packet.data.temp = (TemperaturePacket){25.5, 1623456789};
    process_packet(temp_packet);

    // 模拟接收位置数据包
    NetworkPacket loc_packet;
    loc_packet.type = LOCATION;
    loc_packet.data.loc = (LocationPacket){39.9042, 116.4074, 1623456790};
    process_packet(loc_packet);

    return 0;
}
```



### 1.5  位运算

| 运算符 | 中文名称 | 英文名称  | 作用                                                         |
| :----: | -------- | ---------- | :----------------------------------------------------------: |
| `&`    | 按位与   | AND        | 对应位都为 `1` 时，结果为 `1`。                              |
| `|`    | 按位或   | OR         | 只要有一个对应位为 `1`，结果为 `1`。                         |
| `^`    | 按位异或 | XOR        | 对应位不同则结果为 `1`。                                     |
| `~`    | 按位取反 | NOT        | 将每一位取反。                                               |
| `<<`   | 左移     | / | 将所有位向左移动指定的位数，右边补 `0`，相当于乘以 `2^n`。   |
| `>>`   | 右移     | / | 将所有位向右移动指定的位数，有符号数会保留符号位，无符号数左边补 `0`，相当于除以 `2^n`。 |

### 1.6 `std::cerr`

> 这个和 `std::cout` 很像，但是不一样的是，他们虽然是输出内容，但是代表的是错误的输出，所以一般显示的都是 $\textcolor{red}{红色}$ 的

```cpp
void error_handling(const std::string &message) {
    // std::cerr 内容的输出
    std::cerr << message << std::endl;
    std::exit(1);
}
```

> C 语言的话也可以实现一样的方法

```cpp
void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
```

### 1.7 错误的一般处理

```cpp
void Client::open_chat_box() {
    // 错误捕捉的范围
    try {
        this->chat = new Form(this->socket);
        chat->show();
    } 
    // 任意错误类型的捕捉（可以具体到那种，但是看文档，需要一定的开发经验）
    catch (const std::exception &e) {
        // 捕获标准异常并显示详细的错误信息
        QMessageBox::critical(this, "Error", "打开聊天框时出现错误: " + QString(e.what()));
    } 
    catch (...) {
        // 捕获其他所有异常
        QMessageBox::critical(this, "Error", "打开聊天框时发生了未知错误！");
    }
}
```

<br>

### 1.8 Time 时间库（C）

---

#### 1. 实现程序记时

> `CLOCKS_PER_SEC` 转换 time 类型的数据为对应的秒数

```c
#include<stdio.h>
#include<time.h>

int main()
{
    clock_t start_t,finish_t;
    double total_t = 0;
    int i = 0;
    start_t = clock();
    for(;i<100000;++i)
    {
        //do someting;
    }
    finish_t = clock();
    total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;//将时间转换为秒

    printf("CPU 占用的总时间：%f\n", total_t);
    return 0;
}
```

#### 2. 常用的函数和对应的逻辑关系

- 常用的函数

| **函数**                                                     | **作用**                               |
| :----------------------------------------------------------- | -------------------------------------- |
| time_t time(time_t*);                                        | 获取系统时钟                           |
| struct tm* gmtime(const time_t*);                            | 秒计数器转换为日期时间（格林尼治时间） |
| struct tm* localtime(const time_t*);                         | 秒计数器转换为日期时间（当地时间）     |
| time_t mktime(struct tm*);                                   | 日期时间转换为秒计数器（当地时间）     |
| char* ctime(const time_t*);                                  | 秒计数器转换为字符串（默认格式）       |
| char* asctime(const struct tm*);                             | 日期时间转换为字符串（默认格式）       |
| size_t strftime(char*, size_t, const char*, const struct tm*); | 日期时间转换为字符串（自定义格式）     |

- 数据类型转换的关系

![image-20250120165039147](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250120165039147.png)

### 1.9 `mutable` 

> const 成员函数不可以修改的成员变量，但是有些时候如果一定要改某些值，而且还与这个函数高度绑定的时候，有不想写新的函数，那么 `mutable` 修饰就是一个不错的选择

```cpp
class Logger {
public:
    void log() const {
        log_count++; // ❌ 错误：不能修改成员变量
    }

private:
    int log_count = 0;
};

```

## 2. 泛化

---



<br>

## 4. 文件操作

---

### 4.1 `open` 函数的文件描述符

> `open` 函数的文件状态描述

![image-20241130162353315](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241130162353315.png)

> 当然这个在 C++ 中有更好的写法，所以后面我们会再优化

```cpp
// 创建一个文件，有什么要求就看上面的表
fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
```

### 4.2 C++ 17 `filesystem` 库

> 这个头文件对一些常规的文件处理更加的好用，所以在这里回有很大的介绍
>
> 这里我们默认你做了这些基础的配置

```cpp
#include <filesystem>
#include <fstream>		// 文件操作的类

namespace fs = std::filesystem;		// 简化命名空间
```



#### 4.2.1 CPP 读写文件的操作

> ### 读文件的操作
>
> 这个 `is_open()` 不需要文件是否存在与否，保证文件能够正常打开就行

```cpp
std::string file_path = dirPath + "/data.txt";  // 文件的完全路径
std::string data_to_write = "fuck you linux";   // 写入的内容

// 在对应路径创建一个文件并写入一些内容
std::ofstream outFile(file_path,
    std::ios::out | std::ios::trunc);

// 写入内容
if (outFile.is_open()) {
    outFile << data_to_write;
    outFile.close();
}

else { std::cerr << "Failed to open the file" << std::endl; }
```

> ### 读文件的操作

```cpp
#include <fstream>

int main() {
    std::ifstream inFile("data.txt"); // 打开文件

    if (!inFile) {  // 检查文件是否打开成功
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    std::string line;
    // 按行读取文件内容
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;  // 输出每一行
    }

    inFile.close();  // 关闭文件
    return 0;
}
```

<br>

#### 4.2.2 创建一个文件夹

> 使用 `fs::create_directory` 创建单一目录
>
> 使用 `fs::create_directories` 创建多层目录（如果父目录不存在，会一并创建）

#### 4.2.3 `path`的一些操作

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

### 4.3 IO 流

> 我们一般用的类就是继承了 `iostream` 的部分, 因为他们这些可以对输入输出都有操作
>
> 但是他们服务的对象不同 : 
>
> 1. `iostream`: 标准输入输出
> 2. `fstream`: 文件对象
> 3. `stringstream`: 字符

![在这里插入图片描述](https://i-blog.csdnimg.cn/blog_migrate/0aec288d4cfe78b0462954e833fc9cb1.png)

#### 1. stringstream 使用场景

- 按分隔符切割字符串

```cpp
std::string input = "one two three four";
std::stringstream ss(input);
std::string word;

while (std::getline(ss, word, ' ')) {  // 按空格分割
    std::cout << word << std::endl;
}
```

- 从流中读取多种数据类型

```cpp
std::string input = "123 456.78 hello";
std::stringstream ss(input);

int int_part;
double double_part;
std::string str_part;

ss >> int_part >> double_part >> str_part;  // 读取不同类型的数据

std::cout << "Integer: " << int_part << std::endl;
std::cout << "Double: "  << double_part << std::endl;
std::cout << "String: "  << str_part << std::endl;
```



<br>

## 5. 函数式编程

---

### ==1. 函数指针==

> 函数指针的声明

```C++
/*
	int -> 返回类型
	func_ptr -> 指针名字
	(int, int) -> 包含的参数
*/
int (*func_ptr)(int, int);
```

> 基本使用

```c
int add(int a, int b) { return a + b; }

int main() {
    // 声明并初始化函数指针
    int (*func_ptr)(int, int) = add;

    // 使用函数指针调用函数
    int result = func_ptr(3, 4);
    printf("Result: %d\n", result);  // 输出：Result: 7

    return 0;
}
```

#### 1. 函数指针传参

> `typedef int (*Operation)(int, int);`
>
> `int(*) (int, int) -> Operation`，**如果是 C++ 的话就是**：`using Operation = int(*)(int, int);`

```C
typedef int (*Operation)(int, int);

const int add(int a, int b) { return a + b; }

const int multiply(int a, int b) { return a * b; }

// 使用函数指针类型作为参数
void operate(int x, int y, Operation op) {
    printf("Result: %d\n", op(x, y));
}

int main() {
    operate(5, 3, add);       // 输出 Result: 8
    operate(5, 3, multiply);  // 输出 Result: 15
    return 0;
}
```

#### 2. 函数返回类型为函数指针

> 不声明的写法有点糟糕, 但是还是要认识一下

```cpp
// 定义一个具体的函数
int myFunction(int x) { return x * x; }

// 返回值是函数指针的函数
int (*getFunction())(int) {
	return myFunction;
}
```

> 推荐下麦呢的做法

```c
typedef int(*my_func)(int);		// 把一个函数指针作为一个类型用, 不然容易看不懂

// 返回值是函数指针的函数
my_func getFunction() {
    return myFunction;
}

int main() {
    // 获取函数指针
    int (*funcPtr)(int) = getFunction();	// 函数返回是一个函数指针

    // 调用通过函数指针返回的函数
    printf("Result: %d\n", funcPtr(5)); 	// 输出 25

    return 0;
}
```

### 2. `std::fucntion`

> 这个就是 CPP 版本的函数指针

```cpp
int add(const int a, const int b) { return a + b; }


int main() {
    // 大多数外部的这个 function<> 没有存在感, 因为可以 auto
    function<int(int, int)> add_func_P = add;
    std::cout << std::format("Ans = {}\n", add_func_P(20, 440));
}
```



#### 1. 声明一个函数指针类型

```cpp
int add(const int a, const int b) { return a + b; }

// 这里声明了一个函数指针类型
using my_int_func = function<int(int, int)>;

int main() {
    // 使用类型声明
    my_int_func add_func_P = add;
    std::cout << std::format("Ans = {}\n", add_func_P(220 440));
}
```

<br>

#### 2. 函数指针作为参数传递

```cpp
int add(const int a, const int b) { return a + b; }

using my_int_func = function<int(int, int)>;

// 函数指针作为参数传递
int demo1(const my_int_func &op, int a, int b) {	// 注意这里的函数指针 & 用法
    return op(a, b);
}

int main() {
    std::cout << std::format("Ans = {}\n", op(add, 202, 440));
}
```

#### 3. 函数返回类型为函数指针

```cpp
using my_int_func = function<int(int, int)>;
int add(const int a, const int b) { return a + b; }

// 函数指针类型作为函数类型
my_int_func getFunction() { return add; }

int main() {
    function<int(int, int)> op = getFunction();
    std::cout << std::format("Ans = {}", op(10, 20)) << std::endl;
    return 0;
}
```

### 3. `bind()`

> 它的作用是将一个可调用对象（比如函数、成员函数或 Lambda）和其参数绑定在一起，生成一个新的可调用对象。这个新的可调用对象可以在之后的某个时刻被调用，**而且它的参数可以部分预先绑定**。
>
> - 固定某些参数的值
> - 重新排列参数顺序
> - 将成员函数绑定到特定的对象上

- 普通函数的绑定

> 要注意, 这里这个占位符 不是说 10 占据了 1 这个位置,
>
> 而是 _1 占据了 b 的位置, 这个 b 可以用于传入别的参数

```cpp
// 使用 std::bind 绑定 add 函数，将第一个参数绑定为 10
constexpr int add(const int a, const int b) { return a + b; }
auto bound_add = std::bind(add, 10, std::placeholders::_1);
	
// 调用时只需要提供第二个参数
std::cout << bound_add(20) << std::endl;  // 输出 30
```

- 类对象成员函数的绑定

```cpp
class MyClass {
public:
    int multiply(int a, int b) { return a * b; }
};

int main() {
    MyClass obj;

    // 将成员函数 multiply 绑定到对象 obj 上，固定 a 为 5
    auto bound_multiply = std::bind(&MyClass::multiply, &obj, 5, std::placeholders::_1);

    std::cout << bound_multiply(4) << std::endl;  // 输出 20

    return 0;
}
```

<br>
