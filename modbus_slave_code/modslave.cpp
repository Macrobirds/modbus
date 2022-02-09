#include <modslave.h>

ModSlave::ModSlave(FunctionCode func,USHORT address,USHORT quantity){
    FunCode=func;
    Address=address;
    Quantity=quantity;
    qDebug()<<Quantity;
    USHORT * myregister=new USHORT[Quantity];
    for(int i=0;i<Quantity;i++){

       myregister[i]=0x0000;
    }
    Register=myregister;

}

USHORT ModSlave::frame_execute(UCHAR * framesend,UCHAR * framerec){
    USHORT length=0;
    USHORT requantity=0;//临时访问存贮寄存器变量
    USHORT CRCbyte=0;
    int i=0;
    framesend[FR_ID_OFF]=framerec[FR_ID_OFF];
    length++;
    qDebug()<<"send id:"<<framesend[FR_ID_OFF];
    for(int i=0;i<Quantity;i++){

        qDebug()<<Register[i];
    }
    switch (framerec[FR_PDU_OFF]) {

    case COIL_STATUS:
        if(framerec[FR_PDU_OFF]!=FunCode){
            framesend[FR_PDU_OFF]=0x81;
            length++;
            framesend[FR_ADDR_OFF]=0x01;
            length++;
            qDebug()<<"illegal function";
            //非法功能
        }else{
            framesend[FR_PDU_OFF]=FunCode;
            length++;
            requantity=(USHORT)framerec[FR_NUM_OFF];
            requantity=(requantity<<8)||((USHORT)framerec[FR_NUM_OFF+1]);
            if(requantity%8==0){
                framesend[FR_LEN_VALUE]=requantity/8;
            }else{
                framesend[FR_LEN_VALUE]=requantity/8+1;
            }
            length++;
            qDebug()<<"send lenvalue:"<<framesend[FR_LEN_VALUE];
            for(i=0;requantity>0&&requantity<=Quantity;requantity--){
                qDebug()<<"can enter the loop";
                framesend[FR_SEND_VALUE+i]=(UCHAR)(Register[i]);
                qDebug()<<"send value :"<<framesend[FR_SEND_VALUE+i]<<"i "<<i;
                i++;
                length++;
            }

        }
        qDebug()<<"ready to crc";
        CRCbyte=usMBCRC16(framesend,length);
        qDebug()<<"crc ok!";
        framesend[length++]=(UCHAR)CRCbyte;
        framesend[length++]=(UCHAR)(CRCbyte>>8);
        break;

    case HOLD_REGISTER:
        if(framerec[FR_PDU_OFF]!=FunCode){
            framesend[FR_PDU_OFF]=0x83;
            length++;
            framesend[FR_ADDR_OFF]=0x01;
            length++;
            //非法功能
        }else{
            framesend[FR_PDU_OFF]=FunCode;
            length++;
            framesend[FR_LEN_VALUE]=framerec[FR_NUM_OFF+1]*2;
            length++;
            requantity=framerec[FR_NUM_OFF+1]*2;

            for(i=0;requantity/2<=Quantity&&requantity>0;requantity-=2){
                framesend[FR_SEND_VALUE+i]=(UCHAR)(Register[i]>>8);
                framesend[FR_SEND_VALUE+i+1]=(UCHAR)Register[i];
                i+=2;
                length+=2;
            }


        }
        CRCbyte=usMBCRC16(framesend,length);
        framesend[length++]=(UCHAR)CRCbyte;
        framesend[length++]=(UCHAR)(CRCbyte>>8);
        break;

    case COIL_WRITE:
        if(FunCode!=COIL_STATUS){
            framesend[FR_PDU_OFF]=0x85;
            length++;
            framesend[FR_ADDR_OFF]=0x01;
            length++;
            //非法功能
        }else{
            Register[0]=framerec[FR_COIL_SET];
            framesend[FR_PDU_OFF]=COIL_WRITE;
            framesend[FR_ADDR_OFF]=(UCHAR)(Address>>8);
            framesend[FR_ADDR_OFF+1]=(UCHAR)Address;
            framesend[FR_NUM_OFF]=framerec[FR_NUM_OFF];
            framesend[FR_NUM_OFF+1]=framerec[FR_NUM_OFF+1];
            length=FR_SEND_LEN;
            emit updatecoil();
        }
        CRCbyte=usMBCRC16(framesend,FR_SEND_LEN-FR_SIZE_CRC);
        framesend[CRC_LOW]=(UCHAR)CRCbyte;
        framesend[CRC_HIGH]=(UCHAR)(CRCbyte>>8);
        break;

    }
    qDebug()<<"can return the length";
    return length;
}


void ModSlave::set_gesiter(USHORT registervalue){
    Register[0]=registervalue;
}
USHORT ModSlave::get_register(){
    return Register[0];
}
USHORT ModSlave::get_address(){
    return Address;
}

UCHAR ModSlave::get_func(){
    return FunCode;
}
