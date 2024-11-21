/********************************************************************************
** Form generated from reading UI file 'window_demo.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_DEMO_H
#define UI_WINDOW_DEMO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Window_Demo
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *Window_Demo)
    {
        if (Window_Demo->objectName().isEmpty())
            Window_Demo->setObjectName("Window_Demo");
        Window_Demo->resize(400, 299);
        pushButton = new QPushButton(Window_Demo);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(90, 90, 241, 111));

        retranslateUi(Window_Demo);

        QMetaObject::connectSlotsByName(Window_Demo);
    } // setupUi

    void retranslateUi(QWidget *Window_Demo)
    {
        Window_Demo->setWindowTitle(QCoreApplication::translate("Window_Demo", "Window_Demo", nullptr));
        pushButton->setText(QCoreApplication::translate("Window_Demo", "\351\201\245\346\216\247\345\274\200\345\205\263", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Window_Demo: public Ui_Window_Demo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_DEMO_H
