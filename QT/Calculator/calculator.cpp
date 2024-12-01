//
// Created by shuhe on 2024/11/27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_calculator.h" resolved

#include "calculator.h"
#include "ui_calculator.h"
#include <QTimer>

// 禁用 Qt 的 slots 宏
#undef slots
#include <Python.h>  // 包含 Python 头文件
// 恢复 Qt 的 slots 宏
#define slots Q_SLOTS


calculator::calculator(QWidget *parent) : QWidget(parent), ui(new Ui::calculator) {
    ui->setupUi(this);

    // 固定截面的最大最小尺寸
    this->setMaximumSize(520, 570);
    this->setMinimumSize(520, 570);

    // 显示的窗口名字
    this->setWindowTitle("计算器");

    // 连接槽和信号
    auto connect_key_with_num = [this]() {
        connect(ui->zeroButton, &QPushButton::clicked,
                [this]() { expression += "0"; ui->displayLine->setText(expression); });
        connect(ui->oneButton, &QPushButton::clicked,
                [this]() { expression += "1"; ui->displayLine->setText(expression); });
        connect(ui->twoButton, &QPushButton::clicked,
                [this]() { expression += "2"; ui->displayLine->setText(expression); });
        connect(ui->threeButton, &QPushButton::clicked,
                [this]() { expression += "3"; ui->displayLine->setText(expression); });
        connect(ui->fourButton, &QPushButton::clicked,
                [this]() { expression += "4"; ui->displayLine->setText(expression); });
        connect(ui->fiveButton, &QPushButton::clicked,
                [this]() { expression += "5"; ui->displayLine->setText(expression); });
        connect(ui->sixButton, &QPushButton::clicked,
                [this]() { expression += "6"; ui->displayLine->setText(expression); });
        connect(ui->sevenButton, &QPushButton::clicked,
                [this]() { expression += "7"; ui->displayLine->setText(expression); });
        connect(ui->eightButton, &QPushButton::clicked,
                [this]() { expression += "8"; ui->displayLine->setText(expression); });
        connect(ui->nineButton, &QPushButton::clicked,
                [this]() { expression += "9"; ui->displayLine->setText(expression); });
        connect(ui->clearButton, &QPushButton::clicked,
                [this]() { expression.clear(); ui->displayLine->setText(expression); });
        connect(ui->plusButton, &QPushButton::clicked,
                [this]() { expression += "+"; ui->displayLine->setText(expression); });
        connect(ui->minusButton, &QPushButton::clicked,
                [this]() { expression += "-"; ui->displayLine->setText(expression); });
        connect(ui->multiplyButton, &QPushButton::clicked,
                [this]() { expression += "*"; ui->displayLine->setText(expression); });
        connect(ui->divideButton, &QPushButton::clicked,
                [this]() { expression += "/"; ui->displayLine->setText(expression); });
        connect(ui->leftBracketButton, &QPushButton::clicked,
                [this]() { expression += "("; ui->displayLine->setText(expression); });
        connect(ui->rightBracketButton, &QPushButton::clicked,
                [this]() { expression += ")"; ui->displayLine->setText(expression); });
        connect(ui->delButton, &QPushButton::clicked,
                [this]() { expression.chop(1); ui->displayLine->setText(expression); });
    };
    connect_key_with_num();

    // delButton 用贴图
    QIcon con("D:\\Code\\QT\\Calculator\\img\\back_space.png");
    ui->delButton->setIcon(con);

    // 绑定 equal 函数
    connect(ui->equalButton, &QPushButton::clicked,
            this, &calculator::click_on_equalButton);
}

calculator::~calculator() {
    delete ui;
}

void calculator::click_on_equalButton() {
    // 设置 equalButton 的背景颜色 和 字的颜色
    ui->equalButton->setStyleSheet("background-color: blue; color: white;");

    // 计算算式的表达式
    std::string Expr = this->expression.toStdString();
    auto result = use_pycal(Expr);
    this->expression = QString::number(result);     // 转换 result 为我们新的数据
    // 更新数据为计算的结果
    ui->displayLine->setText(this->expression);

    // 延迟一段时间之后变回来
    QTimer::singleShot(80, [this]() {
        ui->equalButton->setStyleSheet("background-color: ""; color: white;");
    });
}

void config_path(const std::string &scriptPath) {
    if (fs::exists(scriptPath)) {
        // 动态添加路径到 sys.path
        std::string customPath = scriptPath;
        std::string pythonCode = "import sys; sys.path.append('" + customPath + "')";
        PyRun_SimpleString(pythonCode.c_str());
    }
    else { std::cout << "File Path doesn't exist" << std::endl; }
}

double use_pycal(const std::string & Expr) {
    double result = 0;
    // 初始化 Python 解释器
    Py_Initialize();

    // 检查 Python 是否初始化成功
    if (!Py_IsInitialized()) {
        std::cerr << "Python Init Failed！" << std::endl;
    }

    try {
        // 动态添加路径到 sys.path
        config_path(scriptPath);

        // 导入 Python 模块 (cal.py)
        PyObject* pModule = PyImport_ImportModule("cal");
        if (!pModule) {
            std::cerr << "Unable to load 'cal'" << std::endl;
            PyErr_Print(); // 打印 Python 错误信息
        }

        // 获取 py_cal 函数
        PyObject* pFunc = PyObject_GetAttrString(pModule, "py_cal");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            std::cerr << "'py_cal' unable to use" << std::endl;
            Py_XDECREF(pModule);
        }

        // 构造参数
        PyObject* pArgs = PyTuple_Pack(1, PyUnicode_FromString(Expr.c_str()));

        // 调用函数，并获取返回值
        PyObject* pValue = PyObject_CallObject(pFunc, pArgs);

        // 检查返回值并获取结果
        if (pValue != nullptr) {
            result = PyFloat_AsDouble(pValue);
            std::cout << "Result: " << result << std::endl;
            Py_DECREF(pValue);
        }
        else {
            std::cerr << "Function failed to use！" << std::endl;
            PyErr_Print(); // 打印 Python 错误信息
        }

        // 释放资源
        Py_XDECREF(pArgs);
        Py_XDECREF(pFunc);
        Py_XDECREF(pModule);
    }

    catch (...) {
        std::cerr << "Error！" << std::endl;
    }

    // 关闭 Python 解释器
    Py_Finalize();

    return result;
}
