# 网络通信

---

![image-20241009141947218](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241009141947218.png)

## 1. 网络编程的整体函数流程

> 客户端始终只有一个套接字，是自己的，但是要创建服务器的 `sockaddr_in`
>
> 服务器有两个套接字，一个自己的，一个 `accept` 拿的对应的客户端的

![image-20241117105952888](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241117105952888.png)

## 2. `socket()`

> - 参数:
>     - domain: 使用的地址族协议
>         - `AF_INET`: 使用 `IPv4` 格式的 `ip` 地址
>         - `AF_INET6`: 使用 `IPv6` 格式的 `ip` 地址
> - type:
>     - `SOCK_STREAM`: 使用流式的传输协议
>     - `SOCK_DGRAM`: 使用报式(报文)的传输协议
> - protocol: 一般写 0 即可, 使用默认的协议
>     - `SOCK_STREAM`: 流式传输默认使用的是 tcp
>     - `SOCK_DGRAM`: 报式传输默认使用的 udp
> - 返回值:
>     - **成功: 可用于套接字通信的文件描述符**
>     - 失败: -1

```c
// 创建一个套接字
int socket(int domain, int type, int protocol);
```

## 3. 套接字结构体

> 这个是创建套接字之后，我们可以使用这个结构体来配置服务器的基本信息
>
> 譬如说，服务器使用的地址、地址类型和端口

```c
///////////////////////////////////////////////////////////////////////
//////////////////////////////// Linux ////////////////////////////////
///////////////////////////////////////////////////////////////////////
typedef uint16_t in_port_t;		// 端口就是 ：0 - 2^16-1
typedef uint32_t in_addr_t;		// 32 位地址 255.255.255.255 - 0.0.0.0
typedef unsigned short int sa_family_t;

// 表示一个 IPv4 地址，存储形式为 32 位（4 字节）的无符号整数
struct in_addr {	
    // IPv4 地址，32 位整型，大端序
    in_addr_t s_addr;
};  

// 这一般用转换 sockaddr_in(information) 类型的数据，作为参数被其他函数调用 
// (struct sockaddr *)&sockaddr_in_type_data 只保留协议和地址和端口
struct sockaddr {
    sa_family_t sa_family;  // 地址族 (Address family)，例如 AF_INET
    char sa_data[14];       // 存放地址和端口的原始数据
};


// 用于描述 IPv4 套接字地址，包括地址族、端口号和 IP 地址等
struct sockaddr_in {
    sa_family_t sin_family;     // 地址族, AF_INET（IPv4）
    in_port_t sin_port;         // 端口号，2字节
    struct in_addr sin_addr;    // IP 地址，4字节
    unsigned char sin_zero[8];  // 填充字节，确保结构体对齐
}; 
```

### 3.1 使用场景

> 服务端：绑定 IP 和端口
>
> 对于 `htons()` 的说明请看看后面的 3.3

```c
struct sockaddr_in server_addr;

server_addr.sin_family = AF_INET;	 // 使用的协议
server_addr.sin_port = htons(8080);  // 设置端口为 8080
server_addr.sin_addr.s_addr = INADDR_ANY;  // 绑定本机所有可用 IP

// 绑定地址和端口
bind(sever_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
```

> 客户端：连接到服务器

```c
struct sockaddr_in server_addr;

server_addr.sin_family = AF_INET;		// 使用的协议
server_addr.sin_port = htons(8080);  	// 目标服务器端口
// client_addr.sin_addr = inet_addr.s_addr("SERVER_IP"); 比较简单，但有隐患
inet_pton(AF_INET, "SERVER_IP", &server_addr.sin_addr);  // 转换 IP 地址

// 连接到目标服务器
connect(client_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
```

### ==3.2 IP 地址的分类==

![image-20241130183348147](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241130183348147.png)

> ### `A` 类地址

<img src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ee46e44ba6a44427831a4872f76311bd~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp" alt="img" style="zoom:65%;" />

> 所以 A 的地址范围就是：`0.0.0.0 ~ 127.255.255.255`
>
> `10.0.0.0～10.255.255.255`这个范围的A类地址是私有的，不再公网中使用，仅用于局域网（LAN）内部通信，不会在公网（Internet）中直接使用或路由
>
> 那么这些局域网地址一共有多少个呢？

![image-20241130202603587](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241130202603587.png)

