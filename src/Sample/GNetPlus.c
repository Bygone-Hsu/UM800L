#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "GNetPlus.h"

#define ASCII_NUL                               (0x00)
#define ASCII_SOH                               (0x01)
#define GNET_PLUS_HEADER_ID                     (ASCII_SOH)
#define GNET_PLUS_HEADER_EMPTY_ID               (ASCII_NUL)

static const GNetPlus_ControlBlock_t* m_ptGNetPlusCB;

void GNetPlus_ClearPackage()
{
    memset(m_ptGNetPlusCB->ptBuffer, 0, sizeof(GNetPlus_ReceiveBuffer_t));
}

void GNetPlus_Initialize(const GNetPlus_ControlBlock_t* ptGNetPlusCB)
{
    m_ptGNetPlusCB=ptGNetPlusCB;
    GNetPlus_ClearPackage();
}

unsigned short GNetPlus_CRC16_Byte(unsigned short uCRC16, unsigned char bData)
{
    int i;
    uCRC16^=bData;
    for(i=0; i<8; i++)
    {
        if(uCRC16 & 1)
        {
            uCRC16=(uCRC16>>1)^CRC16_POLYNOM;
        }
        else
        {
            uCRC16=(uCRC16>>1);
        }
    }
    return uCRC16;
}

unsigned short GNetPlus_CRC16_Bytes(unsigned short uCRC16, const unsigned char* pBuffer, unsigned short uLength)
{
    while(uLength--)
    {
        uCRC16=GNetPlus_CRC16_Byte(uCRC16, (*pBuffer++));
    }
    return uCRC16;
}

#define ReceiveStatus                           m_ptGNetPlusCB->ptBuffer->bStatus
#define ReceivePackage                          m_ptGNetPlusCB->ptBuffer->tPackage
#define GNetPlus_ReceiveByte(pbData)            m_ptGNetPlusCB->pfReceiveByteHandler(pbData)
unsigned char GNetPlus_ReceivePackage()
{
    unsigned char bStatus;
    unsigned char bData;
    bStatus=GNETPLUS_STATUS_NONE;
    if((GNETPLUS_STATUS_NONE!=ReceiveStatus)
        || ((GNET_PLUS_HEADER_ID!=ReceivePackage.bHeaderId) && (0<ReceivePackage.uLength)))
    {
        GNetPlus_ClearPackage();
    }
    while(GNetPlus_ReceiveByte(&bData))
    {
        printf("pfReceiveByte: %02X\r\n", bData);
        if(GNET_PLUS_HEADER_ID==ReceivePackage.bHeaderId)
        {

            ReceivePackage.bBuffer[ReceivePackage.uLength++]=bData;
            if((GNETPLUS_PACKAGE_MIN_LENGTH+ReceivePackage.bDataLength)<=ReceivePackage.uLength)
            {
                unsigned char* pbCRC16;
                unsigned short uCRC16;
                pbCRC16=&ReceivePackage.bBuffer[ReceivePackage.uLength-2];
                uCRC16=GNetPlus_CRC16_Bytes(CRC16_PRESET,
                          &ReceivePackage.bAddress,
                          (ReceivePackage.uLength-(GNETPLUS_PACKAGE_HEADER_ID_LENGTH+GNETPLUS_CRC16_LENGTH)));
                bStatus=GNETPLUS_STATUS_CRC_ERROR;
                if(uCRC16==((((unsigned short)pbCRC16[0])<<8) | pbCRC16[1]))
                {
                    switch(ReceivePackage.bCode)
                    {
                    case GNETPLUS_CODE_ACK:
                    case GNETPLUS_CODE_NAK:
                        ReceiveStatus=ReceivePackage.bCode;
                    case GNETPLUS_CODE_EVENT:
                        bStatus=ReceivePackage.bCode;
                        break;
                    default:
                        bStatus=GNETPLUS_NEW_REQUEST;
                        break;
                    }
                    m_ptGNetPlusCB->pfOnPackageHandler(&ReceivePackage);
                }
                else
                {
                    GNetPlus_ResyncPackageHeader(&ReceivePackage);
                }
                break;
            }
        }
        else if(GNET_PLUS_HEADER_ID==bData)
        {
            ReceivePackage.bBuffer[ReceivePackage.uLength++]=bData;
        }
    }
    return bStatus;
}

