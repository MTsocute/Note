#include <QApplication>
#include <QPushButton>
#include "window_demo.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Window_Demo win;
    win.show();

    return QApplication::exec();
}
