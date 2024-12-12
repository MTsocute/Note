# C/C++ 语法

---

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

### ==1.4 typdef + c  函数指针==

---

> 说实话真的不知道，C 语言也存在函数指针，C ++ 还是太便利了

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

> **使用 typdef 申明某一类函数指针，作为函数的参数使用，实现函数编程**
>
> `typedef int (*Operation)(int, int);`
>
> `int(*) (int, int) -> Operation`，如果是 C++ 的话就是：`using Operation = int(*)(int, int);`

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

#### ==2. 让一个函数返回类型为函数指针==

```c
// 定义一个具体的函数
int myFunction(int x) { return x * x; }

// 返回值是函数指针的函数
int (*getFunction(void))(int) {
    return myFunction; // 返回函数的地址
}

int main() {
    // 获取函数指针
    int (*funcPtr)(int) = getFunction();	// 函数返回是一个函数指针

    // 调用通过函数指针返回的函数
    printf("Result: %d\n", funcPtr(5)); 	// 输出 25

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

## 2. 类

----

> 对象编程（OOP）中的三个基本特性之一（封装、继承、多态）

### 2.1 delete 禁止某些成员函数

> `delete` 关键字不仅仅限制在**操作符函数**上，也可以用来删除任何**特殊成员函数**，包括普通成员函数

```c++
class MyClass {
public:
    MyClass() = default;
    
    // 禁止拷贝构造
    MyClass(const MyClass&) = delete;
    
    // 禁止赋值操作符
    MyClass& operator=(const MyClass&) = delete;
};
```

> 删除**特殊成员函数**

```c++
class MyClass {
public:
    void doSomething(int) = delete;   // 禁止带有 int 参数的 doSomething 函数
    void doSomething(double) {}       // 允许带有 double 参数的 doSomething 函数
};
```

> 禁止**拷贝和移动构造一个新的类**

```cpp
TCP_Server(const TCP_Server &other) = delete;
TCP_Server operator=(const TCP_Server &other) = delete;
TCP_Server(TCP_Server&&) = delete;  // 禁止移动构造
TCP_Server& operator=(TCP_Server&&) = delete;  // 禁止移动赋值
```

### 2.2 类的静态成员函数

> #### 什么时候使用静态成员函数？
>
> - **无状态操作**：如果函数执行的操作与特定对象无关，不需要访问对象的成员变量或非静态成员函数，那么它可以被声明为静态函数。

```c++
class MathUtils {
public:
    // 静态成员函数，可以在不实例化对象的情况下调用
    static int add(int a, int b) {
        return a + b;
    }

    // 非静态函数，需要对象实例才能调用
    int multiply(int a, int b) {
        return a * b * rd;
    }
private:
    int rd = rand() % 100;
};

int main() {
    // 调用静态成员函数，不需要创建 MathUtils 的实例
    std::cout << "3 + 4 = " << MathUtils::add(3, 4) << std::endl;

    // 需要创建对象才能调用非静态成员函数
    MathUtils utils;
    std::cout << "3 * 4 = " << utils.multiply(3, 4) << std::endl;

    return 0;
}
```

> 类内静态函数是独一份，不论生成了多少个类，公用的都是同一个
>
> 下面这个案例，你会发现输出地址都是一样的

```c++
class Demo {
public:
    // 静态成员函数
    static constexpr int add(int a, int b) { return a + b; }

    // 输出 add 函数的地址
    void printAddAddress() {
        std::cout << "Address of add from instance: " << 
            reinterpret_cast<void*>(&Demo::add) << std::endl;
    }
};

int main() {
    Demo demo1;
    Demo demo2;

    // 比较地址
    demo1.printAddAddress();
    demo2.printAddAddress();

    // 直接获取静态函数地址
    std::cout << "Address from Demo::add: " <<
        reinterpret_cast<void*>(&Demo::add) << std::endl;

    return 0;
}

```

### 2.3 explicit

> 避免隐式的生成一个类

```c++
class MyClass {
public:
    // 没有 explicit 的构造函数
    MyClass(int x) {
        std::cout << "MyClass constructor called with x = " << x << std::endl;
    }
};