> ### `B` 类地址
>
> 其实不同的 x 类地址，就是第一位字节的 0 所在的位置而改变地址的范围
>
> 那么地址类别越靠前，那么说能分配的地址就越多

<img src="https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/1f8675ff617947dca135add4613e4339~tplv-k3u1fbpfcp-zoom-in-crop-mark:1512:0:0:0.awebp" alt="img" style="zoom:67%;" />

<br>

- 我们简单总结一下
- 你转换 IP 地址的第一位，只需要看前几位 0 在哪里就知道是第几类地址了
- `C` 的是最常用的私有地址范围，尤其是在家庭或小型企业网络中

| 地址类别 |           起始范围            |   前几位   |          十进制范围           |          私有地址范围           |
| :------: | :---------------------------: | :--------: | :---------------------------: | :-----------------------------: |
|   A类    | `0.0.0.0` ~ `127.255.255.255` | `0xxxxxxx` | `0.0.0.0` ~ `127.255.255.255` |  `10.0.0.0` ~ `10.255.255.255`  |
|   B类    | `128.0.0.0`~`191.255.255.255` | `10xxxxxx` | `128.0.0.0`~`191.255.255.255` | `172.16.0.0` ~ `172.31.255.255` |
|   C类    | `192.0.0.0`~`223.255.255.255` | `110xxxxx` | `192.0.0.0`~`223.255.255.255` | `192.168.0.0`~`192.168.255.255` |



### 3.3 `htons()`

> 这个函数解决的**大小端问题**，我就不再赘述了，那么看关键的函数名字的部分
>
> 通过这个函数，那么我们在客户端绑定到服务端的时候
>
> 最后的一个 `s` 代表的意思是 `unsigned short` 因为它正好是 32 bit，也就是 4 字节

![image-20241130204911453](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241130204911453.png)

### 3.4 `inet_ptons()`

> 更加安全，虽然一般来说就是 `inet_addr()` 也可以的

```cpp
inet_pton(AF_INET, "192.168.1.1", &client_addr.sin_addr);  // 转换 IP 地址
```

<br>

## 4. `listen()`

> **进入监听状态**：
>
> - 让服务器套接字变为**被动套接字**，专门用来接收连接请求。
> - 配合 `accept` 函数，服务器可以处理客户端的连接。
>
> **连接请求队列**：
>
> - 在 TCP 三次握手过程中，`listen` 提供了一个队列来存储已经完成和等待完成握手的连接请求。
> - `backlog` 参数控制队列的最大长度。
>
> **`backlog` 的详细说明**
>
> - **连接队列的分为两部分**：
>     1. 未完成连接队列：存储那些已经发送客户端 SYN 包但还未完成三次握手的连接。
>     2. 已完成连接队列：存储那些已经完成三次握手、等待 `accept` 的连接。

```c
// 返回值：
	// 成功：返回 0。
	// 失败：返回 -1，并设置 errno 表示错误原因。
int listen(int sockfd, int backlog);
```

## 5. `accept()` 

> 1. **`sockfd`**：
>    - **类型**：`int`。
>     - **描述**：这是一个已经调用了 `bind()` 和 `listen()` 的监听套接字。`sockfd` 是服务器端的套接字描述符
> 2. **`addr`**：
>     - **类型**：`struct sockaddr *`（通常使用 `struct sockaddr_in *`，即 IPv4 地址类型）。
>     - **描述**：这是一个指向 `struct sockaddr` 类型的指针，`accept` 会将客户端的地址信息填充到这个结构体中。你可以使用 `struct sockaddr_in` 来获取 IPv4 地址和端口号。这个参数是可选的，可以为 `NULL`，但通常我们使用它来获取客户端的信息。
>     - **注意**：但是我们一般创建的都是 `sockaddr_in` 类型，所以一般都在这里进行强制类型转换
> 3. **`addrlen`**：
>     - **类型**：`socklen_t *`，就是 sockaddr_in 的大小。
>     - **描述**：用于存储客户端信息的 `sockaddr_in` 

```c
// 返回值
	// 成功：返回一个 *新的套接字描述符 cfd*，该套接字是用于与客户端通信的。此时，服务器已经成功接收到客户端的连接。
	// 失败：返回 -1，并且设置 errno。可能的错误原因包括套接字不可用、资源不足、连接超时等。
int accept(int sockfd, struct sockaddr *client_addr, socklen_t *client_addr_len);
```

