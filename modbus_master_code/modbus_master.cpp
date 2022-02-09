#include "modbus_master.h"
#include "ui_modbus_master.h"
#include <QDebug>

modbus_master::modbus_master(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::modbus_master)
{
    ui->setupUi(this);
    timeout=false;
    com1.Open(1,9600,1000);
    CoilState=0x00;
    HoldRegister=0x0000;
    ui->lineEdit_reg->setReadOnly(true);
    ui->lineEdit_coil->setReadOnly(true);
    ui->lineEdit_reg->setText(tr("0"));
    ui->lineEdit_coil->setText(tr("0"));
    connect(ui->ButtonOpenCoil,SIGNAL(clicked()),this,SLOT(OpenCoil_response()));
    connect(ui->ButtonCloseCoil,SIGNAL(clicked()),this,SLOT(CloseCoil_response()));
    connect(ui->ButtonCoilState,SIGNAL(clicked()),this,SLOT(GetCoilState()));
    connect(ui->ButtonGetGegister,SIGNAL(clicked()),this,SLOT(GetRegValue()));
    connect(this,&modbus_master::UpdateCoil,this,[=](){
        ui->lineEdit_coil->setText(QString::number((bool)CoilState));
    });
    connect(this,&modbus_master::UpdateRegister,this,[=](){

        qDebug()<<HoldRegister;
        ui->lineEdit_reg->setText(QString::number(HoldRegister));
    });

//    TSerialPort * com1=new TSerialPort;

    //    MyTimer * timer=new MyTimer(5);
     //   FrameREC * frame_rec=new FrameREC;
}

modbus_master::~modbus_master()
{
    delete ui;
}

void modbus_master::CloseCoil_response(){
        memset(frame_rec_buffer,'\0',sizeof (frame_rec_buffer));
        memset(frame_send_buffer,'\0',sizeof(frame_send_buffer));
        frame_send_queue(0x05,0x0000,0x0000);

        com1.WriteBuffer(frame_send_buffer,FR_SEND_LEN);
        Sleep(50);
        if(frame_rec_queue()&&usMBCRC16(frame_rec_buffer,bufferpos)==0&&bufferpos>FR_SIZE_MIN&&bufferpos<FR_SIZE_MAX){
            qDebug()<<"crc pass";
            frame_rec.frameanalyse(frame_rec_buffer,bufferpos);
            FrameData=frame_rec.getframedata();
           if(FrameData[0]!=0x00){
                qDebug()<<"error2";
               emit Geterror();
           }else{
               qDebug("Can update");
               CoilState=FrameData[0];
               emit UpdateCoil();
           }
        }else{
            emit Geterror();//CRC校验出错，发生错误
        }
}



void modbus_master::OpenCoil_response(){
    memset(frame_rec_buffer,'\0',sizeof (frame_rec_buffer));
    memset(frame_send_buffer,'\0',sizeof(frame_send_buffer));
    frame_send_queue(0x05,0x0000,0xff00);
    com1.WriteBuffer(frame_send_buffer,FR_SEND_LEN);
    Sleep(50);
    if(frame_rec_queue()&&usMBCRC16(frame_rec_buffer,bufferpos)==0&&bufferpos>FR_SIZE_MIN&&bufferpos<FR_SIZE_MAX){
        frame_rec.frameanalyse(frame_rec_buffer,bufferpos);
        FrameData=frame_rec.getframedata();
       if(FrameData[0]!=0xff){
            qDebug()<<"error2";
           emit Geterror();
       }else{
           qDebug("Can update");
           CoilState=FrameData[0];
           emit UpdateCoil();
       }
    }else{
        emit Geterror();//CRC校验出错，发生错误
    }
}



void modbus_master::GetCoilState(){
    memset(frame_rec_buffer,'\0',sizeof (frame_rec_buffer));
    memset(frame_send_buffer,'\0',sizeof(frame_send_buffer));
    frame_send_queue(0x01,0x0000,0x0001);
    com1.WriteBuffer(frame_send_buffer,FR_SEND_LEN);
    Sleep(50);    
    if(frame_rec_queue()&&usMBCRC16(frame_rec_buffer,bufferpos)==0&&bufferpos>FR_SIZE_MIN&&bufferpos<FR_SIZE_MAX){
        frame_rec.frameanalyse(frame_rec_buffer,bufferpos);
        FrameData=frame_rec.getframedata();
        CoilState=FrameData[0];
        emit UpdateCoil();
    }else{
        emit Geterror();//CRC校验出错，发生错误
    }
}


void modbus_master::GetRegValue(){
    memset(frame_rec_buffer,'\0',sizeof (frame_rec_buffer));
    memset(frame_send_buffer,'\0',sizeof(frame_send_buffer));
    frame_send_queue(0x03,0x0001,0x0001);
    com1.WriteBuffer(frame_send_buffer,FR_SEND_LEN);
    Sleep(50);
    if(frame_rec_queue()&&usMBCRC16(frame_rec_buffer,bufferpos)==0&&bufferpos>FR_SIZE_MIN&&bufferpos<FR_SIZE_MAX){
        frame_rec.frameanalyse(frame_rec_buffer,bufferpos);
        FrameData=frame_rec.getframedata();
//        HoldRegister=FrameData[0];
//        qDebug()<<"Framedate 0:"<<FrameData[0];
//        HoldRegister=((USHORT)FrameData[1])||(HoldRegister<<8);
//        qDebug()<<"Framedate 1:"<<FrameData[1];
        HoldRegister=FrameData[1];
        emit UpdateRegister();
    }else{
        emit Geterror();//CRC校验出错，发生错误
    }
}



void modbus_master::frame_send_queue(UCHAR func,USHORT address,USHORT number){
    USHORT CRCbyte=0;
    frame_send_buffer[FR_ID_OFF]=0x01;
    frame_send_buffer[FR_PDU_OFF]=func;
    frame_send_buffer[FR_ADDR_OFF]=(UCHAR)(address>>8);
    frame_send_buffer[FR_ADDR_OFF+1]=(UCHAR)address;
    frame_send_buffer[FR_NUM_OFF]=(UCHAR)(number>>8);
    frame_send_buffer[FR_NUM_OFF+1]=(UCHAR)number;
    CRCbyte=usMBCRC16(frame_send_buffer,FR_SEND_LEN-FR_SIZE_CRC);
    frame_send_buffer[CRC_LOW]=(UCHAR)CRCbyte;
    frame_send_buffer[CRC_HIGH]=(UCHAR)(CRCbyte>>8);


}

bool modbus_master::frame_rec_queue(){
    int counter=0;
    bufferpos=0;
    timeout=false;
    do{
        timeout=com1.ReadBuffer(getonebyte,sizeof(UCHAR),1000);
        qDebug()<<timeout;
        qDebug()<<"counter"<<counter;
        qDebug()<<((!timeout)&&counter<10);
        Sleep(1000);
        counter++;
    }while((!timeout)&&counter<10);
    qDebug()<<"counter"<<counter;

    if(counter>=10){ //接收时间超时，发生错误
        qDebug()<<"error1";
        emit Geterror();
        return false;
    }else{
        //持续接收字符直到超时时间大于3.5个字符间隔
        do{
             qDebug("%x",getonebyte[0]);
           frame_rec_buffer[bufferpos++]=getonebyte[0];
           timeout=com1.ReadBuffer(getonebyte,sizeof(UCHAR),5);
            qDebug("Getresponse sucesss");


        }while(timeout);
    }
    return true;

}
