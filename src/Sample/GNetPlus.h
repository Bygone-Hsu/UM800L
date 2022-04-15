#ifndef GNETPLUS_H_INCLUDED
#define GNETPLUS_H_INCLUDED

#ifdef	__cplusplus
extern "C" {
#endif

#include "errno.h"

#define GNETPLUS_CODE_ACK                       (0x06)
#define GNETPLUS_CODE_EVENT                     (0x11)
#define GNETPLUS_CODE_NAK                       (0x15)

#define GNETPLUS_NEW_REQUEST                    (1)
#define GNETPLUS_NEW_RESPONSE_ACK               (GNETPLUS_CODE_ACK)
#define GNETPLUS_NEW_RESPONSE_NAK               (GNETPLUS_CODE_NAK)
#define GNETPLUS_NEW_EVENT                      (GNETPLUS_CODE_EVENT)
#define GNETPLUS_LENGTH_DONE_FLAG               (1<<15)

#define CRC16_PRESET                            (0xFFFF)
#define CRC16_POLYNOM                           (0xA001)
unsigned short GNetPlus_CRC16_Byte(unsigned short uCRC16, unsigned char bData);
unsigned short GNetPlus_CRC16_Bytes(unsigned short uCRC16, const unsigned char* pBuffer, unsigned short uLength);

#define GNETPLUS_ADDRESS_ANY_DEVICE             (0)     // Broadcast
#define GNETPLUS_CRC16_LENGTH                   (2)
#define GNETPLUS_PACKAGE_HEADER_ID_LENGTH       (1)
#define GNETPLUS_PACKAGE_HEADER_LENGTH          (4)
#define GNETPLUS_PACKAGE_MIN_LENGTH             (GNETPLUS_PACKAGE_HEADER_LENGTH+GNETPLUS_CRC16_LENGTH)
#define GNETPLUS_PACKAGE_DATA_MAX_LENGTH        (255)
#define GNETPLUS_BUFFER_MAX_LENGTH              (GNETPLUS_PACKAGE_HEADER_LENGTH+GNETPLUS_PACKAGE_DATA_MAX_LENGTH+GNETPLUS_CRC16_LENGTH)
typedef struct
{
    unsigned short uLength;
    union
    {
        struct
        {
            unsigned char bHeaderId;
            unsigned char bAddress;
            unsigned char bCode;
            unsigned char bDataLength;
            unsigned char bDatas[GNETPLUS_PACKAGE_DATA_MAX_LENGTH];
        };
        unsigned char bBuffer[GNETPLUS_BUFFER_MAX_LENGTH];
    };
} GNetPlus_Package_t;

typedef void (*GNetPlus_PFSendByte)(unsigned char bData);
typedef unsigned char (*GNetPlus_PFReceiveByte)(unsigned char* pbData);
typedef void (*GNetPlus_PFOnPackage)(GNetPlus_Package_t* ptPackage);

#define GNETPLUS_STATUS_NONE                    (0)
#define GNETPLUS_STATUS_WAITING                 (0xFF)
#define GNETPLUS_STATUS_ACK                     (GNETPLUS_NEW_RESPONSE_ACK)
#define GNETPLUS_STATUS_NAK                     (GNETPLUS_NEW_RESPONSE_NAK)
#define GNETPLUS_STATUS_EVENT                   (GNETPLUS_NEW_EVENT)
#define GNETPLUS_STATUS_CRC_ERROR               (ERR_PROTO_CRC)
typedef struct
{
    GNetPlus_Package_t tPackage;
    unsigned char bStatus;
} GNetPlus_ReceiveBuffer_t;


typedef struct
{
    GNetPlus_PFSendByte pfSendByteHandler;
    GNetPlus_PFReceiveByte pfReceiveByteHandler;
    GNetPlus_PFOnPackage pfOnPackageHandler;
    GNetPlus_ReceiveBuffer_t* ptBuffer;
} GNetPlus_ControlBlock_t;

void GNetPlus_ClearPackage();
void GNetPlus_Initialize(const GNetPlus_ControlBlock_t* ptControlBlock);

void GNetPlus_ResyncPackageHeader();
unsigned char GNetPlus_ReceivePackage();
signed char GNetPlus_SendPackage(unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength);
void GNetPlus_MakePackage(GNetPlus_Package_t* ptPakcage, unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength);

#ifdef	__cplusplus
}
#endif

#endif // GNETPLUS_H_INCLUDED