int main() {
    MyClass obj = 42; // 隐式调用构造函数
}
```

<br>

### 2.4 类的类型转换

---

#### 2.4.1 向上转换

```c++
// 基类和派生类之间的转换（向上转换）
class Base {
public:
    virtual void show() { cout << "Base class" << endl; }
};

class Derived : public Base {
public:
    void show() { cout << "Derived class" << endl; }
};

Derived derivedObj;
// Derived* -> Base*: 派生转换基类
Base* basePtr = static_cast<Base*>(&derivedObj);  
// 使用派生对象转换基类，使用基类的方法
basePtr->show();
```

```c++
Base* base = new Derived();

// 基类指针到派生类指针的转换
Derived* derived = dynamic_cast<Derived*>(base);

if (derived) {
    cout << "dynamic_cast 成功: ";
    derived->show();
} 
else cout << "dynamic_cast 失败" << endl;

delete base;
```

#### 2.4.2 向下转换

> `dynamic_cast` 主要用于具有虚函数的多态类型的指针或引用之间的转换，尤其适用于**基类和派生类之间的安全向下转换**。
>
> 如果转换失败，指针会返回 `nullptr`，引用则抛出异常

```c++
// 非多态类型的转换示例
Base* nonPolymorphic = new Base();
// 基类转换到派生类（向下转换）
derived = dynamic_cast<Derived*>(nonPolymorphic);

if (derived) derived->show();
else cout << "dynamic_cast 对非多态类型转换失败" << endl;
delete nonPolymorphic;
```

### 2.5 有元函数

> 友元函数可以访问类中的私有数据和方法，这样可以在不暴露这些成员的情况下提供特定功能。
>
> 但是是要把类对象作为参数导入的时候哈

```c++
class MyClass; // 前向声明

class FriendClass {
public:
    void display(MyClass &obj); // 声明友元函数
};

class MyClass {
private:
    int value;

public:
    MyClass(int val) : value(val) {}

    // 声明 FriendClass 为友元
    friend class FriendClass; 
};

void FriendClass::display(MyClass &obj) {
    cout << "MyClass 的私有值: " << obj.value << endl; // 访问私有成员
}

int main() {
    MyClass myObj(42);
    FriendClass friendObj;
    friendObj.display(myObj); // 调用友元函数

    return 0;
}
```



### 2.6 `virtual`

---

#### 1. 基本概念

> 虚函数的关键点是**动态绑定**（也叫运行时绑定）。
>
> 在没有 `virtual` 的情况下，C++ 的函数调用是**静态绑定**（也叫编译时绑定），即编译器在编译时就确定了调用哪个函数。而 `virtual` 会告诉编译器，**函数调用要在运行时根据对象的实际类型来确定**

```cpp
#include <iostream>
using namespace std;

class Animal {
public:
    virtual void makeSound() {  // 虚函数
        cout << "Animal makes sound" << endl;
    }
};

class Dog : public Animal {
public:
    void makeSound() override {  // 重写虚函数
        cout << "Dog barks" << endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() override {  // 重写虚函数
        cout << "Cat meows" << endl;
    }
};

void playSound(Animal* animal) {
    animal->makeSound();  // 动态绑定
}

int main() {
    Dog dog;
    Cat cat;

    playSound(&dog);  // 输出：Dog barks
    playSound(&cat);  // 输出：Cat meows

    return 0;
}
```

#### 2. 虚函数的规则

**必须通过指针或引用调用**：如果直接通过对象调用，仍然是静态绑定。

**重写时函数签名必须一致**：

- 参数和返回值类型必须匹配。
- 可以使用 `override` 关键字来明确地表明重写行为。

```cpp
void display() override;  // 推荐使用
```

**虚函数只能存在于类中**：全局函数不能声明为虚函数

**如果一个类有虚函数，其析构函数通常也应声明为虚函数**：避免通过基类指针删除派生类对象时资源释放不完全的问题。

```cpp
class Base {
public:
    // 析构函数声明为虚类
    virtual ~Base() { std::cout << "Base destroyed\n"; }
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "Derived destroyed\n"; }
};
```

#### 3. 纯虚函数

> 如果一个类中的虚函数不需要具体实现，可以声明为 **纯虚函数**，用 `= 0` 表示
>
> 带有纯虚函数的类是**抽象类**，不能直接实例化。它通常用作接口，让派生类实现特定功能。

```cpp
class Payment {
public:
    virtual void process() = 0;  // 纯虚函数，提供统一接口
};

