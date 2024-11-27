#include <QApplication>
#include "calculator.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    calculator cal;
    cal.show();

    return app.exec();
}
