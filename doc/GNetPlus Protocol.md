# GNetPlus Protocol

## Symbols

Symbols to this protocol set are as follows

### Data Types

This protocol default digital data order uses **Big Endian**

| Type | Bytes | Value Range     | Description              |
|:----:|:-----:|:--------------- | ------------------------ |
| s8   | 1     | -128 \~ 127     | 8 bit signed integer     |
| s16  | 2     | -32768 \~ 32767 | 16 bit signed integer    |
| u8   | 1     | 0 \~ 255        | 8 bit unsigned integer   |
| u16  | 2     | 0 \~ 65535      | 16 bit  unsigned integer |

## Basic Package

### Package Structure

| Offset | Bytes | Type | Name      | Description                                                                                                                                    |
|:------:|:-----:|:----:| --------- | ---------------------------------------------------------------------------------------------------------------------------------------------- |
| 0      | 1     | u8   | Header Id | Start of Header<br />01h: SOH GNetPlus Start of Header                                                                                         |
| 1      | 1     | u8   | Address   | Request / Response Device Address (For RS485)<br />00h: Broadcast (Any Device)                                                                 |
| 2      | 1     | u8   | Code      | Request Code (Host to Device)<br />Others: Request code<br /><br />Response Code (Device to Host)<br />06h: ACK <br />15h: NAK<br />11h: Event |
| 3      | 1     | u8   | Length    | Data Length (N Bytes)                                                                                                                          |
| 4      | N     | u8   | Data      | Request parameters / Response Data                                                                                                             |
| 4+N    | 2     | u16  | CRC16     | CRC16                                                                                                                                          |

### NAK Package Structure

| Offset | Bytes | Type | Name       | Description                                                         |
|:------:|:-----:|:----:| ---------- | ------------------------------------------------------------------- |
| 0      | 1     | u8   | Header Id  | Start of Header <br />01h: SOH GNetPlus Start of Header             |
| 1      | 1     | u8   | Address    | Response Device Address (For RS485)<br />00: Broadcast (Any Device) |
| 2      | 1     | u8   | Code       | Response Code (Device to Host)<br />15h: NAK                        |
| 3      | 1     | u8   | Length     | 01h: 1 Byte error code                                              |
| 4      | N     | u8   | Error Code | Response error code                                                 |
| 4+N    | 2     | u16  | CRC16      | CRC16                                                               |

### Response Example

General response packet

#### Response an ACK package

<br />`[Receive 6 Bytes] General Response an ACK package (ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `00` | `50` | `42` |      |      |      |      |      |      |      |      |      |      | `....PB`                                                |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 06`                                                   | `06h: Code: ACK`                                               |
| `03`     | `1`     | ` 00`                                                   | `Data Length`                                                  |
| `04`     | `2`     | ` 50 42`                                                | `CRC16`                                                        |

#### Response a NAK package

<br />`[Receive 7 Bytes] General Response an NAK package (NAK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `15` | `01` | `F8` | `E6` | `21` |      |      |      |      |      |      |      |      |      | `......!`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 15`                                                   | `15h: Code: NAK`                                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` F8`                                                   | `F8h: Error Code: Parameter error`                             |
| `05`     | `2`     | ` E6 21`                                                | `CRC16`                                                        |

## CRC16 Calculation

### C Source (GNetPlus.c)

```C
#define CRC16_PRESET                            (0xFFFF)
#define CRC16_POLYNOM                           (0xA001)

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

unsigned short GNetPlus_CRC16_Bytes(unsigned short uCRC16, const unsigned char* pBuffer, int iLength)
{
    while(iLength--)
    {
        uCRC16=GNetPlus_CRC16_Byte(uCRC16, (*pBuffer++));
    }
    return uCRC16;
}
```

## GNetPlus Implement

### C Header (errno.h)