<br>

## 6. `connect()` 

---

> 绑定客户端套接字，到服务器的地址信息

### 6.1 **`connect`函数的常见参数**

>- **socket**: 要连接的客户端套接字
>- **address**: 服务器的地址信息，通常是一个 `sockaddr` 结构，包含服务器的 IP 地址和端口号
>- **address_length**: 地址结构的大小，通常是 `sizeof(sockaddr_in)`

### **6.2 `connect` 函数的错误处理**

>- 如果连接成功，`connect` 返回 0
>- 如果连接失败，`connect` 返回 `-1`，并且可以通过 `errno` 获取具体的错误原因

```cpp
// 连接到服务器
if (connect(client_sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
    perror("连接失败");
    close(sock);
    exit(1);
}
```

<br>

## 7. TCP 通信的接受和发送

---

### **`recv` 和 `send` 的返回值的如何确定的**



![image-20241201145321273](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241201145321273.png)

### 1. `send()`

> - **`sockfd`**: 套接字描述符
> 	- 客户端的话要写自己的，但是服务端要写客户端
> - **`buf`**: 指向要发送的数据缓冲区的指针。
> - **`len`**: 要发送的数据字节数。
> - **`flags`**: 控制发送行为的标志。
> 	- 常用标志：
> 		- `0`：默认，无特殊行为。
> 		- `MSG_DONTWAIT`：非阻塞发送。
> 		- `MSG_NOSIGNAL`：阻止发送过程中产生的 `SIGPIPE` 信号（如对端关闭连接时）。
> - **返回值**:
> 	- 成功：返回发送的字节数。
> 	- 失败：返回 `-1`，并设置 `errno`。

```cc
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

<br>

### 2. `recv()`

> - **`sockfd`**: 套接字描述符。
> - **`buf`**: 用于存储接收数据的缓冲区指针。
> - **`len`**: 缓冲区大小（可接收的最大字节数）。
> - **`flags`**: 控制接收行为的标志。
> 	- 常用标志：
> 		- `0`：默认，无特殊行为。
> 		- `MSG_PEEK`：窥探数据，不移出接收缓冲区。
> 		- `MSG_WAITALL`：阻塞接收直到读取完整的 `len` 字节。
> 		- `MSG_DONTWAIT`：非阻塞接收。
> - **返回值**:
> 	- 成功：**返回接收到的字节数**。
> 	- 连接关闭：返回 `0`。
> 	- 失败：返回 `-1`，并设置 `errno`。

```cc
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```



### 3.  `recv()`  返回值的使用场景

> 我们开辟接受数据的空间 1024 大小，但是我们实际上要使用的就是接收到了多少个字节，所以这个返回值其实是不错的一种告诉我们收到多大的数据的方式

```cpp
// 接受服务器发送的数据
auto recv_status = recv(server_sockfd, buf.data(), buf.size(), 0);	// 返回接收到数据的长度
if (recv_status > 0) {
    std::cout << "Server Reply: " 
        	// 注意看这里的 把拿到的数据转换成了 string 然后开辟对应的大小的 begin() + recv_status
          << std::string(buf.begin(), buf.begin() + recv_status) << std::endl;
}
```

### 4. `send()` 发送数据过大的问题

> 如果说 `send()` 发送的数据过大的话，我们“发一收一”的模式就不太合适了
>
> 因为过大的数据，计算机是拆分多次发送，于是发一就马上收一，但是数据没有发送完的嘛，就可能变成多次对话了
>
> 为了解决这个问题，我们可以记录发送的数量，循环一直记录接受的数量，一直到我们发送的数量再停止

```cc
// 接收数据
ssize_t recv_len = 0; // 已接收的字节数
std::vector<char> buf(1024); // 接收缓冲区，大小为1024字节