class CreditCard : public Payment {
public:
    void process() override {
        cout << "Processing credit card payment" << endl;
    }
};

class PayPal : public Payment {
public:
    void process() override {
        cout << "Processing PayPal payment" << endl;
    }
};

// 客户端代码
void checkout(Payment* payment)	// 动态绑定 
{
    payment->process();  // 统一调用接口
}

int main() {
    CreditCard cc;
    PayPal pp;

    checkout(&cc);  // 输出：Processing credit card payment
    checkout(&pp);  // 输出：Processing PayPal payment
    return 0;
}
```

### 2.7 类成员函数 const 

> 我们的成员函数是可以访问类的成员变量，有了这个 const 声明的成员函数是不可以修改类变量的

```cpp
class Client : public QWidget {
public:
    explicit Client(QWidget *parent = nullptr);
    ~Client() override;

private slots:
    // 你会发现所有的成员函数的后面都有 const
    void clicked_on_connect_button() const;
    void open_chat_box() const;

private:
    Ui::Client *ui;
    Form *chat;
    QTcpSocket *socket;
};
```

### 2.8 父类的如果没有无参的默认构造的继承注意事项

> 在 C++ 中，如果一个子类继承了一个父类，并且父类没有默认构造函数（即没有不带参数的构造函数），那么子类的构造函数确实需要显式地调用父类的构造函数并传递适当的参数。

```cpp
class Parent {
public:
    Parent(int x) { 
        // 带参数的构造函数
    }
};

class Child : public Parent {
public:
    // 在实现自己构造的同时并需要实现父类的默认构造函数
    Child(int x) : Parent(x) {  // 必须显式调用父类构造函数并传递参数
        // 子类的构造函数
    }
};

```

> 所以 `QT` 很多的时候都一个 `parent` 参数

```cpp
// 继承于 QMainWindow 所以要实现其父类的构造函数传入 parent
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
	...
}
```



<br>

## 3. 设计模式

----

### 3.1 类创建实例的方式

> 在认识单例模式之前，先看下一个类有哪些方式创建一个实例

```c++
class MyClass {
public:
    MyClass() { std::cout << "Constructor called" << std::endl; }
    ~MyClass() { std::cout << "Destructor called" << std::endl; }
};
```

#### **1. 自动分配（栈分配）**

```c++
int main() {
    MyClass obj;  // 在栈上分配对象
    return 0;     // 作用域结束时 obj 自动销毁
}
```

#### **2. 动态分配（堆分配）**

```c++
int main() {
    MyClass* obj = new MyClass();  // 在堆上分配对象
    // 使用对象
    delete obj;  // 手动销毁对象，释放内存
    return 0;
}

// 智能指针版本
int main() {
    std::unique_ptr<MyClass> obj = std::make_unique<MyClass>();  // 自动管理生命周期
    // 不需要手动 delete
    return 0;
}
```

#### 3. 拷贝构造函数

> 对于比较简单的类，自带的拷贝构造能做到，除非你的数据类型不简单

```c++
class MyClass {
public:
    int x;
    MyClass(int val) : x(val) {}
};

int main() {
    MyClass obj1(10);  // 创建对象 obj1
    MyClass obj2 = obj1;  // 使用拷贝构造函数创建 obj2
    return 0;
}
```

> 譬如下面这个

```c++
class MyClass {
public:
    // 构造函数
    MyClass(int val) : data(new int(val)) {}

    // 自定义拷贝赋值运算符
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {  // 防止自我赋值
            delete data;  // 释放旧资源
            data = new int(*(other.data));  // 深拷贝
        }
        return *this;
    }

    ~MyClass() {
        // 释放动态内存
        delete data;  
    }