```C
#define ERR_NONE            ((signed char)0)    // 0x00: no error occured
#define ERR_UNKNOWN         ((signed char)-1)   // 0xFF: unkonwn error occured
#define ERR_NOMEM           ((signed char)-2)   // 0xFE: not enough memory to perform the requested operation
#define ERR_BUSY            ((signed char)-3)   // 0xFD: device or resource busy
#define ERR_IO              ((signed char)-4)   // 0xFC: generic IO error
#define ERR_TIMEOUT         ((signed char)-5)   // 0xFB: error due to timeout
#define ERR_REQUEST         ((signed char)-6)   // 0xFA: invalid request or requested function can't be executed at the moment
#define ERR_NOMSG           ((signed char)-7)   // 0xF9: No message of desired type
#define ERR_PARAM           ((signed char)-8)   // 0xF8: Parameter error
#define ERR_PROTO           ((signed char)-9)   // 0xF7: Protocol error
#define ERR_PROTO_CRC       ((signed char)-10)  // 0xF6: Protocol check error
#define ERR_UNLINKED        ((signed char)-11)  // 0xF5: Unlinked / Invalid link
#define ERR_HARDWARE        ((signed char)-12)  // 0xF4: Hardware error (Internal error code, need to reset hardware)
```

### C Header (GNetPlus.h)

```C
#define GNETPLUS_TIMEOUT_200_MS                 (200)
#define GNETPLUS_TIMEOUT_1000_MS                (1000)
#define GNETPLUS_TIMEOUT_3000_MS                (1000)
#define GNETPLUS_RECEIVE_TIMEOUT                (GNETPLUS_TIMEOUT_200_MS)
#define GNETPLUS_TIMEOUT_DEFAULT                (GNETPLUS_TIMEOUT_1000_MS)

#define GNETPLUS_HEADER_ID                      (0x01)
#define GNETPLUS_CODE_ACK                       (0x06)
#define GNETPLUS_CODE_EVENT                     (0x11)
#define GNETPLUS_CODE_NAK                       (0x15)

#define GNETPLUS_STATUS_NONE                    (0)
#define GNETPLUS_STATUS_WAITING                 (1)
#define GNETPLUS_STATUS_REQUEST                 (2)
#define GNETPLUS_STATUS_RECEIVE_TIMEOUT         (3)
#define GNETPLUS_STATUS_CHECK_ERROR             (ERR_PROTO_CRC)
#define GNETPLUS_STATUS_RESPONSE_ACK            (GNETPLUS_CODE_ACK)
#define GNETPLUS_STATUS_RESPONSE_NAK            (GNETPLUS_CODE_NAK)
#define GNETPLUS_STATUS_RESPONSE_EVENT          (GNETPLUS_CODE_EVENT)

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

typedef struct
{
    GNetPlus_Package_t tPackage;
    unsigned long ulTicks;
    unsigned char bStatus;
} GNetPlus_ReceiveBuffer_t;

typedef void (*GNetPlus_PFSendByteHandler)(unsigned char bData);
typedef unsigned char (*GNetPlus_PFReceiveByteHandler)(unsigned char* pbData);
typedef void (*GNetPlus_PFPackageHandler)(GNetPlus_Package_t* ptPackage);

typedef struct
{
    GNetPlus_PFSendByteHandler pfSendByteHandler;
    GNetPlus_PFReceiveByteHandler pfReceiveByteHandler;
    GNetPlus_PFPackageHandler pfOnPackageHandler;
    GNetPlus_ReceiveBuffer_t* ptBuffer;
} GNetPlus_ControlBlock_t;

void GNetPlus_Initialize(const GNetPlus_ControlBlock_t* ptControlBlock);

unsigned char GNetPlus_ReceiveHandler();
signed char GNetPlus_SendPackage(unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength);
void GNetPlus_MakePackage(GNetPlus_Package_t* ptPakcage, unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength);
```

### C Source (GNetPlus.c)