while (recv_len < send_len) {
    // 第三个参数是可用的空间的大小，每一次减的是上一次的，所以可用总是反应当前次的
    ssize_t recv_count = recv(client_sockfd, buf.data() + recv_len, buf.size() - recv_len, 0);
    if (recv_count == -1) {
        std::cerr << "Error in recv: " << strerror(errno) << std::endl;
        return -1;
    } else if (recv_count == 0) {
        std::cerr << "Connection closed by peer." << std::endl;
        break; // 对端关闭连接
    }
    recv_len += recv_count; // 累计接收的字节数
}
```

## 8. 广播

> 广播就是利用 UDP 给**同一网络**中的主机传输数据，所以这个适用于内网

- **本地广播** 示例：
    如果设备发送到`255.255.255.255`，本地子网内的所有设备都会接收这个数据包。

- **直接广播 ** 示例：
    假设你的网络是`10.0.0.0/24`，广播地址为`10.0.0.255`，这个数据包将到达`10.0.0.0/24`**子网**内的所有主机。

### 1. **CIDR** 表示法

> 这个用于简写子网掩码用的，譬如说 `/8` 就表示第一位是都是 1，`255.0.0.0`
>
> `/16` 表示 `255.255.0.0`，简单吧

### 2. 如何判断是否在同一子网

> **IP地址**：分为网络位和主机位。例如：`192.168.1.10/24`
>
> **子网掩码**：用于将IP地址的网络部分与**主机部分**分开。`/24` 表示子网掩码为 `255.255.255.0`。
>
> **广播地址**：子网内所有主机位都是`1`的地址。
>
> **网络地址**：子网内所有主机位都是`0`的地址。

- 主机 A：`192.168.1.10/24`

```ruby
192.168.1.10  ->  11000000.10101000.00000001.00001010
255.255.255.0 ->  11111111.11111111.11111111.00000000
AND运算结果 ->  11000000.10101000.00000001.00000000  (192.168.1.0)
```

- 主机 B：`192.168.1.50/24`

```ruby
192.168.1.50  ->  11000000.10101000.00000001.00110010
255.255.255.0 ->  11111111.11111111.11111111.00000000
AND运算结果 ->  11000000.10101000.00000001.00000000  (192.168.1.0)
```

> 所以他们是在一个子网中的

### $\textcolor{pink}{3. 子网划分}$

---

#### 3.1 需要划分成多少个子网

> 就是 $2^x = 需要的网段数$，譬如说像划分4个网段，那么 x = 2
>
> 那么X代表什么意思呢？X 就是取值多少位 1，如 11000000，就是借2位，即 X=2，反推知道一个字节（8位）如果为 11000000，则表示可以划分4个网络（22=4）；如果为 11110000，则表示借4位，可以划分16个网络（24=16）。

#### 3.2 每个网络需要多少个 IP

> 计算公式：$2^y-2$，y 代表取值多少位 0，如 11111000，代表取值 3位 0，即 $y=3$, 得出 $2^3- 2=8-2=6$，即每个网络可用的IP数量是6个。
>
> 假如每个网络需要 60 台主机的IP，那么如何计算呢？即 $2^y≥60$，根据2的幂算法得出，$2^6=64-2=62$，所有y的值为6。

#### 3.3 有哪些子网网络

> 计算公式：`256-子网掩码`。假如子网掩码为 255.255.255.192，则块大小为 $256-192=64$，即块大小为 64。从0开始以64为基数不断递增，直到达到子网掩码值，中间的结果即为子网，本例中子网即 `0、64、128、192`。
>
> `192 -> 1100 | 0000`，所以 `x = 2, y = 6`

#### 3.4 每个子网的子网号和广播地址是多少

> 广播地址很简单，总是下一个子网前面的数。
>
> - 如子网号为 0、64、128、192、则广播地址为 63、127、191
> - 再如子网号为 0、128，则广播地址为 127
>
> ==看到这里知道为啥在计算每个网段可用的 IP 数时，公式里要 -2 吗？==
>
> 这是因为每个子网号（子网号也是一个IP地址）和广播地址是不能配给主机用的，`192.168.0.0` & `192.168.0.255`

## 9. HTTP



# Linux 网络编程配置

---

## 1. Window 配置免密登录

> 在 `.ssh` 所在的位置，使用 `ssh-keygen.exe -t  "密钥文件名"`

```shell
ssh-keygen.exe -t ssh_key
```

> 那么就会有两个文件
>
> - `.pub` 文件存储到服务端的 `authorized_key` 用于验证密钥
> - `ssh_key` 就是密钥，存储到 PC 端

```shell
目录: C:\Users\shuhe\.ssh

Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----        2024/11/22     12:24            399 ssh_key.pub
-a----        2024/11/22     12:24            399 ssh_key
-a----        2024/11/22     12:33            113 config
```

> 其中 `config` 文件需要配置一些内容，才能让免密生效

```shell
# config 文件
Host Momo
  HostName 121.43.34.47
  User root
  Port 22