private:
    int* data = nullptr;
};

int main() {
    MyClass obj1(10);
    MyClass obj2(20);

    obj2 = obj1;  // 调用拷贝赋值运算符
    return 0;
}
```

> **智能指针版本**

```c++
class MyClass {
public:
    // 构造函数，使用 std::make_unique 创建 unique_ptr
    MyClass(int val) : data(std::make_unique<int>(val)) {}

    // 自定义拷贝赋值运算符
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {  // 防止自我赋值
            data = std::make_unique<int>(*(other.data));  // 深拷贝
        }
        return *this;
    }

// 使用默认析构函数即可，unique_ptr 会自动释放内存
private:
    std::unique_ptr<int> data;  // 使用 unique_ptr 代替裸指针
};

int main() {
    MyClass obj1(10);
    MyClass obj2(20);

    obj2 = obj1;  // 调用拷贝赋值运算符
    return 0;
}
```

#### 4. 移动构造函数

> 在 C++11及以后的版本中，**移动构造函数**允许通过**移动语义**来创建对象，避免不必要的拷贝，提高效率。
>
> **是升级版本的拷贝构造函数**
>
> 移动构造函数通常在涉及临时对象或右值引用时使用

```c++
class MyClass {
public:
    MyClass() {}
    // 右值运算符接受 std::move() 变量
    MyClass(MyClass&& other) { /* 这里执行移动逻辑 */ }
};

int main() {
    MyClass obj1;
    // 相当于 MyClass obj2 (std::move(obj1));
    MyClass obj2 = std::move(obj1);  // 使用移动构造函数
    return 0;
}
```



### 3.2单例模式

> 所以你会发现，他和前面几个最大的不同，就是他虽然也是函数返回一个类别，但不是通过默认的几个
>
> 而是自己用函数在里面创建了一个静态实例然后再返回
>
> 因为类内使用 static 所以，返回的对象其实都是同一个

```c++
class Singleton {
public:
    // 获取单例实例的函数
    static Singleton& get_instance() {
        static Singleton instance;  // 静态局部变量，确保只初始化一次
        return instance;
    }

    // 禁止拷贝构造和赋值操作
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    void show() {
        std::cout << "Singleton instance at address: " << this << std::endl;
    }

private:
    // 私有构造函数，防止外部创建对象
    Singleton() = default;
};

int main() {
    // 通过 get_instance 获取单例对象
    Singleton& instance1 = Singleton::get_instance();
    Singleton& instance2 = Singleton::get_instance();

    // 显示两个实例的地址
    instance1.show();  // 输出：Singleton instance at address: 0x<some address>
    instance2.show();  // 输出：Singleton instance at address: 0x<same address>

    return 0;
}
```

**一般的两种模式**

> ### **懒汉模式（Lazy Initialization Singleton）**
>
> 在懒汉模式下，实例的创建是**延迟**的，只有在第一次访问实例时才会创建。这种方式的优点是资源节约：如果从未使用这个实例，就不会浪费内存或其它资源
>
> ### **饿汉模式（Eager Initialization Singleton）**
>
> 在饿汉模式下，实例在程序启动时就**立即**被创建，不管后面是否会用到。这种方式可以避免多线程问题，因为实例在程序一开始就存在。

```c++
class log {
public:
    // 禁止拷贝构造和复制构造函数 --> 不通过他们这些方式，只有函数返回
    log(const log & log) = delete;
    log & operator=(const log & log) = delete;

   /* 注意这里我们是通过一个类函数直接返回一个实例 */
    
    // 这里是 *饿汉模式*
    static log & get_instance1() {
        static log * instance = nullptr;	// 一开始案例就存在，不管有没有给分配空间

        if (!instance) instance = new log();

        return *instance;
    }

