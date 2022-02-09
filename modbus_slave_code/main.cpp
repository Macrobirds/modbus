#include "modbus_slave.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    modbus_slave w;
    w.show();

    return a.exec();
}