# 配置密钥所在的位置
IdentityFile C:\Users\shuhe\.ssh\ssh_key
```

> 服务端的要把 `ssh.pub` 塞入到 `authorized_key` 
>
> 这之后就可以免密登录了

```shell
cat >> authorized_keys < ssh_key.pub
```

## 2. Git 连接 `Github`

> 对于网络开发，我们的环境会分成 `Linux 网络开发` 和 `Linux/Unix 客户端` 开发两个环境
>
> - 处于考虑再任何设备上都可以开发，我们将其部署到 `github` 方便任意设备访问
> - 但是如果要 push 操作的话，就需要对自己的 `repo` 有操控权，所以我们要进行一些配置
> - 由于 `repo` 是通过 `ssh` 操作的，**所以你自己的仓库下载别用 https 而是 git@ **

> 在上面一部免密码登录我们可以拿到一个 pub 公钥

```shell
目录: C:\Users\shuhe\.ssh

Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
-a----        2024/11/22     12:24            399 ssh_key.pub
```

> 我们把这个公钥的内容放到 `github  `的 `ssh `验证里面

![image-20241123160641607](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241123160641607.png)

> 然后 `git config` 改我们的 `git` 的配置，那么我们 `git clone` 下来自己的项目就可以使用 `git` 控制了

```shell
git config --global user.name "UserName"
git config --global user.email "UserName@github.com"
```

> 最后创建一个 `config` 文件就行了

```markdown
Host github.com
HostName ssh.github.com
User git
Port 443
```



## 3. Linux 服务器外网访问的配置

> [软件的地址](https://www.alipan.com/t/iP1WZgrmfUGb4t5a3Bhj)
>
> 使用方法

## 4. Linux 默认编译器版本的切换

> 这个命令可以列出你所有的编译器，然后使用数字来切换默认版本

```shell
 sudo update-alternatives --config gcc
