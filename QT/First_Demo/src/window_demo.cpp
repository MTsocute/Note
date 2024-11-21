//
// Created by shuhe on 2024/11/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Window_Demo.h" resolved

#include "window_demo.h"
#include "ui_Window_Demo.h"
#include "widget_demo.h"


Window_Demo::Window_Demo(QWidget *parent) :
        QWidget(parent), ui(new Ui::Window_Demo), wget(new widget_demo) {
    ui->setupUi(this);
    // 在创建 WinDemo 对象的时候同时召唤 widget 对象
    wget->show();
}

Window_Demo::~Window_Demo() {
    delete ui;
    delete wget;
}
