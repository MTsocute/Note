//
// Created by shuhe on 2024/11/19.
//

// You may need to build the project (run Qt uic code generator) to get "ui_widget_demo.h" resolved

#include "widget_demo.h"
#include "ui_widget_demo.h"


widget_demo::widget_demo(QWidget *parent) :
        QWidget(parent), ui(new Ui::widget_demo) {
    ui->setupUi(this);
}

widget_demo::~widget_demo() {
    delete ui;
}
