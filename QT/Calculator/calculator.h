//
// Created by shuhe on 2024/11/27.
//

#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <QWidget>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

// 指定路径
const std::string scriptPath = "D:/Code/QT/Calculator/script/";

// 指定路径到某一函数位置
void config_path(const std::string& scriptPath);

// 调用我们指定的 Py 函数
double use_pycal(const std::string & Expr);


QT_BEGIN_NAMESPACE
namespace Ui { class calculator; }
QT_END_NAMESPACE

class calculator : public QWidget {
Q_OBJECT

public:
    explicit calculator(QWidget *parent = nullptr);

    ~calculator() override;

private slots:
    void click_on_equalButton();    // 只对 = 做特殊处理

private:
    Ui::calculator *ui;
    QString expression;
};

#endif //CALCULATOR_CALCULATOR_H
