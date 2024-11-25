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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_widget_demo
{
public:
    QLabel *label;
    QLabel *label_2;
    QLineEdit *cmdLineEdit;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *commitButton;
    QPushButton *cancelButton;
    QPushButton *brosewButton;

    void setupUi(QWidget *widget_demo)
    {
        if (widget_demo->objectName().isEmpty())
            widget_demo->setObjectName("widget_demo");
        widget_demo->resize(400, 305);
        label = new QLabel(widget_demo);
        label->setObjectName("label");
        label->setGeometry(QRect(170, 50, 121, 31));
        QFont font;
        font.setFamilies({QString::fromUtf8("Microsoft YaHei")});
        font.setPointSize(14);
        label->setFont(font);
        label_2 = new QLabel(widget_demo);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(60, 120, 52, 21));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Microsoft YaHei")});
        font1.setPointSize(12);
        label_2->setFont(font1);
        cmdLineEdit = new QLineEdit(widget_demo);
        cmdLineEdit->setObjectName("cmdLineEdit");
        cmdLineEdit->setGeometry(QRect(150, 120, 181, 31));
        widget = new QWidget(widget_demo);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(100, 230, 245, 25));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        commitButton = new QPushButton(widget);
        commitButton->setObjectName("commitButton");

        horizontalLayout->addWidget(commitButton);

        cancelButton = new QPushButton(widget);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout->addWidget(cancelButton);

        brosewButton = new QPushButton(widget);
        brosewButton->setObjectName("brosewButton");

        horizontalLayout->addWidget(brosewButton);


        retranslateUi(widget_demo);

        QMetaObject::connectSlotsByName(widget_demo);
    } // setupUi

    void retranslateUi(QWidget *widget_demo)
    {
        widget_demo->setWindowTitle(QCoreApplication::translate("widget_demo", "widget_demo", nullptr));
        label->setText(QCoreApplication::translate("widget_demo", "\350\257\267\350\276\223\345\205\245\345\221\275\344\273\244", nullptr));
        label_2->setText(QCoreApplication::translate("widget_demo", "<html><head/><body><p>\346\211\223\345\274\200\357\274\232</p></body></html>", nullptr));
        commitButton->setText(QCoreApplication::translate("widget_demo", "\347\241\256\345\256\232", nullptr));
        cancelButton->setText(QCoreApplication::translate("widget_demo", "\345\217\226\346\266\210", nullptr));
        brosewButton->setText(QCoreApplication::translate("widget_demo", "\346\265\217\350\247\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class widget_demo: public Ui_widget_demo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_DEMO_H
