//
// Created by shuhe on 2024/11/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_widget_demo.h" resolved

#include "widget_demo.h"
#include "ui_widget_demo.h"


widget_demo::widget_demo(QWidget *parent) :
        QWidget(parent), ui(new Ui::widget_demo) {
    ui->setupUi(this);

    // 槽函数绑定信号
    connect(ui->commitButton, &QPushButton::clicked,
            this, &widget_demo::click_on_commit_button1);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &widget_demo::click_on_cancel_button);
    // 其实还可以还可以直接写一个 lambda 函数
    connect(ui->brosewButton, &QPushButton::clicked,[this]() {
        QMessageBox::information(this, "Information", "Click to Browse");
    });
}

widget_demo::~widget_demo() {
    delete ui;
}

void widget_demo::click_on_commit_button1() {
    QMessageBox::information(this, "Button Clicked", "You have clicked the commit button!");
}

void widget_demo::click_on_commit_button2() {
    QString program = ui->cmdLineEdit->text();
    QProcess * myprogram = new QProcess(this);
    myprogram->start(program);
}

void widget_demo::click_on_cancel_button() {
    this->close();
}