    // 这里是 *懒汉模式*
    static log & get_instance2() {
        // 一开始就创建出了实例
        static log _log;		// 这个需要构造函数，才能写哈
        return _log;
    }
private:
	log() = default;
};
```

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



<br>

## Ex. CMakeLists 编写

---

> 如果使用的环境是 `Clion` 或者 正在苦恼于 `makefile` 的冗杂的话，试试看使用 `CMakeLists.txt` 来管理你的程序吧

<br>

### ==1.  CmakeLists + vkpkg 导入第三方库的正确用法==

> - `target_link_libraries`：这个是针对**第三方库头文件**对应的 `include` 路径

```cmake
cmake_minimum_required(VERSION 3.15)

set(CMAKE_PREFIX_PATH  "D:/vcpkg/installed/x64-mingw-dynamic")	# 找到 vcpkkg 安装目录

# 项目名称
project(EigenExample)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 查找 Eigen3 库
find_package(Eigen3 CONFIG REQUIRED)

if (Eigen3_FOUND)
    message(STATUS "Eigen3 Found at: ${EIGEN3_INCLUDE_DIRS}")

    # 添加可执行文件
    add_executable(${PROJECT_NAME} main.cpp)

    # 链接 Eigen3 库
    target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)
else()
    message(FATAL_ERROR "Eigen3 library not found. Please install it or check the path.")
endif()

```

> - `include_directories`：这个是针对**自定义头文件**链对应的 `include` 路径
>
> `file-tree`：
>
> ```shell
> .
> ├── CMakeLists.txt
> ├── cmake-build-debug
> ├── include (我们自己写的头文件)
> └── src
> ```

```cmake
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(First_Demo src/main.cpp
        src/window_demo.cpp
        src/window_demo.ui
        include/window_demo.h
)

# 导入自己写的头文件库
target_include_directories(First_Demo
        PRIVATE "D:\\Code\\QT\\First_Demo\\include"
)

