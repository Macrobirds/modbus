#ifndef MODBUS_MASTER_H
#define MODBUS_MASTER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <RecFrame.h>
#include <SerialPort.h>

QT_BEGIN_NAMESPACE
namespace Ui { class modbus_master; }
QT_END_NAMESPACE

class modbus_master : public QWidget
{
    Q_OBJECT

public:
    modbus_master(QWidget *parent = nullptr);
    ~modbus_master();

private:
    Ui::modbus_master *ui;
    UCHAR frame_rec_buffer[FR_SIZE_MAX];
    UCHAR frame_send_buffer[FR_SEND_LEN];
    UCHAR CoilState;
    USHORT HoldRegister;
    UCHAR getonebyte[1];
    volatile USHORT bufferpos;
    UCHAR * FrameData;
    TSerialPort com1;
    FrameREC  frame_rec;
    bool timeout;
private:
    void frame_send_queue(UCHAR func,USHORT address,USHORT number);
    bool frame_rec_queue();

public slots:
    void CloseCoil_response();
    void OpenCoil_response();
    void GetCoilState();
    void GetRegValue();
signals:
    void Geterror();
    void UpdateCoil();
    void UpdateRegister();


};
#endif // MODBUS_MASTER_H
