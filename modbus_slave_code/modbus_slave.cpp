#include "modbus_slave.h"
#include "ui_modbus_slave.h"

modbus_slave::modbus_slave(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::modbus_slave)
{
    ui->setupUi(this);
    ui->lineEdit_coil->setReadOnly(true);
    ui->lineEdit_holdregiter->setReadOnly(true);
    ui->lineEdit_coil->setText(tr("0"));
    ui->lineEdit_holdregiter->setText(tr("0"));
    SlaveStack * slavestack=new SlaveStack(this,0x01);
    connect(slavestack,SIGNAL(updateinterface(USHORT,USHORT)),this,SLOT(updatevalue(USHORT,USHORT)));
    connect(ui->ButtonStart,&QPushButton::clicked,this,[=](){
        slavestack->start();
    });

}

modbus_slave::~modbus_slave()
{
    delete ui;
}

void modbus_slave::updatevalue(USHORT coil,USHORT timeregister){
    ui->lineEdit_coil->setText(QString::number((bool)coil));
    ui->lineEdit_holdregiter->setText(QString::number(timeregister));
}
