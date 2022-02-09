#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

#include <QWidget>
#include <slave_stack.h>

QT_BEGIN_NAMESPACE
namespace Ui { class modbus_slave; }
QT_END_NAMESPACE

class modbus_slave : public QWidget
{
    Q_OBJECT

public:
    modbus_slave(QWidget *parent = nullptr);
    ~modbus_slave();

private:
    Ui::modbus_slave *ui;
    SlaveStack * slavestack;
public slots:
    void updatevalue(USHORT,USHORT);

};
#endif // MODBUS_SLAVE_H
