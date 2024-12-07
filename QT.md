# QT

---

> 参考视频

- [主要教学视频](https://www.bilibili.com/video/BV1N34y1H7x7?vd_source=b47817c1aa0db593f452034d53d4273a&spm_id_from=333.788.videopod.episodes)

<br>

## 1. 窗口的类别

![image-20241119203934343](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241119203934343.png)

### 1.1 `QMainWindow` & `QWidget` 的区别

> 带有菜单栏的就是 `QMainWindow`
>
> 不带有菜单栏的就是 `QWidget`

-  `QWidget`

![QWidget](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241125130506175.png)

-  `QMainWindow`

![QMainWindow](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241125130826368.png)

### 1.2 如何给窗口设置标题

> 在 `widget`  继承的类中，我们在对应的构造函数实现我们对窗口的一些基础属性的设置

```c++
calculator::calculator(QWidget *parent) :
        QWidget(parent), ui(new Ui::calculator) {
    ui->setupUi(this);

    // 固定窗口的最大最小尺寸（不可以拖拉改变大小）
    this->setMaximumSize(520, 570);
    this->setMinimumSize(520, 570);

    // 显示的窗口名字
    this->setWindowTitle("计算器");

    // 连接槽函
    connect(ui->oneButton, &QPushButton::clicked,
            this, &calculator::click_on_oneButton);
}
```



## 2. 信号和槽

---

### 2.1 QT 开辟进程

> 这里是如何使用 `QT` 创建一个程序

```c++
// 通过信号创建一个进程
Qprocess * myprocess = new QProcess(this);
myprocess->start(program, arguments[可选]);
```

### 2.2 如何创建一个事件和对应的槽

> 如何找到自己再 QT 里面创建的 UI 

```c++
// 可以使用 ui 去找到自己创建的类对象，具体的函数名就是自己创建的
ui->cmdline; 	// 找到对应的对象
```

![image-20241125191545300](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241125191545300.png)

> 我们在这个窗口最大的父类函数中开辟叫 `slots` 的私人变量空间
>
> 然后我们可以在这里创建对应的事件

```c++
// 这个在头文件中
class widget_demo : public QWidget {
Q_OBJECT

public:
    explicit widget_demo(QWidget *parent = nullptr);

    ~widget_demo() override;

private:
    Ui::widget_demo *ui;

// 就是这里这个槽空间
private slots:
    void click_on_commit_button();		// 这里就是我们想要执行的事件（信号）
};
```

> 在头文件声明了信号和槽，那么我们就要在具体的内部连接信号和槽函数
>
> 然后实现具体槽函数的效果

```c++
widget_demo::widget_demo(QWidget *parent) :
        QWidget(parent), ui(new Ui::widget_demo) {
    ui->setupUi(this);

    // 在这里我们把槽函数绑定给 commitButton 这个成员的 cliked 动作
    connect(ui->commitButton, &QPushButton::clicked, this, &widget_demo::click_on_commit_button);
}

widget_demo::~widget_demo() {
    delete ui;
}

// 在这里我们实现了具体的槽函数的效果：打开一个消息窗口
void widget_demo::click_on_commit_button() {
    QMessageBox::information(this, "Button Clicked", "You have clicked the commit button!");
}
```

### 2.3 `connect()` 的 lambda 写法

```c++
// 其实还可以还可以直接写一个 lambda 函数
connect(ui->brosewButton, &QPushButton::clicked,
        // 对的，我们其实可以直接 lambda 一步走，不再需要 this 了
        [this]() { QMessageBox::information(this, "Information", "Click to Browse"); }
   );
```

## 3. `LineEdit`

---

### 3.1 `LineEdit` 显示输出

```cpp 
// 你可以使用 ui 命名空间去找到 你在 UI 界面创建的类的名字

// 我们的数据
ui->displayLine->setText(expression);	// displayLine 就是我们的 LineEdit
```

## 4. `PushButton`

---

### 4.1 `PushButton `改变背景色

```cpp 
```

## 5. `QTimer`

> 这个是 `QT` 自带的一种定时器， 在 `QTObject` 里面，但是它也有一个单独的类，所以我们可以直接使用或者类实现定时器的效果

- 创建一个定时器

```cpp
// 在 Qwidget 窗口中创建一个定时器，记得定时器需要在构造函数中创建哈
auto * timer = new QTimer;
```

- 这个定时器是每间隔一个 `TIMEOUT` 的时间间隔就执行一个 `updateImage `函数

```cpp
// 连接定时器的timeout信号
connect(timer, &QTimer::timeout, this, &Timer::updateImage);
```

- 开始和停止都非常的简单，只需要用 `timer` 的成员函数 `start()` 和 `stop()` 就可以了

- 当然计时器也是可以只执行一次的

```cpp
QTimer::singleShot(TIMEOUT, this, &Timer::updateImage);
```

<br>

## 6.`QT`  文件操作

---

### 6.1  获取文件信息

> 我们 `Windows` 上面那种常见的打开文件的对话框

<img src="https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241205130647418.png" alt="image-20241205130647418" style="zoom:88%;" />

> 打开页面的对话框已经封装好了，所以我们只用调用就可以了
>
> ### 总结：
>
> - `parent`：指定对话框的父窗口。
> - `caption`：设置对话框的标题。
> - `dir`：设置对话框初次打开时的目录。
> - `filter`：文件类型过滤器，用于限制显示的文件类型。
> - `selectedFilter`：获取用户选择的过滤器。
> - `options`：控制对话框的额外行为和外观的选项。
>
> ### **返回值**
>
> 如果你真的选了一个文件，其实会返回一个路径来的

```cpp
#include <QFileDialog>

static QString getOpenFileName(
	QWidget *parent = nullptr,
	const QString &caption = QString(), 
	const QString &dir = QString(),
  	const QString &filter = QString(),
  	QString *selectedFilter = nullptr,
   	Options options = Options()
);
```

> 一般的调用函数

```cpp
// 这里是 home 的路径    
QFileDialog::getOpenFileName(this, tr("选择一个文件"), QDir::homePath(), "*.*");
// 这里是 current 路径
QFileDialog::getOpenFileName(this, tr("选择一个文件"), 
                             QCoreApplication::applicationDirPath(), 
                             "*.*");
// 我们来限制只要的文件的类型
QFileDialog::getOpenFileName(this, tr("选择一个文件"), 
                             QCoreApplication::applicationDirPath(), 
                             "*.cpp");
```

> 限制了之后就只有 `.cpp` 文件才可以被选中了

![image-20241205132430517](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241205132430517.png)

### 6.2 打开文件返回值处理

```cpp
auto fileName =  QFileDialog::getOpenFileName(this, tr("打开"),
    QCoreApplication::applicationDirPath(),
    "*.cpp");

// 如果取消了，返回值就是空的
if (fileName.isEmpty()) {
    QMessageBox::warning(this, "警告", "请选择一个文件");
}
else {
    qDebug() << fileName;	// 一般就就是路径
}
```

### 6.3 读取文件的内容

```cpp
QFile file(fileName);		// 创建文件对象
// 设置文件为只读和
if (file.open(QFile::ReadOnly | QFile::Text)) {
    QTextStream stream(&file);
    auto content = stream.readAll();

    // 把读取到的东西全部输出到 Text 上面
    ui->textEdit->setText(content);
}
else {
    qDebug() << fileName;
    QMessageBox::warning(this, "错误", "无法打开文件进行读取");
}
file.close();
```

### 6.4 保存文件

> 把 `textEdit` 的内容保存为一个文件存储

```cpp
// 判断 textEdit 是否不为空，如果是的话，就另存其内容为一个新的文件
if (ui->textEdit->toPlainText().isEmpty()) {
    QMessageBox::warning(this, "警告", "当前文本内容为空无法另存为其他文件!");
    return;
}

// 打开一个另存为选择框
auto fileName = QFileDialog::getSaveFileName(this, tr("另存为"), QDir::homePath());

// 如果文件名啥也没有，就返回一个空
if (fileName.isEmpty()) {
    QMessageBox::warning(this, "警告", "请选择一个文件");
} else {
    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
    } else {
        QMessageBox::warning(this, tr("错误"), tr("无法保存文件"));
    }
    file.close();
}
```

<br>

## 7. QT `Event()`

---

> 在 UI 界面的类函数创建一个键盘事件的触发函数
>
> 事件触发的类型非常的多，具体的话，还是去看文档哈c

```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
	
    // 键盘对应事件的函数的就创建出来了
    void keyPressEvent(QKeyEvent *k_event) override;	// 就是这里

    ~MainWindow() override;

private slots:
    void newActionTriggered();
    void openActionTriggered();
    void saveActionTriggered();

private:
    Ui::MainWindow *ui;
};
```

> 函数中做的事情

```cpp
void MainWindow::keyPressEvent(QKeyEvent *k_event) {
    // 如果键盘按下 ctrl + N
    if (k_event->modifiers() == Qt::ControlModifier && k_event->key() == Qt::Key_N) { this->newActionTriggered(); }
    // 如果键盘按下 ctrl + O
    if (k_event->modifiers() == Qt::ControlModifier && k_event->key() == Qt::Key_O) { this->openActionTriggered(); }
    // 如果键盘按下 ctrl + S
    if (k_event->modifiers() == Qt::ControlModifier && k_event->key() == Qt::Key_S) { this->saveActionTriggered(); }
}
```



## 8. 客户端和服务端的通信

---

> **客户端**主要是，套接字的创建，和发送请求

```cpp
// 创建套接字
this->socket = new QTcpSocket();

// 外部交互获取 IP 和 PORT
auto IP = ui->ip_lineEdit->text();
auto PORT = ui->port_lineEdit->text();

// 连接服务器
this->socket->connectToHost(QHostAddress(IP), PORT.toShort());

/* 请求成功和失败的处理 */

// 连接服务器成功
connect(this->socket, &QTcpSocket::connected, [this]() {
    QMessageBox::information(this, "Server", "Connected");
});
// 连接失败
connect(this->socket, &QTcpSocket::disconnected, [this]() {
    QMessageBox::information(this, "Server", "Disconnected");
});
```

> **服务端**

```cpp
this->server = new QTcpServer;	// 先创建服务端，而不是套接字

// 开启监听
server->listen(QHostAddress::AnyIPv4, 10000);

// 获取客户端套接字
auto *socket = server->nextPendingConnection();
auto IP = socket->peerAddress().toString(); // 获取客户端地址
auto PORT = socket->peerPort(); // 获取客户端端口号
```



## Ex. 环境配置

---

### 1. `CMakeLists.txt` 配置

> `QT` 所有需要的 `dll`和 `xxx.h` 都在 `D:/App/QT/6.8.1/mingw_64` 的两个目录下
>
> - `bin`：所有 `dll `所在的位置
> - `lib/cmake`：所有头文件所在的位置

```cmake
set(QT_PATH "D:/App/QT/6.8.1/mingw_64")
link_directories("${QT_PATH}/bin")                      # 第三方库的所在位置
set(CMAKE_PREFIX_PATH "${QT_PATH}/lib/cmake")           # 头文件库的所在位置
```

> 我们一般用这两个方法导入，我们需要的头文件和对应的**源文件**（动态库）
>
> 但是链接动态文件实际上并没有导入到 `cmake-build-debug`

![image-20241207133555892](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241207133555892.png)

> 解决

```cmake
# 将 Qt6Network.dll 复制到目标目录
add_custom_command(TARGET Server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QT_PATH}/bin/Qt6Network.dll"		# 直接塞入 cmake-build-debug
        $<TARGET_FILE_DIR:Server>)
```

### 2. ==编译出来的文件在外部可运行的配置==

> 设置了这些之后我们在外部的 exe 文件才可以独立运行

![image-20241207145759288](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241207145759288.png)

### 3. 如何分文件编译

> 我们文件的分布如下，然后分开的文件如何汇编到一起呢？

```shell
QT_Client/
├── CMakeLists.txt          # 顶层 CMake 文件
├── src/
│   ├── main.cpp
│   └── CMakeLists.txt      # 子目录 CMake 文件
├── include/
│   ├── client.h
│   └── form.h
└── UI/
    ├── client.ui
    └── form.ui
```

> 对应的配置为
>
> `CMAKE_SOURCE_DIR`：这个参数显示的位置就是 `CMakeLists.txt` 所在的文件夹的位置

```cmake
# 设置 AUTOUIC 搜索路径
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





