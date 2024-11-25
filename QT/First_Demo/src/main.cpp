#include <QApplication>
#include "widget_demo.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    widget_demo w;
    w.show();

    return QApplication::exec();
}
