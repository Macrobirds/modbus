#ifndef RECFRAME_H
#define RECFRAME_H


#include<QObject>
#include<rtucrc.h>

#define FR_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define FR_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define FR_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define FR_ID_OFF       0       /*!< Offset of slave address in Ser-PDU. */
#define FR_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */
#define FR_ADDR_OFF     2
#define FR_NUM_OFF      4
#define FR_SEND_LEN     8
#define CRC_LOW         6
#define CRC_HIGH        7
#define DEFAULT_ADD     0x00

#define FR_REC_LEN      2



//typedef enum{
//    READ_COIL=0x01,
//    WRITE_COIL=0x05,
//    READ_REG=0x03

//} FUNCODE;

class FrameREC:public QObject{
    Q_OBJECT

    private:
        UCHAR slaveid;
        UCHAR funcode;
        USHORT slaveadress;
        UCHAR datalength;
        UCHAR * framedata;


    public:
        FrameREC();
        ~FrameREC();
        bool frameanalyse(UCHAR * frame,USHORT  length);
        USHORT getslaveid();
        UCHAR getfuncode();
        UCHAR getslaveadress();
        UCHAR getdatalength();
        UCHAR * getframedata();
 //       void updatestate();
//    signals:
//        void updatecoil();
//        void updateregister();



};
#endif // RECFRAME_H

