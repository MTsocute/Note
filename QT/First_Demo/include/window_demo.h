//
// Created by shuhe on 2024/11/19.
//

#ifndef FIRST_DEMO_WINDOW_DEMO_H
#define FIRST_DEMO_WINDOW_DEMO_H

#include <QWidget>
#include <memory>
#include "widget_demo.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Window_Demo; }
QT_END_NAMESPACE

class Window_Demo : public QWidget {
Q_OBJECT

public:
    explicit Window_Demo(QWidget *parent = nullptr);

    ~Window_Demo() override;

private:
    Ui::Window_Demo *ui;
    widget_demo *wget;
};


#endif //FIRST_DEMO_WINDOW_DEMO_H