```

> 对于比较新的版本的 `GCC` 才可以支持像是 `format` 这类的操作，对于新版的安装方法需要[参考这个链接](https://phoenixnap.com/kb/install-gcc-ubuntu)
>
> 同时使用 24 年之后的版本比较好，我这里选了 13.3

```shell
wget http://ftp.gnu.org/gnu/gcc/gcc-13.3.0/gcc-13.3.0.tar.gz
```



# 多进程服务器的实现
---
![image-20241205184320376](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241205184320376.png)

> `CPU` 核的个数与进程数，CPU 能够进行数字处理的运算器就是核，而一个CPU有几个计算器，就称之为几核
>
> 每一个核可以处理一个进程，所以可以同时执行多个进程，如果进程数量多于 CPU 核的个数，就会**分时**使用核

## 1. 多进程服务器

---

### 1.1 fork 函数

![image-20241205184734571](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241205184734571.png)

> fork 函数会在自己这个进程的内部产生一个子进程同时和自己执行一样的代码，但是他们是两个不同的程序
>
> **父子进程有完全独立的内存结构**
>
> 所以两个函数输出的结构是完全不一样的，如果你输出 `gval` 和 `lval`，完全是两个结果

### 1.2 僵尸进程产生的原因

> 僵尸进程的触发条件就是：**子进程退出后，父进程没有及时处理其退出状态**。
>
> 虽然僵尸进程本身不会占用太多系统资源，但如果父进程长期不处理，系统中僵尸进程过多可能会导致资源耗尽问题

### 1.3 僵尸进程的验证

- 僵尸进程的 demo

```c
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();		// 创建了子进程

    if (pid == 0)
    {
        puts("Hi, I am a child Process");
    }
    else	// 父进程不会马上退出，而是休息一段时间，这段时间内，子进程没有父进回收，于是哪怕结束了就成为了僵尸
    {
        printf("Child Process ID: %d \n", pid);
        sleep(30);
    }

    if (pid == 0)
        puts("End child proess");
    else
        puts("End parent process");
    return 0;
}
```

![image-20241205193445752](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241205193445752.png)

### 1.4 销毁僵尸进程

---

>  为了销毁子进程，父进程还可以主动去请求子进程的返回值，后面两个函数就是主动发起请求的方式

#### 1. `wait()` 函数

> 先入代码为主，我们解释一下 `wait` 和 `WIFEXITED` 这两个函数

```c
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t pid = fork(); 
    
    if (pid == 0) { return 3; }		// 让子进程提前结束
    else {
        printf("Child PID: %d \n", pid);
        pid = fork(); 	// 注意这里再一次创建了子进程
        if (pid == 0) { exit(7); }	// 子进程提前结束
        else {
            printf("Child PID: %d \n", pid);
            wait(&status);         	// 终止的子进程相关信息将被保存到 status 中，同时相关子进程被完全销毁
            if (WIFEXITED(status)) 	// 验证子进程是否正常终止。
                printf("Child send one: %d \n", WEXITSTATUS(status));	// 返回子进程返回值

            wait(&status); 	//因为之前创建了两个进程，所以再次调用 wait 函数和宏
            if (WIFEXITED(status))
                printf("Child send two: %d \n", WEXITSTATUS(status));
            sleep(30);
        }
    }
    return 0;
}
```

- `wait(&status)`  如果之前有子进程结束了，就会提前获取终止子进程的 ID
- `WIFEXITE(status)` 判断子进程是不是正常的结束
- `WEXITSTATUS(status)` 返回子进程的返回值

> 返回结果，你可以看得到返回值就是 return 和 exit 的值

![image-20241205195214658](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241205195214658.png)

#### 2. `waitpid()` 函数

> wait 可能回引起阻塞
>
> 1. 譬如说主程序先到了 `wait() `但是子进程还在处理之前的内容
> 2. 于是主程序就自在等待子进程释放，但是在其没结束前应该处理别的时间，这个阻塞是不好的
>
> 所以我们可以使用 `waitpid()` 来解决阻塞问题

**语法**：如果没有任何子进程状态发生变化（即子进程还未退出），`waitpid()` **立即返回**，不会阻塞父进程

```c
pid_t waitpid(pid_t pid, int *status, int options);
```

- 参数说明：
	- `pid`：指定要等待的子进程的 PID。**`-1` 表示等待任意子进程**。
	- `status`：用于存储子进程的退出状态。
	- `options`：控制行为的选项，比如 `WNOHANG`。

> 案例

```c
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        sleep(15); 	//用 sleep 推迟子进程的执行，确保主进程等待这个线程
        return 24;
    }
    else {
        // 调用 waitpid 传递参数 WNOHANG ，这样之前有没有终止的子进程则返回 0
        while (!waitpid(-1, &status, WNOHANG)) {
            // 没有子进程结束，那么我们就干点别的事情
            sleep(3);
            puts("sleep 3 sec.");
            // 这里面都是做些别的事情
        }

        if (WIFEXITED(status))
            printf("Child send %d \n", WEXITSTATUS(status));
    }
    return 0;
}
```

#### 3. `Sigaction()` 函数

> 先看完下面的信号处理再回来

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 这个最好的点就在于，我们不需要挂一个 while 等，而是有了结束就直接处理
void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed proc id: %d \n", id);             //子进程的 pid
        printf("Child send: %d \n", WEXITSTATUS(status)); //子进程的返回值
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);	// 子进程结束触发

    pid = fork();	// 1.
    if (pid == 0) //子进程执行阶段
    {
        puts("Hi I'm child process");
        sleep(10);
        return 12;	// 触发 1.
    }
    else //父进程执行阶段
    {
        printf("Child proc id: %d\n", pid);
        pid = fork();	// 2.
        if (pid == 0) {
            puts("Hi! I'm child process");
            sleep(10);
            exit(24);	// 触发 2.
        }
        else {
            printf("Child proc id: %d \n", pid);
            for (int i = 0; i < 5; i++) {
                puts("wait");
                sleep(5);
            }
        }
    }
    return 0;
}
```





### 1.5 信号处理

> 虽然说等待处理也是一个不错的方法，但是很多时候父进程也很忙，就算处理别的事情，也是 `whlie` 里面做一部分，总不可能大部分逻辑都是 `while` 实现吧
>
> 于是就有了这个信号处理，信号处理就是在子进程结束之后，返回值给操作系统，但是原本操作系统不做操作的嘛，而信号做操作了，通知了父进程

![image-20241205205735750](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241205205735750.png)

#### 1. ==`signal()`== 函数

> 信号函数的函数声明

```c
void (*signal(int signo, void (*func)(int)))(int);
```

> 那么这个其实是两个函数指针所以看的写迷糊，拆分分析下

