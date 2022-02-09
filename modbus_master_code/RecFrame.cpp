#include <RecFrame.h>

FrameREC::FrameREC(){
     slaveid=0;
     funcode=0;
     slaveadress=0;
     datalength=0;
     framedata=NULL;
}

FrameREC::~FrameREC(){

}

bool FrameREC::frameanalyse(UCHAR *frame, USHORT length){

    //calculate CRC16

    if(usMBCRC16(frame,length)==0&&length>=FR_SIZE_MIN&&length<FR_SIZE_MAX){
        slaveid=frame[FR_ID_OFF];
        funcode=frame[FR_PDU_OFF];
        switch(funcode){
        case 0x01:
            slaveadress=DEFAULT_ADD;
            datalength=frame[FR_REC_LEN];
            framedata=(UCHAR *)(frame+3);
            break;
        case 0x03:
            slaveadress=DEFAULT_ADD;
            datalength=frame[2];
            framedata=(UCHAR *)(frame+3);
            break;
        case 0x05:
            slaveadress=(USHORT)frame[FR_ADDR_OFF+1];
            slaveadress=slaveadress||((USHORT)(frame[FR_ADDR_OFF])<<8);
            datalength=2;
            framedata=(UCHAR *)(frame+FR_NUM_OFF);
            break;
        default: return false;
        }
        return true;


    };
    return false;

}

USHORT FrameREC::getslaveid(){
    return slaveid;
}

UCHAR FrameREC::getfuncode(){
    return funcode;
}

UCHAR FrameREC::getslaveadress(){
    return slaveadress;
}

UCHAR FrameREC::getdatalength(){
    return datalength;
}
UCHAR * FrameREC::getframedata(){
    return framedata;
}

//void FrameREC::updatestate(){
//    switch(funcode){
//    case 0x01:
//        emit updatecoil();
//        break;
//    case 0x05:
//        emit updatecoil();
//        break;
//    case 0x03:
//        emit updateregister();
//        break;


//    }
//}