```C
#include <string.h>
#include <windows.h>

#include "GNetPlus.h"

#define GNetPlus_GetSysTicks()                  (GetTickCount())
// GetTickCount: Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.

static const GNetPlus_ControlBlock_t* m_ptGNetPlusCB;

static void GNetPlus_ClearPackage()
{
    memset(m_ptGNetPlusCB->ptBuffer, 0, sizeof(GNetPlus_ReceiveBuffer_t));
}

void GNetPlus_Initialize(const GNetPlus_ControlBlock_t* ptGNetPlusCB)
{
    m_ptGNetPlusCB=ptGNetPlusCB;
    GNetPlus_ClearPackage();
}

#define CRC16_PRESET                            (0xFFFF)
#define CRC16_POLYNOM                           (0xA001)
static unsigned short GNetPlus_CRC16_Byte(unsigned short uCRC16, unsigned char bData)
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

static unsigned short GNetPlus_CRC16_Bytes(unsigned short uCRC16, const unsigned char* pBuffer, unsigned short uLength)
{
    while(uLength--)
    {
        uCRC16=GNetPlus_CRC16_Byte(uCRC16, (*pBuffer++));
    }
    return uCRC16;
}

#define GNETPLUS_HEADER_ID_EMPTY                (0x00)
static void GNetPlus_ResyncPackageHeader(GNetPlus_Package_t* ptBuffer)
{
    unsigned short i;
    for(i=1; i<ptBuffer->uLength; i++)
    {
        if(GNETPLUS_HEADER_ID==ptBuffer->bBuffer[i])
        {
            break;
        }
    }
    if(i<ptBuffer->uLength)
    {
        ptBuffer->uLength-=i;
        memcpy(ptBuffer->bBuffer, &ptBuffer->bBuffer[i], ptBuffer->uLength);
    }
    else
    {
        GNetPlus_ClearPackage();
    }
}

#define ReceiveStatus                           m_ptGNetPlusCB->ptBuffer->bStatus
#define ReceivePackage                          m_ptGNetPlusCB->ptBuffer->tPackage
#define ReceiveBufferTicks                      m_ptGNetPlusCB->ptBuffer->ulTicks
#define GNetPlus_ReceiveByte(pbData)            m_ptGNetPlusCB->pfReceiveByteHandler(pbData)
unsigned char GNetPlus_ReceiveHandler()
{
    unsigned char bStatus;
    bStatus=GNETPLUS_STATUS_NONE;
    if(GNETPLUS_STATUS_NONE!=ReceiveStatus)
    {
        GNetPlus_ClearPackage();
    }
    else if(GNETPLUS_HEADER_ID==ReceivePackage.bHeaderId)
    {
        if(GNETPLUS_RECEIVE_TIMEOUT<(GNetPlus_GetSysTicks()-ReceiveBufferTicks))
        {
            bStatus=GNETPLUS_STATUS_RECEIVE_TIMEOUT;
            GNetPlus_ResyncPackageHeader(&ReceivePackage);
        }
    }
    if(GNETPLUS_STATUS_NONE==bStatus)
    {
        unsigned short uCount;
        unsigned char bData;
        uCount=0;
        while(GNetPlus_ReceiveByte(&bData))
        {
            uCount++;
            if((GNETPLUS_HEADER_ID==ReceivePackage.bHeaderId) || (GNETPLUS_HEADER_ID==bData))
            {

                ReceivePackage.bBuffer[ReceivePackage.uLength++]=bData;
                if((GNETPLUS_PACKAGE_MIN_LENGTH+ReceivePackage.bDataLength)<=ReceivePackage.uLength)
                {
                    GNetPlus_PFPackageHandler pfPackageHandler;
                    unsigned char* pbCRC16;
                    unsigned short uCRC16;
                    pbCRC16=&ReceivePackage.bBuffer[ReceivePackage.uLength-2];
                    uCRC16=GNetPlus_CRC16_Bytes(CRC16_PRESET,
                              &ReceivePackage.bAddress,
                              (ReceivePackage.uLength-(GNETPLUS_PACKAGE_HEADER_ID_LENGTH+GNETPLUS_CRC16_LENGTH)));
                    bStatus=GNETPLUS_STATUS_CHECK_ERROR;
                    pfPackageHandler=GNetPlus_ResyncPackageHeader;
                    if(uCRC16==((((unsigned short)pbCRC16[0])<<8) | pbCRC16[1]))
                    {
                        bStatus=ReceivePackage.bCode;
                        if((GNETPLUS_CODE_ACK!=bStatus)
                           && (GNETPLUS_CODE_NAK!=bStatus)
                           && (GNETPLUS_CODE_EVENT!=bStatus))
                        {
                            bStatus=GNETPLUS_STATUS_REQUEST;
                        }
                        ReceiveStatus=bStatus;
                        pfPackageHandler=m_ptGNetPlusCB->pfOnPackageHandler;
                    }
                    pfPackageHandler(&ReceivePackage);
                    break;
                }
            }
        }
        if(0<uCount)
        {
            m_ptGNetPlusCB->ptBuffer->ulTicks=GNetPlus_GetSysTicks();
        }
    }
    return bStatus;
}

static signed char GNetPlus_WaitForResponse(unsigned short uTimeout)
{
    unsigned long ulTicks;
    signed char cResult;
    cResult=ERR_NONE;
    ulTicks=GNetPlus_GetSysTicks();
    while(ERR_NONE==cResult)
    {
        unsigned char bStatus;
        if((GNetPlus_GetSysTicks()-ulTicks)>=uTimeout)
        {
            cResult=ERR_TIMEOUT;
        }
        bStatus=GNetPlus_ReceiveHandler();
        if(GNETPLUS_STATUS_RESPONSE_ACK==bStatus)
        {
            cResult=ERR_NONE;
            break;
        }
        else if(GNETPLUS_STATUS_RESPONSE_NAK==bStatus)
        {
            cResult=((signed char)ReceivePackage.bDatas[0]);
            break;
        }
    }
    return cResult;
}

#define GNetPlus_SendByte(bData)                m_ptGNetPlusCB->pfSendByteHandler(bData)
signed char GNetPlus_SendPackage(unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength)
{
    unsigned short uCRC16;
    GNetPlus_SendByte(GNETPLUS_HEADER_ID);
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
    return GNetPlus_WaitForResponse(GNETPLUS_TIMEOUT_DEFAULT);
}

void GNetPlus_MakePackage(GNetPlus_Package_t* ptPakcage, unsigned char bAddress, unsigned char bCode, const unsigned char* pbDatas, unsigned char bLength)
{
    unsigned short uCRC16;
    memset(ptPakcage, 0, sizeof(GNetPlus_Package_t));
    ptPakcage->bHeaderId=GNETPLUS_HEADER_ID;
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
```

