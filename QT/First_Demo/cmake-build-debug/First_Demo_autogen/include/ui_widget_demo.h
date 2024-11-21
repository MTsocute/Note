/********************************************************************************
** Form generated from reading UI file 'widget_demo.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_DEMO_H
#define UI_WIDGET_DEMO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_widget_demo
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *widget_demo)
    {
        if (widget_demo->objectName().isEmpty())
            widget_demo->setObjectName("widget_demo");
        widget_demo->resize(400, 311);
        pushButton = new QPushButton(widget_demo);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(90, 100, 211, 101));

        retranslateUi(widget_demo);

        QMetaObject::connectSlotsByName(widget_demo);
    } // setupUi

    void retranslateUi(QWidget *widget_demo)
    {
        widget_demo->setWindowTitle(QCoreApplication::translate("widget_demo", "widget_demo", nullptr));
        pushButton->setText(QCoreApplication::translate("widget_demo", "this is for widget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class widget_demo: public Ui_widget_demo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_DEMO_H
