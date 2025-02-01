# `Boost::Asio`

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

<br>

## 2. 同步（Synchrony）

---

> 这里主要是实现的就是 TCP/IP 同步服务的最后一种方法多线程处理阻塞 IO

### 1 `Asio` 同步客户端

```cpp
using boost::asio;
io_service service;	// Boost.Asio 的核心类，用于管理所有异步 I/O 操作
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

### 2. 一个 io_service 实例和多个处理线程

> 回顾一下，我们的异步操作相当于申请了事件，我们异步事件的执行是按照顺序的，那么我们分配多个线程干什么呢，线程并不可以帮我们分担异步事件啊
>
> 其实线程里面做的事情并不是分担信号，分担槽，就是处理信号发生之后的回调函数（`xxx_handler()`），这避免了单线程模式下因一个耗时的回调阻塞其他操作处理程序的问题

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