### Test Code

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "GNetPlus.h"

#define GNetPlus_GetSysTicks()                  (GetTickCount())
// GetTickCount: Retrieves the number of milliseconds that have elapsed since the system was started, up to 49.7 days.

#define UM800L_REQUEST_CODE_GET_VERSION         (0x10)
#define UM800L_VERSION_TYPE_FIRMWARE            (0)
#define UM800L_VERSION_TYPE_HARDWARE            (1)
signed char UM800L_Send_GetVersion(unsigned char bVertionType)
{
    return GNetPlus_SendPackage(GNETPLUS_ADDRESS_ANY_DEVICE, UM800L_REQUEST_CODE_GET_VERSION, &bVertionType, 1);
}

void ShowPackage(const char* pszTitle, GNetPlus_Package_t* ptPackage)
{
    unsigned short i;
    printf("%08lu %s[%0u]: ", GNetPlus_GetSysTicks(), pszTitle, ptPackage->uLength);
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
        //printf("SendByte[%03u]: %02X\r\n", m_tRequest.uLength, bData);
        m_tRequest.bBuffer[m_tRequest.uLength++]=bData;
    }
}

unsigned char ReceiveByte(unsigned char* pbData)
{
    unsigned char bResult;
    bResult=0;
    if(m_iReceiveIdx<m_tRequest.uLength)
    {
        //printf("ReceiveByte[%03u]: %02X\r\n", m_iReceiveIdx, m_tRequest.bBuffer[m_iReceiveIdx]);
        *pbData=m_tRequest.bBuffer[m_iReceiveIdx++];
        bResult=1;
    }
    return bResult;
}

const unsigned char FIRMWARE_VERSION[]="Name ROM-T0001 V1.00R1 (210723)";
const unsigned char HARDWARE_VERSION[]="Name Hardware V1.0";
void OnRequest(GNetPlus_Package_t* ptPackage)
{
    signed char cResult;
    cResult=ERR_PARAM;
    // Self Test: Host to Device
    ShowPackage("OnPackage(Request)", ptPackage);
    if(UM800L_REQUEST_CODE_GET_VERSION==ptPackage->bCode)
    {
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
    }
    if(ERR_NONE!=cResult)
    {
        m_iReceiveIdx=0;
        GNetPlus_MakePackage(&m_tRequest, GNETPLUS_ADDRESS_ANY_DEVICE, GNETPLUS_CODE_NAK, ((unsigned char*)&cResult), 1);
    }
}

void OnResponse(GNetPlus_Package_t* ptPackage)
{
    // Self Test: Device to Host
    ShowPackage("OnPackage(Response)", ptPackage);
}

