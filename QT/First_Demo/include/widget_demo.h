//
// Created by shuhe on 2024/11/19.
//

#ifndef FIRST_DEMO_WIDGET_DEMO_H
#define FIRST_DEMO_WIDGET_DEMO_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class widget_demo; }
QT_END_NAMESPACE

class widget_demo : public QWidget {
Q_OBJECT

public:
    explicit widget_demo(QWidget *parent = nullptr);

    ~widget_demo() override;

private:
    Ui::widget_demo *ui;
};


#endif //FIRST_DEMO_WIDGET_DEMO_H
