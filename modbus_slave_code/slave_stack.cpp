#include <slave_stack.h>

SlaveStack::SlaveStack(QObject *parent,UCHAR slaveid){
    SlaveID=slaveid;
    //创建一个线圈寄存器
    ModSlave * SlaveCoil=new ModSlave(COIL_STATUS,0x0000,0x0001);
    //创建一个保持寄存器
    ModSlave * SlaveRegister=new ModSlave(HOLD_REGISTER,0x0001,0x0001);
    modslave[0]=SlaveCoil;
    modslave[1]=SlaveRegister;
    stackstate=STACK_IDLE;
    portstate=PORT_READ;
    coilchange=false;
    memset(Frame_Rec_Buffer,'\0',sizeof (Frame_Rec_Buffer));
    memset(Frame_Send_Buffer,'\0',sizeof(Frame_Send_Buffer));
    com2.Open(2,9600,1000);
    connect(modslave[0],SIGNAL(updatecoil()),this,SLOT(updatevalue()));

}

SlaveStack::~SlaveStack(){

}

void SlaveStack::frame_received(){
    bufferpos=0;
    timeout=false;
    timeout=com2.ReadBuffer(GetOneByte,sizeof(UCHAR),1000);

    if(timeout){
        do{
           qDebug("%x",GetOneByte[0]);
           Frame_Rec_Buffer[bufferpos++]=GetOneByte[0];
           timeout=com2.ReadBuffer(GetOneByte,sizeof(UCHAR),5);
           qDebug("Getresponse sucesss");


        }while(timeout);
        portstate=PORT_IDLE;
        stackstate=STACK_REC;
    }

}

void SlaveStack::frame_send(USHORT length){
    qDebug()<<"length :"<<length;
    int writelength=0;
    writelength=com2.WriteBuffer(Frame_Send_Buffer,length);
    if(writelength==length){
        portstate=PORT_READ;
        stackstate=STACK_IDLE;
    }
}

void SlaveStack::updatestate(){
    qDebug()<<"enter update function";
    qDebug()<<portstate;
    switch (portstate) {
    case PORT_IDLE:
        break;
    case PORT_READ:
        qDebug()<<"try to receive";
        frame_received();
        break;
    case PORT_WRITE:
        frame_send(bufferpos);
        break;

    }
}

void SlaveStack::StackPoll(){
    qDebug()<<"enter poll";

    USHORT addresstemp=0;
    qDebug()<<"can define var";
    while(1){
        qDebug()<<"update state";
        updatestate();

        switch (stackstate) {
        case STACK_IDLE:
            break;
        case STACK_REC:

            if (Frame_Rec_Buffer[FR_ID_OFF]!=SlaveID&&usMBCRC16(Frame_Rec_Buffer,bufferpos)!=0){
                memset(Frame_Rec_Buffer,'\0',sizeof (Frame_Rec_Buffer));
            }else{
                qDebug()<<"can execute";
                stackstate=STACK_EXECUTE;
            }
            break;
        case STACK_EXECUTE:
            addresstemp=0;
            addresstemp=(USHORT)Frame_Rec_Buffer[FR_ADDR_OFF];
            addresstemp=(addresstemp<<8)||(USHORT)Frame_Rec_Buffer[FR_ADDR_OFF+1];
            qDebug()<<"addresstemp :"<<addresstemp;
            for(int i=0;i<2;i++){

                if(addresstemp==modslave[i]->get_address()){
                    qDebug()<<"i :"<<i<<"address :"<<modslave[i]->get_address();
                    bufferpos=modslave[i]->frame_execute(Frame_Send_Buffer,Frame_Rec_Buffer);
                    qDebug()<<"execute success";
                    stackstate=STACK_SEND;
                    portstate=PORT_WRITE;
                    break;
                }
            }
            break;
        case STACK_SEND:
            break;



        }
    }

}

void SlaveStack::updatevalue(){
    if(!coilchange&&(modslave[0]->get_register())){
        modslave[1]->set_gesiter(modslave[1]->get_register()+1);
    }
    coilchange=modslave[0]->get_register();
    emit updateinterface(modslave[0]->get_register(),modslave[1]->get_register());
}

void SlaveStack::run(){
    this->StackPoll();
}
