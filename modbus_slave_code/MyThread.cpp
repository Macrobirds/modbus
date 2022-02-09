#include <MyThread.h>

MyThread::MyThread(QObject *parent){
     SlaveStack * slavestack=new SlaveStack(0x01);
     connect(slavestack,SIGNAL(updateinterface(USHORT,USHORT)),this,SLOT(getupdatevalue(USHORT,USHORT)));
}



void MyThread::run(){
    qDebug()<<"is running";
    slavestack->StackPoll();

}

void MyThread::getupdatevalue(USHORT value_1,USHORT value_2){
    emit updatevalue(value_1,value_2);
}