```c
void (*signal (int signo, ...))(int)；		// ... 在下面
// ... 具体
void (*func)(int) // 他是一个函数指针作为第二个参数
```

> 所以 `signal()` 是一个函数，它有两个参数
>
> 他的返回值也是一个函数指针，就是 `void (int)` 部分，目的就是为了把你传入的函数返回给你
>
> 所以整体结构差不多是声明了一个 *signal 函数指针，使用这个函数指针要两个参数，会返回一个函数结果

```cpp
using SignalHandler = void(*)(int);
// 简化实现版本
SignalHandler signal(int signo, SignalHandler func);
```

> 第一个参数代表遇到的情况，第二个函数代表遇到这种情况的话，需要执行的函数
>
> 第一个参数常用的有
>
> ![image-20241205212057083](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/uPic/image-20241205212057083.png)

#### 2. `alarm()` 函数

> 就是上面也有说，信号的第一个参数就是遇到的情况，其中的 `SIGALRM` 就是和 `alarm()` 相关，所以就来具体介绍下这个函数
>
> ### **参数**
>
> - `seconds`
> 	- 要设置的定时器时间（秒）。
> 	- 如果传入 `0`，表示取消之前设置的定时器（如果有），此时不会触发任何 `SIGALRM` 信号
>
> ### **返回值**
>
> - 返回值：上一个未决定时器的剩余时间（秒）。
> 	- 如果之前没有设置过定时器，则返回 `0`。
> 	- 如果之前有一个定时器在运行，则返回该定时器剩余的时间。

```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

> ###  **使用注意事项**

1. **信号处理机制**
	- 通常需要结合 `signal()` 或 `sigaction()` 注册 `SIGALRM` 信号的处理程序，否则会导致默认行为（终止进程）。
2. **一次性触发**
	- `alarm()` 是**一次性触发**的定时器，定时器触发后会被清除。如果需要周期性触发，可以在信号处理程序中重新调用 `alarm()`。
3. **定时器覆盖**
	- 如果调用 `alarm()` 时已经有一个定时器在运行，新的调用会覆盖之前的定时器。
4. **分辨率**
	- 定时器的分辨率是秒级的，因此对于亚秒级的需求，可以使用其他更精确的定时器（如 `setitimer` 或 `clock_nanosleep`）

> ### 案例

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// 函数定的比较死， void ...(int 参)
void timeout(int sig) //信号处理器
{
    if (sig == SIGALRM)
        puts("Time out!");
    alarm(2); // 为了每隔 2 秒重复产生 SIGALRM 信号，在信号处理器中调用 alarm 函数
}
void keycontrol(int sig) //信号处理器
{
    if (sig == SIGINT)
        puts("CTRL+C pressed");
}

int main(int argc, char *argv[]) {
    int i;

    // 发生对应时间的时候，使用信号函数
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    // ...
    alarm(2); // 预约 2 秒候发生 SIGALRM 信号

    // 这个等待很长的，你用 ctrl + c 来触发 timeout 函数
    for (i = 0; i < 3; i++) {
        puts("wait...");
        sleep(100);
    }
    return 0;
}
```



#### 3. ==`sigaction()`== 函数

> 第三个参数是：通过此参数获取之前注册的信号处理函数指针，若不需要则传递 0

```c
#include <signal.h>

struct  sigaction {
	union __sigaction_u __sigaction_u;  /* signal handler */
	sigset_t sa_mask;               	/* signal mask to apply */
	int     sa_flags;               	/* see signal options below */
};

int sigaction(int signo, const struct sigaction * act, struct sigaction * oldact);
```

> 看使用案例来知道咋用就行

```c
void timout(int sig);	// 和上面保持一致，触发之后，每 2s 执行一个

int main(int argc, char *argv[]) {
    struct sigaction act;
    act.sa_handler = timeout;    //保存函数指针
    sigemptyset(&act.sa_mask);   //将 sa_mask 函数的所有位初始化成0
    act.sa_flags = 0;            //sa_flags 同样初始化成 0
    
    // 开启信号接受等待状态
    sigaction(SIGALRM, &act, 0);

    alarm(2); //2 秒后发生 SIGALRM 信号

    for (int i = 0; i < 3; i++) {
        puts("wait...");
        sleep(100);
    }
    return 0;
}
```

### ==1.6 linux 服务端的问题==

