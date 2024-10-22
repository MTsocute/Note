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



## 2. 类

----

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

```c++

```



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

