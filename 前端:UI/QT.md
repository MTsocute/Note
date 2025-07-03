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

### 1.3  如何给窗口添加 ICON 

> 文件统一存放到我们的 `.qrc` 文件中，然后可以使用代码挂载 ICON，或者说使用 UI 文件
>
> 关于什么是 qrc 文件，和如何配置，请具体看我们的 17

- 代码：有些类型可能找不到，就用这个命令

```cpp
QIcon icon;
icon.addFile(QString::fromUtf8(":/img/chat_ico.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
loginDialog->setWindowIcon(icon);
```

- UI 文件

![image-20250106121347269](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250106121347269.png)

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

### ==2.4 伪闭包== 

> 我们按照以 `connect` 为例

```cpp
// this 传入
QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
    emit this->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
    reply->deleteLater();
});
```

> 其实在信号和槽中，如果直接使用 `this` 指针，将会存在潜在的空指针问题，这个 this 实际上成为了一个空指针，再次使用程序就直接崩溃
>
> 为了防止这个问题，我们就是用 shared_from_this 方法，你可以理解为造了两个 this，传入的是赋值的，那么 `share_ptr` 计数就是 2，少了一个也没不会提前释放

```cpp
// 你当前用的这个类，必须得继承 enable_shared_from_this<>
class A : public enable_shared_from_this<A> {
    auto self = shared_from_this();
    // 正确处理, self 传入
    QObject::connect(reply, &QNetworkReply::finished, [reply, self, req_id, mod](){
        emit self->sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
        reply->deleteLater();
});
}
```



## 3. `LineEdit`

---

### 3.1 `LineEdit` 显示输出

```cpp 
// 你可以使用 ui 命名空间去找到 你在 UI 界面创建的类的名字

// 我们的数据
ui->displayLine->setText(expression);	// displayLine 就是我们的 LineEdit
```

### 3.2 输入内容的模式

> `QLineEdit` 提供了四种主要的输入模式，分别是：
>
> 1. **`Normal`**: 正常显示输入的文本。
> 2. **`NoEcho`**: 输入的文本不显示。
> 3. **`Password`**: 输入的字符显示为掩码符号。
> 4. **`PasswordEchoOnEdit`**: 输入时为掩码，失去焦点后显示实际字符

## 4. `PushButton`

---

### 4.1 `PushButton `改变背景色

> 我们的 QT Designer 里面可以设置一些 CSS 来改变当前的构建

![image-20241213200531822](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213200531822.png)

> 在弹出来的页面，我们书写对应的 CSS 语法即可

![image-20241213200608122](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241213200608122.png)

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

> 图片还特殊些，所以我们这里记录一下

```cpp
    auto path = QFileDialog::getOpenFileName(this, "打开图片", imagePath,
        tr("Images(*.png *.jpg *.bmp *jpeg)"));	// 注意看 tr 里面的内容是 Images 哈
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

### 6.5 使用手册查找系统定义文件的路径

![image-20241216174648925](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241216174648925.png)

<br>

## 7. QT `Event()`

---

> 在 UI 界面的类函数创建一个键盘事件的触发函数
>
> 事件触发的类型非常的多，具体的话，还是去看文档哈

```cpp
#include <QKeyEvent>

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

### 8.1 客户端发送内容给服务器

### 8.2 服务端接受发送的内容

## 9. 处理带参自定义信号

> 自定义的信号要靠着 emit 函数来触发，譬如说：`emit this->eat();`

```cpp
class Person : public QObject {
    Q_OBJECT
public:
    explicit Person(QObject * parent = nullptr);
    ~Person() override = default;

signals:
    void eat();
    // 这里重构了一个带参数的信号函数，connect 如何分别两种并处理?
    void eat(const QString & food);
};
```

> 处理方法
>
> 我们建立两个对象的类方法成员函数指针，然后再 `connet` 实例和对应的函数指针实现绑定

```cpp
// TODO: How to deal with two kinds of signals
// create two kinds of class function's function pointer
void (Person::*eat)() = &Person::eat;
void (Person::*eat_food)(const QString &food) = &Person::eat;

// slot for function with parameter
void (House::*house_cook)() = &House::do_cook;
void (House::*house_cook_food)(const QString &food) = &House::do_cook;

// bind slots with corresponding signals
connect(person, eat, house, house_cook);
connect(person, eat_food, house, house_cook_food);
```

## ==10. 关闭窗口默认为关闭的操作==

> 我们打开一个新的窗口的时候保证其一直运行，所以会 new
>
> 但是实际上我们关闭 QT 窗口并不会实际关闭，所以 new 类的析构只有等待所有释放了才释放
>
> 那么我们如何设置 QT 关闭窗口是真正的关闭呢？

