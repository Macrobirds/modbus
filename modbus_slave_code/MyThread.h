#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <slave_stack.h>

class MyThread : public QThread{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent=0);
    ~MyThread(){}
protected:
    void run() override;
signals:
    void updatevalue(USHORT,USHORT);
private:
    SlaveStack * slavestack;
public slots:
    void getupdatevalue(USHORT,USHORT);

};









#endif // MYTHREAD_H