static signed char GNetPlus_WaitForResponse(unsigned short uTimeout)
{
    unsigned long ulTicks;
    signed char cResult;
    cResult=ERR_TIMEOUT;
    ulTicks=GetTickCount(); // GetTickCount: Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.
    while((GetTickCount()-ulTicks)<uTimeout)
    {
        unsigned char bStatus;
        bStatus=GNetPlus_ReceivePackage();
        if(GNETPLUS_STATUS_ACK==bStatus)
        {
            cResult=ERR_NONE;
            break;
        }
        else if(GNETPLUS_STATUS_NAK==bStatus)
        {
            cResult=((signed char)ReceivePackage.bDatas[0]);
            break;
        }
        cResult=ERR_TIMEOUT;
    }
    return cResult;
}

#define GNET_PLUS_TIMEOUT_1000_MS               (1000)
#define GNET_PLUS_TIMEOUT_3000_MS               (1000)
#define GNET_PLUS_TIMEOUT_DEFAULT               GNET_PLUS_TIMEOUT_1000_MS
#define GNetPlus_SendByte(bData)                m_ptGNetPlusCB->pfSendByteHandler(bData)
signed char GNetPlus_SendPackage(unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength)
{
    unsigned short uCRC16;
    GNetPlus_SendByte(GNET_PLUS_HEADER_ID);
    GNetPlus_SendByte(bAddress);
    uCRC16=GNetPlus_CRC16_Byte(CRC16_PRESET, bAddress);
    GNetPlus_SendByte(bCode);
    uCRC16=GNetPlus_CRC16_Byte(uCRC16, bCode);
    GNetPlus_SendByte(bLength);
    uCRC16=GNetPlus_CRC16_Byte(uCRC16, bLength);
    while(bLength--)
    {
        unsigned char bData;
        bData=(*pbDatas++);
        GNetPlus_SendByte(bData);
        uCRC16=GNetPlus_CRC16_Byte(uCRC16, bData);
    }
    GNetPlus_SendByte((unsigned char)(uCRC16>>8));
    GNetPlus_SendByte((unsigned char)uCRC16);
    return GNetPlus_WaitForResponse(GNET_PLUS_TIMEOUT_DEFAULT);
}

void GNetPlus_MakePackage(GNetPlus_Package_t* ptPakcage, unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength)
{
    unsigned short uCRC16;
    memset(ptPakcage, 0, sizeof(GNetPlus_Package_t));
    ptPakcage->bHeaderId=GNET_PLUS_HEADER_ID;
    ptPakcage->bAddress=bAddress;
    ptPakcage->bCode=bCode;
    ptPakcage->bDataLength=bLength;
    if(0<bLength)
    {
        memcpy(ptPakcage->bDatas, pbDatas, bLength);
    }
    ptPakcage->uLength=(GNETPLUS_PACKAGE_MIN_LENGTH+bLength);
    uCRC16=GNetPlus_CRC16_Bytes(CRC16_PRESET,
                          &ptPakcage->bAddress,
                          (GNETPLUS_PACKAGE_HEADER_LENGTH-GNETPLUS_PACKAGE_HEADER_ID_LENGTH+bLength));
    ptPakcage->bDatas[bLength]=((unsigned char)(uCRC16>>8));
    ptPakcage->bDatas[bLength+1]=((unsigned char)uCRC16);
}

void GNetPlus_ResyncPackageHeader(GNetPlus_Package_t* ptBuffer)
{
    unsigned short uLen;
    unsigned short i;
    uLen=ptBuffer->uLength;
    ptBuffer->uLength=0;
    for(i=1; i<uLen; i++)
    {
        if(GNET_PLUS_HEADER_ID==ptBuffer->bBuffer[i])
        {
            uLen-=i;
            memcpy(ptBuffer->bBuffer, &ptBuffer->bBuffer[i], uLen);
            ptBuffer->uLength=uLen;
            break;
        }
    }
    printf("GNetPlus_ResyncPackageHeader\r\n");
}


