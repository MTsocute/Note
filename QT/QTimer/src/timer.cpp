//
// Created by shuhe on 2024/12/1.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Timer.h" resolved

#include "timer.h"
#include "ui_Timer.h"


Timer::Timer(QWidget *parent) :
    QWidget(parent), ui(new Ui::Timer) {
    ui->setupUi(this);
}

Timer::~Timer() {
    delete ui;
}
