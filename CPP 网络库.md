# $Boost::Asio$

---

> 虽然说我们需要理解 TCP/IP 套接字等逻辑，和如何实现多进程、线程、IO复用等服务器的，但是那是为了能写出非常好的架构实现服务器，对于大多数时候，我们自己造轮子还是不太合适的，所以学习一个成熟的框架是非常重要的，这个就是一个非常常用的 CPP 网络库

- 我们看右边的框图，我们需要知道两个部分
  1. 同步和异步，前面的部分都是一样的，创建 endpoint、socket share pointer、acceptor
  2. 同步是循环监听，每一个循环创建一个新的 socket 用于 accept 客户端
  3. 异步是异步监听，在异步函数中创建新的 socket 用于下一次事件

![https://cdn.llfc.club/1540562-20190417002428451-62583604.jpg](https://cdn.llfc.club/1540562-20190417002428451-62583604.jpg)

<br>

## 1. 同步和异步的区别

> **在同步编程中，所有的操作都是顺序执行的**，比如从socket中读取（请求），然后写入（回应）到socket中。**每一个操作都是阻塞的**。因为操作是阻塞的，所以为了不影响主程序，**当在socket上读写时，通常会创建一个或多个线程来处理socket的输入/输出**。因此，同步的服务端/客户端通常是多线程的
>
> 相反的，**异步编程是事件驱动的**。虽然启动了一个操作，但是你不知道它何时会结束；它只是提供一个回调给你，当操作结束时，它会调用这个API，并返回操作结果。**和 Qt 的信号与槽机制非常的像**。
>
> 所以我们其实可以把启动一个异步操作（async_connect`、`async_wait）这种注册了事件的行为理解为信号，然后我们注册信号的时候还需要往里面塞入函数，这个函数的满足条件后的调用理解为槽
>
> ```cpp
> acc.async_accept(*sock, boost::bind(handle_accept, sock, _1));
> ```
>
> 因为中途做改变会非常困难而且容易出错，所以你在项目初期（最好是一开始）就得决定用同步还是异步的方式实现网络通信。不仅API有极大的不同，你程序的语意也会完全改变（异步网络通信通常比同步网络通信更加难以测试和调试）。你需要考虑是采用阻塞调用和多线程的方式（同步，通常比较简单），或者是更少的线程和事件驱动（异步，通常更复杂）
>
> 注：如果后面代码中的 `Boost API` 看不懂的话也可以先问问 GPT，我们后面会具体说一些 API 的，主要是理解在干什么，API 就是满足这个功能的具体方法而已

### 1. 异步工作

> 默认情况下，**你是不知道每个异步`handler`的调用顺序的**
>
> 除了通常的异步调用（来自异步socket的读取/写入/接收）你可以使用`service.post()`来使你的**自定义方法被异步地调用**

```cpp
using namespace boost::asio;
io_service service;

void func(int i) { std::cout << "func called, i= " << i << std::endl; }

void worker_thread() { service.run(); }

int main(int argc, char* argv[]) {
    // 将任务提交给 io_service, 注意这里的 .post() 
    for ( int i = 0; i < 10; ++i) { service.post(boost::bind(func, i)); }
    boost::thread_group threads;		// 简单线程池
    // 三个线程
    for ( int i = 0; i < 3; ++i) threads.create_thread(worker_thread);
    // 等待所有线程被创建完
    boost::this_thread::sleep( boost::posix_time::millisec(500));
    threads.join_all();
}
```

> 你不能确定异步方法调用的顺序。它们不会以我们调用*post()*方法的顺序来调用。下面是运行之前代码可能得到的结果：

```sh
func called, i= 0
func called, i= 2
func called, i= 1
func called, i= 4
func called, i= 3
func called, i= 6
func called, i= 7
func called, i= 8
func called, i= 5
func called, i= 9
```

### 2. 异步工作的顺序执行

> 有时候你会想让一些**异步处理方法顺序执行**。比如，你去一个餐馆，下单，然后吃。你需要先去餐馆，然后下单，最后吃。这样的话，**你需要用到 `io_service::strand`，这个方法会让你的异步方法被顺序调用**

```cpp
using namespace boost::asio;
io_service service;

void func(const int i) { std::cout << "func called, i= " << i << std::endl; }

void worker_thread() { service.run(); }

int main() {
    io_service::strand strand_one(service), strand_two(service);
    // 前五个
    for (int i = 0; i < 5; ++i)
        service.post(strand_one.wrap([i] { func(i); }));

    // 后五个
    for (int i = 5; i < 10; ++i)
        service.post(strand_two.wrap([i] { func(i); }));

    boost::thread_group threads;

    for (int i = 0; i < 3; ++i)
        threads.create_thread(worker_thread);

    // 等待所有线程被创建完
    boost::this_thread::sleep(boost::posix_time::millisec(1000));
    threads.join_all();
}
```

> 在上述代码中，我们保证前面的 5个 线程和后面的 5个 线程是顺序执行的, 不是你预期的 0~9
>
> 但是前五个线程一定是先于后五个执行的, **所以无论你如何运行一定会有 0 运行在 5 之前的** (如果你缩短那个 sleep 时间的话, 其实还是会出现后面执行更快的现象)

### 3. 异步调用处理方法

> `post() VS dispatch() VS wrap()`
>
> - `service.post(handler)`：这个方法能确保其在请求`io_service`实例，然后调用指定的处理方法之后立即返回。`handler`稍后会在某个调用了`service.run()` 的线程中被调用。
> - `service.dispatch(handler)`：这个方法请求`io_service`实例去调用给定的处理方法，但是另外一点，如果当前的线程调用了`service.run()`，它可以在方法中直接调用handler。
> - `service.wrap(handler)`：这个方法创建了一个封装方法，当被调用时它会调用`service.dispatch(handler)`

```cpp
using namespace boost::asio;
io_service service;
void func(int i) { std::cout << "func called, i= " << i << std::endl; }

void run_dispatch_and_post() {
    for ( int i = 0; i < 10; i += 2) {
        service.dispatch(boost::bind(func, i));	// server.run() 处理这个函数的时候, 上交一个任务处理一个
        service.post(boost::bind(func, i + 1));	// server.run() 处理结束之后, 才依次执行任务
    }
}
int main(int argc, char* argv[]) {
    // 注意这里提交的任务是: run_dispatch_and_post
    service.post(run_dispatch_and_post);
    service.run();
}
```

> - **`dispatch`立即执行**：因为此时`service.run()`正在运行（处理`run_dispatch_and_post`任务），`dispatch`绑定的`func(i)`会立即执行，输出偶数（0, 2, 4, 6, 8）。
> - **`post`延迟执行**：将`func(i+1)`加入队列，但当前任务（`run_dispatch_and_post`）未完成，这些`handler`会在循环结束后执行，输出奇数（1, 3, 5, 7, 9）。

`wrap()` 返回了一个仿函数，它可以用来做另外一个方法的参数：

```cpp
using namespace boost::asio;
io_service service;
void dispatched_func_1() { std::cout << "dispatched 1" << std::endl; }
void dispatched_func_2() { std::cout << "dispatched 2" << std::endl; }

void test(std::function<void()> func) {
    std::cout << "test" << std::endl;
    service.dispatch(dispatched_func_1);	// 提交给 io_server 的任务 1, 并立即执行
    func();	// 提交给 io_server 的任务 2, 在当前函数执行之后, 再执行
}
void service_run() { service.run(); }

int main(int argc, char* argv[]) {
    test( service.wrap(dispatched_func_2) );	// 包装了 io_server 的任务 2
    boost::thread th(service_run);		// 主线程执行 test(), 创建的线程执行异步回调函数
    boost::this_thread::sleep( boost::posix_time::millisec(500));
    th.join();
}
```

> 结果

```sh
test
dispatched 1
dispatched 2
```

<br>

## 2. 同步（Synchrony）

---

> 这里主要是实现的就是 TCP/IP 同步服务的最后一种方法多线程处理阻塞 IO

### 1 `Asio` 同步客户端

```cpp
using boost::asio;
io_service service;	// Boost.Asio 的核心类，用于管理所有 I/O 操作
ip::tcp::endpoint ep(ip::address::from_string("Server_IP"), 2001);	// 表示一个网络连接的终点，包括 IP 地址和端口号
ip::tcp::socket sock(service);	// 创建一个套接字并绑定到前面的 io_service，以便通过它进行网络通信
sock.connect(ep);	// 连接到指定的服务器
```

> `Boost.Asio` 使用 *io_service* 同操作系统的输入/输出服务进行交互
>
> 然后，创建**你想要连接的地址和端口**，再建立`socket`。把 `socket` 连接到你创建的地址和端口
>
> 所以你修改 IP 地址，也是可以连接到别的服务器的

### 2 `Asio` 同步客户端 -- 域名

> 对解析完毕的 resolver 解引用的话，返回的结果就是一个 `endpoint`
>
> 使用迭代器的原因是域名可以是多个地址的(端点), 所以我们可能要访问很多个端点,这个 `connet` 帮我们处理好的迭代的工作了

```cpp
using boost::asio;
io_service service;

// 解析域名
ip::tcp::resolver resolver(service);
ip::tcp::resolver::query query("example.com", "2001");  // 域名和端口
ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);	// 创建连接服务器的端点模式

// 创建 socket 并连接
ip::tcp::socket sock(service);
boost::asio::connect(sock, endpoint_iterator);		// sock.connect(*endpoint_iterator);
```

### 3 `Asio`同步服务端

```cpp
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;		// 套接字指针

io_service service;

ip::tcp::endpoint ep(ip::tcp::v4(), 2001));		// 网络服务器端点
ip::tcp::acceptor acc(service, ep);

while (true) {
    socket_ptr sock(new ip::tcp::socket(service));
    acc.accept(*sock);		// 阻塞等待客户端连接，连接建立后 sock 对象中保存了客户端连接的信息

    // 使用线程来创建 IO 服务
    boost::thread(boost::bind(client_session, sock));
}

// IO 服务的主要函数部分
void client_session(socket_ptr sock) {
    while (true) {
        char data[512];
        size_t len = sock->read_some(buffer(data));
        if (len > 0) { 
            write(*sock, buffer("ok", 2)); 
        }
    }
}
```

### 4. echo 服务器

```cpp
```



### 5. echo 客户端

```cpp
```



<br>

## 3. 异步（Asynchrony）

---

### 1. 客户端 

> 当连接完成时（其完成处理程序）你就异步地连接到了指定的地址和端口，也就是说，*connect_handler*被调用了。
>
> 当 *connect_handler* 被调用时，检查错误代码（*ec*），如果成功，你就可以向服务端进行异步的写入。
>
> 当连接完成时（其完成处理程序）你就异步地连接到了指定的地址和端口，也就是说，*connect_handler*被调用了。
>
> 当 *connect_handler* 被调用时，检查错误代码（*ec*），如果成功，你就可以向服务端进行异步的写入。

```cpp
using boost::asio;
io_service service;
ip::tcp::endpoint ep(ip::address::from_string("Server_IP"), 2001);
ip::tcp::socket sock(service);

// Asy
sock.async_connect(ep, connect_handler);
service.run();

void connect_handler(const boost::system::error_code & ec) {
    // 如果ec返回成功我们就可以知道连接成功了
}
```

### 2. 服务端

> 在上述代码片段中，首先，你创建一个*io_service*实例，指定监听的端口。然后，你创建接收器acc——一个接受客户端连接，创建虚拟的socket，异步等待客户端连接的对象。
>
> 最后，运行异步*service.run()*循环。当接收到客户端连接时，*handle_accept*被调用（调用*async_accept*的完成处理程序）。如果没有错误，这个socket就可以用来做读写操作。
>
> 在使用这个socket之后，你创建了一个新的socket，然后再次调用*start_accept()*，用来创建另外一个“等待客户端连接”的异步操作，从而使*service.run()*循环一直保持忙碌状态。

```cpp
using boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;		// 

io_service service;
ip::tcp::endpoint ep(ip::tcp::v4(), 2001)); 	// 监听 Port 2001

ip::tcp::acceptor acc(service, ep);
socket_ptr sock( new ip::tcp::socket(service) );

// 开了 IO 监听，等待连接信号调用到对应的部分
start_accept(sock);
service.run();		// 启动事件循环，处理所有异步操作

// 异步等待客户端连接
void start_accept(socket_ptr sock) {
    // _1 是一个占位符，表示 sock 要作为第一个参数
    acc.async_accept(*sock, boost::bind(handle_accept, sock, _1));	// 这个就相当于信号，里面的函数就是槽
}

// 这里相当于槽
void handle_accept(socket_ptr sock, const boost::system::error_code &err) {
    if (err) return;
    
    // 从这里开始, 你可以从socket读取或者写入
    
    // 开启新的一轮
    socket_ptr sock(new ip::tcp::socket(service));	// 创建新的 socket，保存了客户端连接的信息
    start_accept(sock);		
}
```

<br>

## 4. 异常处理 & 错误代码

---

> `Boost.Asio` **允许同时使用异常处理或者错误代码**，所有的异步函数都有抛出错误和返回错误码两种方式的重载
>
> `boost::system::system_error` 就是错误处理和错误代码的区分关键，更具两者的不同，我们从而决定 try 还是 if 

### 1. 异常处理

```cpp
// 略 ...
ip::tcp::socket sock(service);
sock.connect(ep);  // 如果连接失败，会抛出 boost::system::system_error 异常

try {
    sock.connect(ep);
} 
// 异常处理
catch (boost::system::system_error& e) {
    std::cerr << "连接失败: " << e.what() << std::endl;
}
```

### 2. 错误代码

> 大多时候都在使用这个，因为异步的回调函数把状态作为参数传递的

```cpp
boost::system::error_code ec;
sock.connect(ep, ec);		// 主动放到 connect 捕获状态

// 错误代码判断
if (ec) {
    std::cerr << "连接失败: " << ec.message() << std::endl;
} else {
    // 连接成功
}
```

> 在你的异步函数中，你可以使用异常处理或者错误码（随心所欲），但要 **保持一致性**。同时使用这两种方式会导致问题，大部分时候是崩溃（当你不小心出错，忘记去处理一个抛出来的异常时）。如果你的代码很复杂（调用很多socket读写函数），你最好选择异常处理的方式，把你的读写包含在一个函数`try {} catch {}` 块里面。

## ==5. `IO_Server` 类==

---

### 1.  单  io_server 单线程

> `io_server` 是管理异步的管家，所有 `asy_xx` 的函数都是往操作系统申请事件，虽然我们的异步是非阻塞的，但是如果你注册了多个事件，事件之间是由顺序的，后一个事件要等前一个事件结束，所以是另一种“阻塞”

```cpp
io_service service; // 所有socket操作都由service来处理 

ip::tcp::socket sock1(service); 
ip::tcp::socket sock2(service); 

// 三个异步操作，但是按顺序执行
sock1.async_connect(ep, connect_handler); 
sock2.async_connect(ep, connect_handler); 
deadline_timer t(service, boost::posixtime::seconds(5));
t.async_wait(timeout_handler); 

// 一个 io_server 处理异步事件
service.run();
```

### 2. 一个` io_service` 实例和多个处理线程

> 回顾一下，我们的异步操作相当于申请了事件，我们异步事件的执行是按照顺序的，那么我们分配多个线程干什么呢，线程并不可以帮我们分担异步事件啊
>
> **其实线程里面做的事情并不是分担信号而是分担槽**，就是处理信号发生之后的回调函数（`xxx_handler()`），这避免了单线程模式下因一个耗时的回调阻塞其他操作处理程序的问题

```cpp
io_service service;
ip::tcp::socket sock1(service);
ip::tcp::socket sock2(service);

// 三个异步操作
sock1.async_connect( ep, connect_handler);
sock2.async_connect( ep, connect_handler);
deadline_timer t(service, boost::posixtime::seconds(5));
t.async_wait(timeout_handler);

// 使用线程池快速的处理回调函数
for ( int i = 0; i < 2; ++i)	boost::thread(run_service);

void run_service() {
    service.run();
}
```

### 3. 多个  io_service 实例和多个处理线程

> 其实你应该到这里可以猜得出，为什么会有多个 io_server 了，我们说过，多个线程其实是分担不了异步事件，只能处理事件对应的回调函数。但是 io_server 可以啊，所以我们可以利用多 io 多线程来实现高并行
>
> 这个例子的目的是说明 **如何使用多个独立的 `io_service` 实例来管理不同的异步操作**

```cpp
io_service service[2];

// 注意这里的 sock 是两个不同的 io_server
ip::tcp::socket sock1(service[0]);
ip::tcp::socket sock2(service[1]);

sock1.asyncconnect( ep, connect_handler);		// io_server[0]
sock2.async_connect( ep, connect_handler);		// io_server[1]
deadline_timer t(service[0], boost::posixtime::seconds(5));		// 所以其实只有 io_server[0] 会有多个异步事件
t.async_wait(timeout_handler);

// 多个线程分别执行不同 io 的回调函数
	// 线程 0：调用 service[0].run()，负责处理 sock1 的连接和定时器事件。
	// 线程 1：调用 service[1].run()，负责处理 sock2 的连接
for (int i = 0; i < 2; ++i) boost::thread(boost::bind(run_service, i));

void run_service(int idx) {
    service[idx].run();
}
```

<br>

### 4. 异步 `run(), runone(), poll(), pollone()`

---

#### 1. 持续运行

> 如果有等待执行的操作，*run()*会一直执行，直到你手动调用*io_service::stop()*。为了保证*io_service*一直执行，通常你添加一个或者多个异步操作，然后在它们被执行时，你继续一直不停地添加异步操作

![image-20250216160231597](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250216160231597.png)

#### 2. 单次执行

> 你可以使用run_once()启动一个异步操作，然后等待它执行完成

```cpp
io_service service;
bool write_complete = false;
void on_write(const boost::system::error_code & err, size_t bytes) { write_complete = true; }

std::string data = "login ok”;
write_complete = false;
async_write(sock, buffer(data), on_write);

// 在确保其他异步执行正常的情况下, 直到完成一次 on_write 函数就结束
do service.run_once() while (!write_complete);
```



## 6. socket 类

---

> 不是所有的方法都可以在各个类型的套接字里使用, 如果一个成员函数没有出现在这，说明它在所有的套接字类都是可用的
>
> 注意：所有的异步方法都立刻返回，而它们相对的**同步**实现需要操作完成之后才能返回

| 名字               | TCP  | UDP  | ICMP |
| ------------------ | ---- | ---- | ---- |
| async_read_some    | 是   | -    | -    |
| async_receive_from | -    | 是   | 是   |
| async_write_some   | 是   | -    | -    |
| async_send_to      | -    | 是   | 是   |
| read_some          | 是   | -    | -    |
| receive_from       | -    | 是   | 是   |
| write_some         | 是   | -    | -    |
| send_to            | -    | 是   | 是   |

<br>

### 1. socket  的连接处理函数

| 函数                         | 描述                                                         |
| :--------------------------- | ------------------------------------------------------------ |
| `assign(protocol, socket)`   | 分配一个原生的 socket 给该 socket 实例，常用于处理已建立原生 socket 的旧程序。 |
| `open(protocol)`             | 使用给定的 IP 协议（IPv4 或 IPv6）打开一个 socket，主要用于 UDP/ICMP socket 或服务端 socket。 |
| `bind(endpoint)`             | 绑定 socket 到一个指定的地址。                               |
| `connect(endpoint)`          | 以**同步方式**连接到指定地址。                               |
| `async_connect(endpoint)`    | 以**异步方式**连接到指定地址。                               |
| `is_open()`                  | 判断套接字是否已打开，已打开返回 `true`。                    |
| `close()`                    | 关闭套接字，任何未完成的异步操作会被立即终止，并返回 `error::operation_aborted` 错误码。 |
| `shutdown(type_of_shutdown)` | 立即禁用 `send` 或 `receive` 操作，或两者同时禁用。          |
| `cancel()`                   | 取消套接字上所有异步操作，所有异步操作会立即结束，并返回 `error::operation_aborted` 错误码。 |

> 这里补充说明一下这个 `open` 方法

```cpp
ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
sock.open(ep.protocol());		// 这个端点创建的时候会隐式创建 IPv4 协议, 所以使用的时候也是
```

<br>

### 2.  socket 的 IO 处理函数

| 函数类型             | 函数名称             | 参数                                 | 功能描述                     | 异步/同步 | 备注                           |
| -------------------- | -------------------- | ------------------------------------ | ---------------------------- | --------- | ------------------------------ |
| **异步接收**         | `async_receive`      | `buffer, [flags,] handler`           | 启动从套接字异步接收数据     | 异步      | 与 `async_read_some` 功能一致  |
|                      | `async_read_some`    | `buffer, handler`                    | 同上                         | 异步      | 别名函数                       |
| **异步接收（端点）** | `async_receive_from` | `buffer, endpoint[, flags], handler` | 从指定端点异步接收数据       | 异步      | 适用于 UDP/无连接协议          |
| **异步发送**         | `async_send`         | `buffer [, flags], handler`          | 启动异步发送缓冲区数据       | 异步      | 与 `async_write_some` 功能一致 |
|                      | `async_write_some`   | `buffer, handler`                    | 同上                         | 异步      | 别名函数                       |
| **异步发送（端点）** | `async_send_to`      | `buffer, endpoint, handler`          | 异步发送数据到指定端点       | 异步      | 适用于 UDP/无连接协议          |
| **同步接收**         | `receive`            | `buffer [, flags]`                   | 阻塞接收数据，直到完成或出错 | 同步      | 与 `read_some` 功能一致        |
|                      | `read_some`          | `buffer`                             | 同上                         | 同步      | 别名函数                       |
| **同步接收（端点）** | `receive_from`       | `buffer, endpoint [, flags]`         | 从指定端点阻塞接收数据       | 同步      | 适用于 UDP/无连接协议          |
| **同步发送**         | `send`               | `buffer [, flags]`                   | 阻塞发送数据，直到完成或出错 | 同步      | 与 `write_some` 功能一致       |
|                      | `write_some`         | `buffer`                             | 同上                         | 同步      | 别名函数                       |
| **同步发送（端点）** | `send_to`            | `buffer, endpoint [, flags]`         | 阻塞发送数据到指定端点       | 同步      | 适用于 UDP/无连接协议          |
| **状态查询**         | `available`          | 无                                   | 返回可无阻塞同步读取的字节数 | 同步      | 用于检查数据是否就绪           |

<br>

### ==3 . 缓冲区==

> `ip::socket_type::socket::message_peek`：这个标记只监测并返回某个消息，但是下一次读消息的调用会重新读取这个消息
>
> 你最常用的可能是 *message_peek* ，使用方法请参照下面的代码片段：

```cpp
char buff[1024];
sock.receive(buffer(buff), ip::tcp::socket::message_peek );
memset(buff,1024, 0);
// 重新读取之前已经读取过的内容
sock.receive(buffer(buff));
```

> 当从一个套接字读写内容时，你需要一个缓冲区，用来保存读取和写入的数据。
>
> 缓冲区内存的有效时间必须比`I/O`操作的时间要长；你需要**保证它们在I/O操作结束之前不被释放**。 对于同步操作来说，这很容易；当然，这个缓冲区在`receive`和`send`时都存在。

<br>

> 当需要对一个buffer进行读写操作时，代码会把实际的缓冲区对象封装在一个buffer()方法中，然后再把它传递给方法调用：

```cpp
char buff[512];
sock.async_receive(buffer(buff), on_read);
```



#### 1. 异步套接字缓冲区的问题

> 但是在异步操作时就没这么简单了，看下面的代码片段：

```cpp
// 非常差劲的代码 ...
void on_read(const boost::system::error_code & err, std::size_t read_bytes)
{ ... }

void func() {
    char buff[512];
    sock.async_receive(buffer(buff), on_read);	// 进入异步函数之后, 我们的 buff 是释放了还是没有呢?
}
```

> 我们进入异步的 on_read 函数回调的时候, 这个buff 是释放还是没有被释放的状态呢?
>
> 在我们调用 *async_receive()* 之后，buff 就已经超出有效范围，它的内存当然会被释放。当我们开始从套接字接收一些数据时，我们会把它们拷贝到一片已经不属于 buff 的内存中；不属于 buff 的内存可能会被释放，或者被其他代码重新开辟来存入其他的数据，结果就是：内存冲突。

#### 2. 缓冲区问题解决方案

##### 1.使用全局缓冲区

>  第一个方法显然不是很好，因为我们都知道全局变量非常不好。此外，如果两个实例使用同一个缓冲区怎么办？

##### 2.创建一个缓冲区，然后在操作结束时释放它

> 下面是**第二种方式的实现**：

```cpp
void on_read(char * ptr, const boost::system::error_code & err, std::size_t read_bytes) {
    // 读取操作完成之后 ...
    delete[] ptr;		// 手动释放空间
}

char * buff = new char[512];	// 动态的空间,异步结束后之后处理

// _1 和 _2 分别占位 error_code 和 read_bytes，这两个参数会在异步操作完成时由 async_receive 传入。
sock.async_receive(buffer(buff, 512), boost::bind(on_read, buff, _1, _2));
```

> **第二种方式**的另一种实现:

```cpp
struct shared_buffer {
    // 构造函数：分配指定大小的缓冲区并初始化
    shared_buffer(size_t size) : buff(new char[size]), size(size) {}
    
    // 返回 asio 的 标准 buffer
    mutable_buffers_1 asio_buff() const { return buffer(buff.get(), size); }
    
    boost::shared_array<char> buff;	// 这个是核心, 因为它会随着作用域的析构而析构
    int size;
};

// 当on_read超出范围时, boost::bind对象被释放了,同时也会释放 buff
void on_read(shared_buffer, const boost::system::error_code & err, std::size_t read_bytes) {}

struct shared_buffer buff(512);
sock.async_receive(buff.asio_buff(), boost::bind(on_read,buff,_1,_2));
```

##### 3.使用一个集合对象管理这些套接字和其他的数据，比如缓冲区数组

> 第三个选择是使用一个连接对象来管理套接字和其他数据，比如缓冲区，通常来说这是正确的解决方案但是非常复杂。



### 4.套接字控制

> 这些函数用来处理**套接字**的高级选项：
>
> - `get_io_service()`：这个函数返回构造函数中传入的io_service实例
> - `get_option(option)`：这个函数返回一个套接字的属性
> - `set_option(option)`：这个函数设置一个套接字的属性
> - `io_control(cmd)`：这个函数在套接字上执行一个I/O指令

这些是你可以获取/设置的套接字选项：

| 名字                        | 描述                                                  | 类型 |
| --------------------------- | ----------------------------------------------------- | ---- |
| `broadcast`                 | 如果为true，允许广播消息                              | bool |
| `debug`                     | 如果为true，启用套接字级别的调试                      | bool |
| `do_not_route`              | 如果为true，则阻止路由选择只使用本地接口              | bool |
| `enable_connection_aborted` | 如果为true，记录在accept()时中断的连接                | bool |
| `keep_alive`                | 如果为true，会发送心跳                                | bool |
| `linger`                    | 如果为true，套接字会在有未发送数据的情况下挂起close() | bool |
| `receive_buffer_size`       | 套接字接收缓冲区大小                                  | int  |
| `receive_low_watemark`      | 规定套接字输入处理的最小字节数                        | int  |
| `reuse_address`             | 如果为true，套接字能绑定到一个已用的地址              | bool |
| `send_buffer_size`          | 套接字发送缓冲区大小                                  | int  |
| `send_low_watermark`        | 规定套接字数据发送的最小字节数                        | int  |
| `ip::v6_only`               | 如果为true，则只允许IPv6的连接                        | bool |

```cpp
ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 80);
ip::tcp::socket sock(service);
sock.connect(ep);
// TCP套接字可以重用地址
ip::tcp::socket::reuse_address ra(true);
sock.set_option(ra);
// 获取套接字读取的数据
ip::tcp::socket::receive_buffer_size rbs;
sock.get_option(rbs);
std::cout << rbs.value() << std::endl;
// 把套接字的缓冲区大小设置为8192
ip::tcp::socket::send_buffer_size sbs(8192);
sock.set_option(sbs);
```

> **在上述特性工作之前，套接字要被打开**。否则，会抛出异常

### 5. 套接字读取数据

> 之前的 demo 里面都是大致的如何连接, 同步和异步的区别, 但是 IO 部分一直都没有处理, 毕竟没涉及到缓存, 所以知道了缓存之后, 我们再来看看如何实现**同步或异步地从不同类型的套接字上读取数据**

- 例1: 是在一个 **TCP套接字** 上进行同步读写：

  ```cpp
  ip::tcp::endpoint ep(ip::address::from_string("87.248.112.181"), 80);
  ip::tcp::socket sock(service);
  sock.connect(ep);
  
  sock.write_some(buffer("GET /index.html\r\n"));
  std::cout << "bytes available " << sock.available() << std::endl;
  char buff[512];
  size_t read = sock.read_some(buffer(buff));
  ```

- 例2: 是在一个 **UDP套接字** 上进行同步读写：

  ```cpp
  ip::udp::socket sock(service);
  sock.open(ip::udp::v4());
  ip::udp::endpoint receiver_ep("87.248.112.181", 80);		// target
  
  sock.send_to(buffer("testing\n"), receiver_ep);
  char buff[512];
  ip::udp::endpoint sender_ep;
  sock.receive_from(buffer(buff), sender_ep);
  ```

- 例3: 是从一个 **UDP服务端** 套接字中 **异步** 读取数据：

```cpp
using namespace boost::asio;
io_service service;
ip::udp::socket sock(service);
ip::udp::endpoint sender_ep;		// 用于记录发送端

char buff[512];

void on_read(const boost::system::error_code & err, std::size_t read_bytes) {
    std::cout << "read " << read_bytes << std::endl;
    sock.async_receive_from(buffer(buff), sender_ep, on_read);
}

int main(int argc, char* argv[]) {
    ip::udp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);    // 服务端自己的端点
    sock.open(ep.protocol());		// 
    sock.set_option(ip::udp::socket::reuse_address(true));	//
    sock.bind(ep);
    // region 核心部分
    sock.async_receive_from(buffer(buff,512), sender_ep, on_read);
    // endregion
    service.run();
}
```

### 6. 套接字实例不能被拷贝

> 最后要注意的一点，套接字实例不能被拷贝，因为拷贝构造方法和＝操作符是不可访问的。

```cpp
ip::tcp::socket s1(service), s2(service);
s1 = s2; // 编译时报错
ip::tcp::socket s3(s1); // 编译时报错
```

> 因为每一个实例都拥有并管理着一个资源（原生套接字本身）
>
> 如果我们允许拷贝构造，结果是我们会有两个实例拥有同样的原生套接字；`Boost.Asio` 选择不允许拷贝（如果你想要创建一个备份，请使用共享指针）

## ==7. 伪封包延长对象的生命周期==

> 看如下情况

```cpp
void Session::Start() {
    _sock.async_read_some(
        asio::buffer(_data, MAX_SIZE),
        // 看这里，我们其实是把 this 在异步函数中把 this 传递出去了
        [this]<typename T0, typename T1>(T0 && PH1, T1 && PH2) {
            handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2)); 
        }
    );
}
```

> 那么这个 `this` 和异步回调实际上是有冲突的，如果我们的 `this` 在 `handle_read` 之前就释放了自己，那么异步再使用就会导致程序的崩溃
>
> 为了解决这个方法，我们希望延长 `this` 的声明周期，至少保证说，我们的 `this` 要在 `handle_read` 生命周期内不消亡
>
> **所以我们使用 `share_ptr`**，制造一个 `this` 的 `share` 版本传递给 `handle_read`，那么由于 `handle_read `作用域还没有结束，计数不会归 0 所以 this 不会消亡，就保证了，`handle_read `生命周期内 `this` 一直存在

```cpp
// 处理一下 handle_read 用于接受 share 版本的 this
void Session::handle_read(boost::system::error_code & error, size_t bytes_transferred, share_ptr<Session> & _self);
```

```cpp
// 调整一下 Start
void Session::Start() {
    auto _self = share_ptr<Session>(this);		// 创建一个 share 的 this
    _sock.async_read_some(
        asio::buffer(_data, MAX_SIZE),
        // 看这里，我们其实是把 this 在异步函数中把 this 传递出去了
        [this, _self]<typename T0, typename T1>(T0 && PH1, T1 && PH2) {
            handle_read(std::forward<T0>(PH1), std::forward<T1>(PH2), _self); 
        }
    );
}
```

> 虽然看起来我们是用 share 维护了异步函数时候 this 的生命周期，但是还有一个问题，就是 this 的 share 指针都指向了同一个 this，那么其实可能会造成二次析构的，所以我们需要引入 `share_from_this()` 来解决二次析构的问题
>
> 因为 `share_from_this()` 创建了一个 `this` 的副本，然后再增加计数，这样子就不用担心二次析构的问题了

# $Ex. 关联拓展$