```cpp
void MainWidget::openSigSlot() {
    auto * base = new MainWindow(this);
    base->setAttribute(Qt::WA_DeleteOnClose);   // 关闭窗口就是认为删除
    base->show();
}
```

## 11. 当一个窗口`A`打开另一个窗口`B`的时候，如何让窗口 `A` 无法被点击

```cpp
void MainWidget::openSigSlot() {
    auto * base = new MainWindow(this);
    base->setAttribute(Qt::WA_DeleteOnClose);   // 关闭窗口就是认为删除
    base->show();
    // 当前窗口不可用
    this->setEnable(false);		// this->setDisable(true);
}
```

> 但是上面的也会造成另一个问题，就是就算我们关闭了 B 窗口，但是 A 还是不可以用，所以我们要关闭 B 的时候，让 A 可以用，这里就涉及到了新的信号 `destroyed`
>
> **但是别拿这个信号作为释放子窗口内存的方法哈，没用的**

```cpp
void MainWidget::openSigSlot() {
    auto *base = new MainWindow;
    base->setAttribute(Qt::WA_DeleteOnClose);
    // 如果子窗口关闭通知 MainWidget 窗口
    connect(base, &QWidget::destroyed, [this]() {
        this->setEnabled(true);
    });
    base->show();
    this->setEnabled(false);
}
```

> 还可以搭配其他的来用 `setVisible()`

## 12. 如何让一个窗口显示到最前面

> 譬如说前面的隐藏操作，在显示的时候就会遇到藏在最后面的问题，这个可以解决那个问题

```cpp
setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
raise(); // for MacOS
activateWindow(); // for Windows
```

## 13. `OpenCV` 

---

### 1. OpenCV 如何解决中文路径文件打开问题

```cpp
this->m_scr_image = cv::imread(path.toLocal8Bit().toStdString());
```

### 2. 通过 cv::Mat 获取的图片转换 `QImage`

```cpp
const auto &image = QImage(
    result_Array.data,
    result_Array.cols, result_Array.rows,			// 这里不多说，就是获取 图片矩阵的大小
    result_Array.channels() * result_Array.cols(),	 // RBG 有三个色块，然后一列 * 三个色块，才是这一列的具体大小
    QImage::Format_RGB888
);

// 显示图片在 label 里面
label->setPixmap(QPixmap::fromImage(image));
```

### 3. 根据其他对象的大小修改合适的比例

```cpp
QImage ImageWidget::imageCenter(const QImage &image, QLabel *label) {
    QImage temp;
    QSize imageSize = image.size();
    QSize labelSize = label->size();

    // 通过他们的比例来缩放
    double widthRadio = static_cast<double>(imageSize.width()) / labelSize.width();
    double heightRadio = static_cast<double>(imageSize.height()) / labelSize.height();
    
    // 图片缩放比例，只可以有一个标准，所以我们让比较大的作为整体缩放尺度
    if (widthRadio > heightRadio) {
        temp = image.scaledToWidth(label->width());
    }
    else {
        temp = image.scaledToHeight(label->height());
    }
    return temp;
}
```

> 但是这个涉及到一个问题，就是他只会在图片载入的时候调整大小，如果我们拖动了窗就还是那么大小，**希望动态的改变大小的话**，我们就需要 `resize` 这个事件（event）
>
> 记得给设定最小尺寸，对于多空间排布是有好处的



## 14. `QSettings`

> `QSettings` 是 Qt 提供的一个功能强大的类，用于存储和检索应用程序的配置信息，例如用户设置、窗口布局、应用状态等。它支持跨平台的持久化配置存储，可以将数据保存到多种后端存储中（如 Windows 注册表、INI 文件、JSON 等），并在程序启动时轻松恢复这些设置

### 1. 初始化一个配置文件 ini

```cpp
// 前面的部分就是自动找到 QT 所在的运行路径
auto path = qApp->applicationDirPath() + this->CONFIG_FILE_NAME;
// 如果你传入 this 的话，那么就不需要再析构函数里面释放了
this->m_pIniSet = new QSettings(path, QSettings::IniFormat, this);	// 创建一个 ini 的操作类实例
```

### 2. `QSettings.value() `读取配置

> - `"LastPath"` 是键名，用来标识配置文件中的一个数据项
> -  `"path"` 是默认值。如果配置文件中没有 `LastPath`，就会返回 `path`，没写就返回 "" 空串
>
> 这个函数用于读键返回的值和 `json` 一个道理的

```cpp
this->m_lastPath = m_pIniSet->value("LastPath", path).toString();
```