void OnPackage(GNetPlus_Package_t* ptPackage)
{
    switch(ptPackage->bCode)
    {
    case GNETPLUS_CODE_ACK:
    case GNETPLUS_CODE_EVENT:
    case GNETPLUS_CODE_NAK:
        // Self Test: Host to Device
        OnResponse(ptPackage);
        break;
    default:
        // Self Test: Device to Host
        OnRequest(ptPackage);
        break;
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
    printf("%s Version: %.*s\r\n", pszTitle, m_tGNetPlusCB_Buffer.tPackage.bDataLength, m_tGNetPlusCB_Buffer.tPackage.bDatas);
}

int main()
{
    unsigned long ulTicks;
    unsigned char bStatus;
    int i;
    GNetPlus_Initialize(&m_tGNetPlusCB);

    m_iReceiveIdx=0;
    m_tRequest.uLength=6;
    m_tRequest.bHeaderId=0x01;  // Make CRC16 error package
    printf("Receive CRC16 Error Test:\r\n");

    i=0;
    ulTicks=0;
    while(1)
    {
        if(GNETPLUS_RECEIVE_TIMEOUT<(GNetPlus_GetSysTicks()-ulTicks))
        {
            ulTicks=GNetPlus_GetSysTicks();
            if(m_iReceiveIdx>=m_tRequest.uLength)
            {
                signed char cResult;
                m_iReceiveIdx=0;
                m_tRequest.uLength=0;
                if(UM800L_VERSION_TYPE_FIRMWARE==i)
                {
                    m_iReceiveIdx=0;
                    m_tRequest.uLength=4;
                    m_tRequest.bHeaderId=0x01;  // Make incomplete error package
                    printf("\r\nGet Firmware Version Test:\r\n");
                    cResult=UM800L_Send_GetVersion(UM800L_VERSION_TYPE_FIRMWARE);
                    if(ERR_NONE==cResult)
                    {
                        ShowVersion("Firmware");
                    }
                    else
                    {
                        printf("\r\nGet Firmware Error: %02X\r\n", ((unsigned char)cResult));
                    }
                }
                else if(UM800L_VERSION_TYPE_HARDWARE==i)
                {
                    printf("\r\nGet Hardware Version Test:\r\n");
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
                    printf("\r\nGet Unknown Version Test:\r\n");
                    cResult=UM800L_Send_GetVersion(i);
                    if(ERR_NONE!=cResult)
                    {
                        printf("Get Unknown Version Error: %02X\r\n", ((unsigned char)cResult));
                    }
                }
                else if((UM800L_VERSION_TYPE_HARDWARE+2)==i)
                {
                    printf("\r\nGet Receive Package Timeout Test:\r\n");
                    m_iReceiveIdx=0;
                    m_tRequest.uLength=4;
                    m_tRequest.bHeaderId=0x01;  // Make incomplete error package
                }
                else if(5<i)
                {
                    break;
                }
                i++;
            }
        }
        bStatus=GNetPlus_ReceiveHandler();
        if(GNETPLUS_STATUS_NONE!=bStatus)
        {
            if(GNETPLUS_STATUS_CHECK_ERROR==bStatus)
            {
                printf("Protocol CRC16 Error");
            }
            else if(GNETPLUS_STATUS_RECEIVE_TIMEOUT==bStatus)
            {
                printf("ReceivePackage: Timeout");
            }
            else
            {
                printf("ReceivePackage: Code: %02X", bStatus);
            }
            printf("\r\n");
        }
    }
    printf("\r\nTest Finish\r\n");
    return 0;
}
```

### Test Code Output

```Text
Receive CRC16 Error Test:
ReceivePackage: Code: F6

Get Firmware Version Test:
159082906 OnPackage(Request)[7]: 01001001007100
159082906 OnPackage(Response)[37]: 0100061F4E616D6520524F4D2D54303030312056312E30305231202832313037323329BF61
Firmware Version: Name ROM-T0001 V1.00R1 (210723)

Get Hardware Version Test:
159083109 OnPackage(Request)[7]: 0100100101B1C1
159083109 OnPackage(Response)[24]: 010006124E616D652048617264776172652056312E30C834
Hardware Version: Name Hardware V1.0

Get Unknown Version Test:
159083312 OnPackage(Request)[7]: 0100100102B081
159083312 OnPackage(Response)[7]: 01001501F8F211
Get Unknown Version Error: F8

Get Receive Package Timeout Test:
ReceivePackage: Timeout
ReceivePackage: Timeout

Test Finish
```
