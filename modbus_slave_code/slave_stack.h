#ifndef SLAVE_STACK_H
#define SLAVE_STACK_H
#include<SerialPort.h>
#include <modslave.h>
#include <QThread>



typedef enum{
    STACK_IDLE,
    STACK_REC,
    STACK_EXECUTE,
    STACK_SEND
} StackState;

typedef  enum{
    PORT_READ,
    PORT_WRITE,
    PORT_IDLE
} PortState;




class SlaveStack : public QThread
{
    Q_OBJECT
public:
    SlaveStack(QObject* parent=nullptr,UCHAR slaveid=0x01) ;
    ~SlaveStack() ;
private:
    volatile StackState stackstate;
    volatile PortState portstate;
    volatile UCHAR bufferpos;
    volatile bool timeout;
    bool coilchange;
    UCHAR SlaveID;
    UCHAR Frame_Rec_Buffer[FRAME_REC_LEN];
    UCHAR Frame_Send_Buffer[FR_SIZE_MAX];
    UCHAR GetOneByte[1];

    TSerialPort com2;
    ModSlave * modslave[2];



    void frame_received();
    void frame_send(USHORT length);
    void updatestate();
public slots:
    void updatevalue();
signals:
    void updateinterface(USHORT,USHORT);
public:
      void StackPoll();
protected:
      void run() override;

};

#endif // SLAVE_STACK_H