### 3. `QSettings.setValue()` 创建键值对

```cpp
// 写入配置
settings.setValue("username", "Momo");
settings.setValue("age", 30);
```



## 15.  `Combox`

---

### 1. 一个 `ComBox` 的内容指定另一个可用于与否的联动

> 如下效果

![image-20250106144957019](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250106144957019.png)

```cpp
// 一开始我们都控制住
if (ui->recvModeCBox->currentIndex() == 0) {
    ui->recvModeEncode->setEnabled(false);
} else if (ui->recvModeCBox->currentIndex() == 1) {
    ui->recvModeEncode->setEnabled(true);
}

if (ui->sendModeCBox->currentIndex() == 0) {
    ui->sendModeEncode->setEnabled(false);
} else if (ui->sendModeCBox->currentIndex() == 1) {
    ui->sendModeEncode->setEnabled(true);
}

// 我们使用这个来解放控制权
// slot 函数处理和上面是一样的
connect(ui->recvModeCBox, &QComboBox::currentIndexChanged, this, &SerialHelper::slot_hex_tex_mode_modified);
connect(ui->sendModeCBox, &QComboBox::currentIndexChanged, this, &SerialHelper::slot_hex_tex_mode_modified);
```

## 16. `Qss` 美化

> 配置 qss，qss 文件书写规则和 css 差不多

```cpp
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Qss 美化
    QFile qss(":/style/style.qss");

    if (qss.open(QIODevice::ReadOnly)) {
        qDebug() << "qss Open";
        QString styple = QLatin1String(qss.readAll());
        a.setStyleSheet(styple);
        qss.close();
    } else {
        qDebug() << "qss Open Failed";
    }

    return a.exec();
}

```



## 17. qrc 资源文件

---

> qrc 就是 QT 中管理资源路径的文件，有这个确定过文件路径之后，我们大部分图片等资源，就可以使用这个一同管理
>
> 先看下我们的文件结构构成
>
> - prefix就是前缀的虚拟文件夹名，在代码中引用规则为:虚拟文件夹名/文件路径
> - file 就是这个虚拟文件夹下的文件，所以要对应实际的路径

```html
<RCC>
    <qresource prefix="/image">
        <file>resources/qss/flatgray/1.png</file>
        <file>resources/qss/flatgray/2.png</file>
        <file>resources/qss/flatgray/3.png</file>

    </qresource>
    
    <qresource prefix="/qss">
        <file>resources/qss/flatgray.qss</file>
    </qresource>

</RCC>
```



## Ex. 环境配置

---

### 1. `CMakeLists.txt` 配置

```cmake
# dll 所在位置
set(CMAKE_PREFIX_PATH "path/to/msvc2022_64")

# 找包
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED
)
# 导库  
target_link_libraries(demo
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)
# 在 win 的部分写入你需要的库存
foreach (QT_LIB Core Gui Widgets Network)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
```

### 2. ==exe 在外部可运行的配置==

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

### 4. 外部第三方库导入方法

- 我们使用的 VS 的MSVC 的编译环境和对应的配置

- VCPKG 管理第三方库的安装，版本都是 x64-windows

- ```cmd
    # 记得使用下面命令，集成化管理库
    vcpkg integrate install
    ```

> `Clion`

![image-20241217184325236](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241217184325236.png)

> `CMake optitions` 里面的内容

```cmake
-DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

> `QT Creator` 里面的内容

![image-20241217184501535](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20241217184501535.png)

> 然后接下来，我们就剩下两个步骤了，find_package 和 target_link_lib
>
> 但是很多时候都不知道对应的名字叫啥咋办呢
>
> 其实我们可以先去 `installed/x64-windows/share/<triple_lib>` 找对应的第三方库
>
> 在上述路径中，你会找到 `<triple_lib>Config.cmake` 或类似的文件。打开这个文件，你通常会在文件头部找到 `find_package` 所需的名称

### 5.`Qt6` 如何导入QT的库

> https://blog.csdn.net/m0_61629312/article/details/133445578

```cmake
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)

target_link_libraries(ChatRoom PRIVATE Qt${QT_VERSION_MAJOR}::Network)   
```

### 6. 解决 exe 运行出现的窗口问题

![image-20250120022524260](https://cdn.jsdelivr.net/gh/MTsocute/New_Image@main/img/image-20250120022524260.png)

> 对于这个问题，在编译 exe 的时候 加入 Win32 就行
>
> 但是会带来一个 bug 就是使用 qt 输出内容到终端这个功能会再 clion 中不可见，但是如果我们真的打包了程序的时候，也确实不需要输出内容到终端了，所以再 release 的时候自己写一个宏定义就好了