> 就是我们的 `linux` 服务器的 `sigaction` 所在的 `signal.h` 头文件中虽然有一个 `uion` 让我们用，但是仍然是可以使用 `.sa_handler` 的
>
> 另一个就是 `struct sigaction` 最后面有一些多余的红色部分，直接删除就可以，不影响使用的

- 类实现

```cc
class TCP_Server {
private:
    int _port;
    sockaddr_in _server_addr, _client_addr;
    int _server_sockfd, _client_sockfd;
    std::string buf;
    struct sigaction _act;
public:
    explicit TCP_Server(int port);

    TCP_Server(const TCP_Server &other) = delete;
    TCP_Server operator=(const TCP_Server &other) = delete;
    TCP_Server(TCP_Server&&) = delete;  // 禁止移动构造
    TCP_Server& operator=(TCP_Server&&) = delete;  // 禁止移动赋值

    void bind_addr_info();

    void start_listening();

    virtual void handle_with_client();

    ~TCP_Server() noexcept = default;
};
```

- 具体逻辑实现

```cc
//
// Created by Momo on 2024/11/21.
//

#include "Server.h"

void error_handling(const std::string &message) {
    // std::cerr 内容的输出
    std::cerr << message << std::endl;
    std::exit(1);
}

void read_child_proc(int sig) {
    pid_t PID;
    int status;

    PID = waitpid(-1, &status, WNOHANG);
    std::cout << "removed proc id " << PID << std::endl;
}

TCP_Server::TCP_Server(int port) : _port(port)
{
    this->buf.resize(1024);
    this->_server_addr.sin_family = AF_INET;
    this->_server_addr.sin_port = htons(this->_port);
    this->_server_addr.sin_addr.s_addr = INADDR_ANY;
    this->_server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 初始化信号处理函数
    memset(&this->_act, 0 ,sizeof(this->_act));
    this->_act.sa_handler = read_child_proc;
    sigemptyset(&this->_act.sa_mask);
    this->_act.sa_flags = 0;

    if (this->_server_sockfd == -1)
        error_handling("服务器初始化失败...\n");
    else
        std::cout << "服务器初始化完毕\n";
}

void TCP_Server::bind_addr_info() {
        if (bind(this->_server_sockfd, (sockaddr *)&this->_server_addr, 
                sizeof(this->_server_addr)) == -1) { error_handling("bind() Error"); }
        else {
            std::cout << "绑定成功\n";
        }
}

void TCP_Server::start_listening() {
        socklen_t client_addr_len = sizeof(_client_addr);
        if (listen(this->_server_sockfd, 5) == -1) {
            error_handling("listen error");
        }
        else {
            std::cout << "Starting Listen on Port : " 
            << this->_port << std::endl;
        }
}

void TCP_Server::handle_with_client(){
    while (true) {
        socklen_t client_addr_len = sizeof(this->_client_addr);
        this->_client_sockfd = accept(_server_sockfd, 
                                      (sockaddr *)&this->_client_addr, &client_addr_len);
        if (this->_client_sockfd == -1) { continue; }
        else {
            std::cout << "new clietn connected from - IP " 
                << inet_ntoa(this->_client_addr.sin_addr) 
                << " : "
                << " PORT "
                << ntohs(this->_client_addr.sin_port)
                << std::endl;   
        }

        //  捕捉子进程结束信号
        if (sigaction(SIGCHLD, &this->_act, 0) == -1) {
            close(this->_client_sockfd);
            std::cout << "sign 创建失败\n";
            continue;    
        } 
        
        pid_t PID = fork();
        // 创建子进程失败
        if (PID == -1) {
            close(this->_client_sockfd);
            continue;
        }
        if (PID == 0){
            close(this->_server_sockfd);
            // 处理客户端通信
            while (true) {
                auto recv_len = recv(_client_sockfd, buf.data(), buf.size(), 0);
                if (recv_len > 0) {
                    std::string reply(buf.data(), recv_len);
                    std::cout << "The Client says: " << reply << std::endl;
                    auto send_len = send(_client_sockfd, reply.c_str(), reply.size(), 0);
                    if (send_len == -1)
                        error_handling("Send Error");
                }
                else if (recv_len == 0) {
                    std::cout << "The Client disconnecting... \n";
                    break;
                }
                else error_handling("Recv Error");
            }
            close(_client_sockfd);
        }
        else close(this->_client_sockfd);
    }
}
```