# 导入第三方库
target_link_libraries(First_Demo
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
```

### 2. 根据不同的 OS 来设置 

```cmake
cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

project(OS)

# 根据操作系统设置
if (WIN32)
    message(STATUS "Configuring for Windows")
elseif (APPLE)
    message(STATUS "Configuring for macOS")
else ()
    message(STATUS "Configuring for other OS")
endif ()
```

### 3. 在 C++ 环境中使用 Python

```cmake
# 找到 Python3
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Python3 支持
add_executable(python_demo python_in_cpp.cpp)
target_link_libraries(python_demo Python3::Python)
```

> 简单的使用 Python 的程序

```cpp
#include <Python.h>
#include <iostream>

int main() {
    // 初始化 Python 解释器
    Py_Initialize();

    // 执行一些简单的 Python 代码
    const char* code = "print('Hello from Python!')";
    
    // 运行 Python 代码
    PyRun_SimpleString(code);

    // 终止 Python 解释器
    Py_Finalize();

    return 0;
}
```

> ### 在一个 C++ 使用 Python3 写好的程序并使用获取对应的返回值结果

- py 文件中的内容

```python
# cal.py
import os
import sys

def say_hello(something=""):
    return 114514
```

- 在 CPP 中调用 py 文件的函数并获取对应的返回值

```cpp
#include <Python.h>
#include <iostream>

int main() {
    // 初始化 Python 解释器
    Py_Initialize();

    // 检查 Python 是否初始化成功
    if (!Py_IsInitialized()) {
        std::cerr << "Python 初始化失败！" << std::endl;
        return -1;
    }

    try {
        // 动态添加路径到 sys.path
        std::string customPath = "D:/Code/QT/Calculator/script/";
        std::string pythonCode = "import sys; sys.path.append('" + customPath + "')";
        PyRun_SimpleString(pythonCode.c_str());

        // 导入 Python 模块 (cal.py)
        PyObject* pModule = PyImport_ImportModule("cal");
        if (!pModule) {
            std::cerr << "Unable to load 'cal'" << std::endl;
            PyErr_Print(); // 打印 Python 错误信息
            return -1;
        }

        // 获取 say_hello 函数
        PyObject* pFunc = PyObject_GetAttrString(pModule, "say_hello");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            std::cerr << "'say_hello' 函数不可用或不可调用" << std::endl;
            Py_XDECREF(pModule);
            return -1;
        }

        std::string expr = "1 + 1";
        // 构造参数
        PyObject* pArgs = PyTuple_Pack(1, PyUnicode_FromString(expr.c_str()));

        // 调用函数，并获取返回值
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

        // 检查返回值并获取结果
        if (pValue != nullptr) {
            long result = PyLong_AsLong(pValue);
            std::cout << "结果: " << result << std::endl;
            Py_DECREF(pValue);
        } else {
            std::cerr << "函数调用失败！" << std::endl;
            PyErr_Print(); // 打印 Python 错误信息
        }

        // 释放资源
        Py_XDECREF(pArgs);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);

    } catch (...) {
        std::cerr << "发生异常！" << std::endl;
    }

    // 关闭 Python 解释器
    Py_Finalize();
    return 0;
}
```

### 4. 指定头文件的搜索路径的写法

```cmake
# 添加头文件搜索路径
include_directories(
	# 头文件所在的路径
    include
    .pio/libdeps/STC89C52RC/include/
)
```

### 5. 多个不同地方的源文件的一起汇编处理

> 可以把全部的 **源文件** 和 **头文件** 分化为两个部分
>
> 但是为了区别模块的不同的功能，我们一般会采用分文件的写法，往往很可能出现多个文件存储源文件的情况，这个时候编写 `CMakeLists.txt` 的 `add_executable()` 总不能把一个个源文件全部都写出来吧，所有就有了下面的介绍

- `set()`：这个函数就是把所有的文件统一的这个整体取一个别名

```cmake
# 和 main.c 一起编译的源文件所在的地方
file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*" "Hardware/*.*")

# 使用别名统一汇编# 设置 AUTOUIC 搜索路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/UI)		# Source Directory: {D:/Code/QT/QT_Client}/UI

# 定义源文件和头文件和UI文件
set(SOURCE_FILES
        main.cpp
        src/client.cpp
        src/form.cpp
)

set(HEADER_FILES
        include/client.h
        include/form.h
)

set(UI_FILES
        UI/client.ui
        UI/form.ui
)

# 添加可执行文件
add_executable(Client
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
)
```

### 6.  CMake 位置参数的变量

> 小总结

| 变量名                     | 说明                                            |
| -------------------------- | ----------------------------------------------- |
| `CMAKE_SOURCE_DIR`         | 顶层 CMakeLists.txt 所在目录的绝对路径          |
| `CMAKE_BINARY_DIR`         | 顶层构建目录的绝对路径（通常是 `build` 文件夹） |
| `CMAKE_CURRENT_SOURCE_DIR` | 当前 CMakeLists.txt 所在目录的绝对路径          |
| `CMAKE_CURRENT_BINARY_DIR` | 当前目录的构建目录的绝对路径                    |

### 7. 第三方库的 'head-only'

> 你会发现你用 `vcpkg` 拿到的第三方库会有 `head-only 后缀` 和 `无后缀` 两个版本
>
> ### **`fmt::fmt`**
>
> - **描述**: 这是 `fmt` 库的常规用法，表示链接 `fmt` 的预编译库文件（通常是动态库或静态库）。
> - 实现方式: 编译时，`fmt` 的源代码已经被编译为 `.dll`（动态库）或 `.lib`（静态库），但是链接很多时候找库找不到，所以用起来很恼火
>
> ### **`fmt::fmt-header-only`**
>
> - **描述**: 这是 `fmt` 的头文件模式，所有实现代码都直接包含在头文件中。
> - 实现方式: 不需要链接任何预编译的库文件，功能完全通过头文件实现

- 所以对于一些第三方库，我们就直接导入 head-only 的版本

```cmake
cmake_minimum_required(VERSION 3.30)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(3rtParty "D:/vcpkg/installed/x64-mingw-dynamic")
set(CMAKE_PREFIX_PATH "${3rtParty}/include")

find_package(fmt CONFIG REQUIRED)

add_executable(untitled main.cpp)

target_link_libraries(untitled PRIVATE
        fmt::fmt-header-only
)
```



