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

## 3. LineEdit

### 3.1 `LineEdit` 显示输出

```cpp 
// 你可以使用 ui 命名空间去找到 你在 UI 界面创建的类的名字

// 我们的数据
ui->displayLine->setText(expression);	// displayLine 就是我们的 LineEdit
```

## 4. PushButton

### 4.1 `PushButton `点击改变背景色



## Ex. 环境配置

### 1. CMakeLists.txt 配置编译的路径

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 这个用于追踪我们需要的路径
set(CMAKE_PREFIX_PATH "D:/App/QT/6.8.0/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
```

