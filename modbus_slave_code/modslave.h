#ifndef MODSLAVE_H
#define MODSLAVE_H
#include <QObject>
#include <rtucrc.h>
#include <QtDebug>
#define FRAME_REC_LEN   8
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

#define FR_LEN_VALUE    2
#define FR_SEND_VALUE   3
#define FR_COIL_SET     4


typedef unsigned char UCHAR;
typedef unsigned long ULONG;
typedef unsigned short USHORT;

typedef enum : UCHAR{
    COIL_STATUS = 0x01,
    HOLD_REGISTER = 0x03,
    COIL_WRITE = 0x05
}FunctionCode;


class ModSlave : public QObject
{
    Q_OBJECT
public:
    ModSlave() {}
    virtual ~ModSlave() {}
private:
    FunctionCode FunCode;
    USHORT Address;
    USHORT Quantity;
    USHORT * Register;

public:
    ModSlave(FunctionCode func,USHORT address,USHORT quantity);
    USHORT frame_execute(UCHAR * framesend,UCHAR * framerec);
    void set_gesiter(USHORT registervalue);
    USHORT get_register();
    USHORT get_address();
    UCHAR get_func();

signals:
    void updatecoil();


};





#endif // MODSLAVE_H
