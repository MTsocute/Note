//
// Created by shuhe on 2024/12/1.
//

#ifndef TIMER_H
#define TIMER_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Timer; }
QT_END_NAMESPACE

class Timer : public QWidget {
Q_OBJECT

public:
    explicit Timer(QWidget *parent = nullptr);
    ~Timer() override;

private:
    Ui::Timer *ui;
};


#endif //TIMER_H
