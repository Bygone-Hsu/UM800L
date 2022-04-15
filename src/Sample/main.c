#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "GNetPlus.h"

#define UM800L_REQUEST_CODE_GET_VERSION         (0x10)
#define UM800L_VERSION_TYPE_FIRMWARE            (0)
#define UM800L_VERSION_TYPE_HARDWARE            (1)
signed char UM800L_Send_GetVersion(unsigned char bVertionType)
{
    return GNetPlus_SendPackage(GNETPLUS_ADDRESS_ANY_DEVICE, UM800L_REQUEST_CODE_GET_VERSION, &bVertionType, 1);
}

unsigned long m_ulAppBeginTickCount;
unsigned long AppTickCount()
{   // GetTickCount: Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.
    return (GetTickCount()-m_ulAppBeginTickCount);
}

void ShowPackage(const char* pszTitle, GNetPlus_Package_t* ptPackage)
{
    unsigned short i;
    printf("%08lu %s[%0u]: ", AppTickCount(), pszTitle, ptPackage->uLength);
    for(i=0; i<ptPackage->uLength; i++)
    {

        printf("%02X", ptPackage->bBuffer[i]);;
    }
    printf("\r\n");
}


unsigned short m_iReceiveIdx;
GNetPlus_Package_t m_tRequest;

void SendByteToSaveToPackage(unsigned char bData)
{   // Call SendByte to output data to device e.g. UART.
    // The example is to save to the package
    if(m_tRequest.uLength<GNETPLUS_BUFFER_MAX_LENGTH)
    {

        printf("Send[%03u]: %02X\r\n", m_tRequest.uLength, bData);
        m_tRequest.bBuffer[m_tRequest.uLength++]=bData;
    }
}

unsigned char ReceiveByte(unsigned char* pbData)
{
    unsigned char bResult;
    bResult=0;
    if(m_iReceiveIdx<m_tRequest.uLength)
    {
        *pbData=m_tRequest.bBuffer[m_iReceiveIdx++];
        bResult=1;
    }
    return bResult;
}

const unsigned char FIRMWARE_VERSION[]="Name ROM-T0001 V1.00R1 (210723)";
const unsigned char HARDWARE_VERSION[]="Name V1.0";
void OnPackage(GNetPlus_Package_t* ptPackage)
{
    ShowPackage("OnPackage", ptPackage);
    if(UM800L_REQUEST_CODE_GET_VERSION==ptPackage->bCode)
    {
        signed char cResult;
        cResult=ERR_PARAM;
        if(1==ptPackage->bDataLength)
        {
            const unsigned char* pszVersion;
            unsigned short uLen;
            uLen=0;
            pszVersion=NULL;
            if(UM800L_VERSION_TYPE_FIRMWARE==ptPackage->bDatas[0])
            {
                cResult=ERR_NONE;
                uLen=(sizeof(FIRMWARE_VERSION)-1);
                pszVersion=FIRMWARE_VERSION;
            }
            else if(UM800L_VERSION_TYPE_HARDWARE==ptPackage->bDatas[0])
            {
                cResult=ERR_NONE;
                uLen=(sizeof(HARDWARE_VERSION)-1);
                pszVersion=HARDWARE_VERSION;
            }
            if(ERR_NONE==cResult)
            {
                m_iReceiveIdx=0;
                GNetPlus_MakePackage(&m_tRequest, GNETPLUS_ADDRESS_ANY_DEVICE, GNETPLUS_CODE_ACK, pszVersion, uLen);
            }
        }
        if(ERR_NONE!=cResult)
        {
            m_iReceiveIdx=0;
            GNetPlus_MakePackage(&m_tRequest, GNETPLUS_ADDRESS_ANY_DEVICE, GNETPLUS_CODE_NAK, ((unsigned char*)&cResult), 1);
        }
    }
}

GNetPlus_ReceiveBuffer_t m_tGNetPlusCB_Buffer;
const GNetPlus_ControlBlock_t m_tGNetPlusCB=
{
    .pfSendByteHandler=SendByteToSaveToPackage,
    .pfReceiveByteHandler=ReceiveByte,
    .pfOnPackageHandler=OnPackage,
    .ptBuffer=&m_tGNetPlusCB_Buffer
};

void ShowVersion(const char* pszTitle)
{
    printf("%s: %.*s\r\n", pszTitle, m_tGNetPlusCB_Buffer.tPackage.bDataLength, m_tGNetPlusCB_Buffer.tPackage.bDatas);
}

int main()
{
    unsigned long ulTicks;
    unsigned char bStatus;
    int i;
    GNetPlus_Initialize(&m_tGNetPlusCB);
    i=0;
    ulTicks=0;
    m_ulAppBeginTickCount=GetTickCount();
    while(1)
    {
        if(200<(AppTickCount()-ulTicks))
        {
            ulTicks=GetTickCount();
            if(m_iReceiveIdx>=m_tRequest.uLength)
            {
                signed char cResult;
                m_iReceiveIdx=0;
                m_tRequest.uLength=0;
                if(UM800L_VERSION_TYPE_FIRMWARE==i)
                {
                    m_iReceiveIdx=0;
                    m_tRequest.uLength=4;
                    m_tRequest.bHeaderId=0x01;  // Make Error Package
                    cResult=UM800L_Send_GetVersion(UM800L_VERSION_TYPE_FIRMWARE);
                    if(ERR_NONE==cResult)
                    {
                        ShowVersion("Firmware");
                    }
                    else
                    {
                        printf("Get Firmware Error: %02X\r\n", ((unsigned char)cResult));
                    }
                }
                else if(UM800L_VERSION_TYPE_HARDWARE==i)
                {
                    cResult=UM800L_Send_GetVersion(UM800L_VERSION_TYPE_HARDWARE);
                    if(ERR_NONE==cResult)
                    {
                        ShowVersion("Hardware");
                    }
                    else
                    {
                        printf("Get Hardware Error: %02X\r\n", ((unsigned char)cResult));
                    }
                }
                else if((UM800L_VERSION_TYPE_HARDWARE+1)==i)
                {
                    cResult=UM800L_Send_GetVersion(i);
                    if(ERR_NONE!=cResult)
                    {
                        printf("Get Unknown Version Error: %02X\r\n", ((unsigned char)cResult));
                    }
                }
                else if(5<i)
                {
                    break;
                }
                i++;
            }
        }
        bStatus=GNetPlus_ReceivePackage();
        if(GNETPLUS_STATUS_NONE!=bStatus)
        {
            if(GNETPLUS_STATUS_CRC_ERROR==bStatus)
            {
                printf("Protocol CRC16 Error\r\n");
            }
            else
            {
                printf("ReceivePackage: Code: %02X\r\n", bStatus);
            }
        }
    }
    return 0;
}
