#include "modbus_master.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    modbus_master w;
    w.show();
    return a.exec();
}
