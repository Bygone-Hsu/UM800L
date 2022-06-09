# UM800L Command Set

## Index of Contents

1. [History](#1-history)
2. [Symbols](#2-symbols)
3. [GNetPlus Protocol](GNetPlus%20Protocol.md)
   1. [Basic Package](GNetPlus%20Protocol.md#basic-package)
   2. [CRC16 Calculation](GNetPlus%20Protocol.md#crc16-calculation)
   3. [GNetPlus Implement](GNetPlus%20Protocol.md#gnetplus-implement)
4. [Commands](#4-commands)
   1. [Get Version](#4-1-get-version-command)
   2. [Set Working Mode](#4-2-set-working-mode)
   3. [Read EEPROM](#4-3-read-eeprom)
   4. [Write EEPROM](#4-4-write-eeprom)
5. [Error Code](#5-error-code)

## 1. History

### 2022/06/09
* Fix wrong example of entering auto mode

### 2022/04/15

* Initial Version

## 2\. Symbols

Symbols to this command set are as follows

### Data Types

The RF385-00 commands digital data order uses **Big Endian**

| Type | Bytes | Value Range     | Description              |
| :--: | :---: | :-------------- | ------------------------ |
|  s8  |   1   | -128 \~ 127     | 8 bit signed integer     |
| s16  |   2   | -32768 \~ 32767 | 16 bit signed integer    |
|  u8  |   1   | 0 \~ 255        | 8 bit unsigned integer   |
| u16  |   2   | 0 \~ 65535      | 16 bit  unsigned integer |

## 4\. Commands

RF385-00 commands is base on [GNetPlus Protocol](GNetPlus%20Protocol.md)

## *Request Code List*

| Name                                      | Code  | Description                     |
|:-----------------------------------------:|:-----:| ------------------------------- |
| [Get Version](#4-1-get-version-command)   | `10h` | Get Firmware / Hardware version |
| [Set Working Mode](#4-2-set-working-mode) | `12h` | Set Working Mode                |
| [Read EEPROM](#4-3-read-eeprom)           | `24h` | Read data from EEPROM           |
| [Write EEPROM](#4-4-write-eeprom)         | `22h` | Write data to EEPROM            |

## 4-1\. Get Version Command

Request Code is 10h

### Get Firmware/Hardware Version

* Parameter

| Offset | Bytes | Type | Name  | Description                                                 |
|:------:|:-----:|:----:| ----- | ----------------------------------------------------------- |
| 0      | 1     | u8   | Index | Version Index<br>0: Firmware Version<br>1: Hardware Version |

* Response
  * NAK: Response an [error code](#4\.-error-code)
  * ACK: Success with response data
    * Response Data

| Offset | Bytes | Type | Name    | Description                  |
|:------:|:-----:|:----:| ------- | ---------------------------- |
| 0      | N     | u8   | Version | Firmware or Hardware Version |

#### Example

For the NAK response of the example command, please refer to [GNetPlus's example](GNetPlus%20Protocol.md#response-a-nak-package)

* Get Firmware Version

<br />`[Send 7 Bytes] Get Version (10h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `10` | `01` | `00` | `71` | `00` |      |      |      |      |      |      |      |      |      | `.....q.`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` 00`                                                   | `00h: Device Address: Broadcast (Any Device)`                  |
| `02`     | `1`     | ` 10`                                                   | `10h: Code: Get Version`                                       |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` 00`                                                   | `0: Version Index: Firmware Version`                           |
| `05`     | `2`     | ` 71 00`                                                | `CRC16`                                                        |

<br />`[Receive 44 Bytes] Get Version Response (06h: Code: ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `26` | `55` | `4D` | `38` | `30` | `30` | `4C` | `2D` | `30` | `30` | `20` | `52` | `4F` | `...&UM800L-00 RO`                                      |
| `10`     | `4D` | `2D` | `54` | `31` | `34` | `33` | `38` | `20` | `56` | `31` | `2E` | `30` | `30` | `52` | `35` | `20` | <code>M-T1438 V1.00R5 </code>                           |
| `20`     | `28` | `32` | `31` | `31` | `31` | `31` | `38` | `30` | `29` | `00` | `5E` | `DE` |      |      |      |      | `(2111180).^.`                                          |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div>                                                                                                      | <div style='min-width:32em' align='center'>`Description`</div>   |
|:--------:|:-------:|:------------------------------------------------------------------------------------------------------------------------------------------------------------ |:---------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                                                                                                                        | `SOH (Start of Heading)`                                         |
| `01`     | `1`     | ` FF`                                                                                                                                                        | `Device Address`                                                 |
| `02`     | `1`     | ` 06`                                                                                                                                                        | `06h: Code: ACK`                                                 |
| `03`     | `1`     | ` 26`                                                                                                                                                        | `Data Length`                                                    |
| `04`     | `38`    | ` 55 4D 38 30 30 4C 2D 30` <br /> ` 30 20 52 4F 4D 2D 54 31` <br /> ` 34 33 38 20 56 31 2E 30` <br /> ` 30 52 35 20 28 32 31 31` <br /> ` 31 31 38 30 29 00` | `Firmware Version:`<br />`UM800L-00 ROM-T1438 V1.00R5 (2111180)` |
| `2A`     | `2`     | ` 5E DE`                                                                                                                                                     | `CRC16`                                                          |

## 4-2\. Set Working Mode

Request Code is 12h

### Enter Auto / Command Mode

* Parameter

| Offset | Bytes | Type | Name | Description                                                    |
|:------:|:-----:|:----:| ---- | -------------------------------------------------------------- |
| 0      | 1     | u8   | Mode | New [Working Mode](#working-mode)<br />0: Auto<br />1: Command |

* Response
  * NAK: Response an [error code](#4\.-error-code). 
  * ACK: Success with response data
    * Response Data

| Offset | Bytes | Type | Name | Description                                                        |
|:------:|:-----:|:----:| ---- | ------------------------------------------------------------------ |
| 0      | 1     | u8   | Mode | Current [Working Mode](#working-mode)<br />0: Auto<br />1: Command |

#### Working Mode

| Value | Description                                                      |
| ----- | ---------------------------------------------------------------- |
| 0     | Auto Mode:<br />Automatic inventory tag and output tag data      |
| 1     | Command Mode:<br />Waiting for the command without inventory Tag |

#### Example

For the NAK response of the example command, please refer to [GNetPlus's example](GNetPlus%20Protocol.md#response-a-nak-package)

* Enter Auto Mode
  <br />`[Send 7 Bytes] Enter Auto Mode (12h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `12` | `01` | `00` | `B1` | `A1` |      |      |      |      |      |      |      |      |      | `.......`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` 00`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 12`                                                   | `00h: Code: Change Working Mode`                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` 00`                                                   | `00h`: [Working Mode](#working-mode): Auto Mode                |
| `05`     | `2`     | ` B1 A1`                                                | `CRC16`                                                        |

<br />`[Receive 7 Bytes] Enter Auto Mode Response (06h: Code: ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `01` | `00` | `A1` | `D1` |      |      |      |      |      |      |      |      |      | `.......`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 06`                                                   | `06h: Code: ACK`                                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` 00`                                                   | `00h`: [Working Mode](#working-mode): Auto Mode                |
| `05`     | `2`     | ` A1 D1`                                                | `CRC16`                                                        |

* Enter Command Mode
  <br />`[Send 7 Bytes] Enter Command Mode (12h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `12` | `01` | `01` | `71` | `60` |      |      |      |      |      |      |      |      |      | <code>.....q\`</code>                                   |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` 00`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 12`                                                   | `00h: Code: Change Working Mode`                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` 01`                                                   | `01h`: [Working Mode](#working-mode): Command Mode             |
| `05`     | `2`     | ` 71 60`                                                | `CRC16`                                                        |

<br />`[Receive 7 Bytes] Enter Command Mode Response (06h: Code: ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `01` | `00` | `A1` | `D1` |      |      |      |      |      |      |      |      |      | `.......`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 06`                                                   | `06h: Code: ACK`                                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` 00`                                                   | `01h`: [Working Mode](#working-mode): Command Mode             |
| `05`     | `2`     | ` A1 D1`                                                | `CRC16`                                                        |

## 4-3\. Read EEPROM

Request Code is 24h

### Read Device Settings from EEPROM  (Registers)

* Parameter

| Offset | Bytes | Type | Name    | Description                                                                                                                                                |
|:------:|:-----:|:----:| ------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0      | 2     | u16  | Address | Read [EEPROM Address](EEPROM%20Table.md#eeprom-table) (Big Endian)<br />Bit 0~15: Address<br /><br />Bit 15: Memory Option<br />0: EEPROM<br />1: Register |
| 1      | 1     | u8   | Length  | Number of bytes read from EEPROM                                                                                                                           |

* Response
  * NAK: Response an [error code](#4\.-error-code). 
  * ACK: Success with response data
    * Response Data

| Offset | Bytes | Type | Name | Description |
|:------:|:-----:|:----:| ---- | ----------- |
| 0      | N     | u8   | Data | EEPROM Data |

#### Example

For the NAK response of the example command, please refer to [GNetPlus's example](GNetPlus%20Protocol.md#response-a-nak-package)

* Read RF Power from EEPROM
  `[Send 9 Bytes] Read RF Power from EEPROM (24h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `24` | `03` | `00` | `00` | `01` | `98` | `B1` |      |      |      |      |      |      |      | `..$......`                                             |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` 00`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 24`                                                   | `24h: Code: Read EEPROM Command`                               |
| `03`     | `1`     | ` 03`                                                   | `Data Length`                                                  |
| `04`     | `2`     | ` 00 00`                                                | `0000h: EEPROM Address: RF Power`                              |
| `06`     | `1`     | ` 01`                                                   | `01h: Read EEPROM Length`                                      |
| `07`     | `2`     | ` 98 B1`                                                | `CRC16`                                                        |

<br />`[Receive 7 Bytes] Read RF Power from EEPROM Response (06h: Code: ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `01` | `0A` | `A6` | `51` |      |      |      |      |      |      |      |      |      | `......Q`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 06`                                                   | `06h: Code: ACK`                                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` 0A`                                                   | `0Ah: Data: RF Power 10 dBm`                                   |
| `05`     | `2`     | ` A6 51`                                                | `CRC16`                                                        |

* Read Baudrate from EEPROM
  <br />`[Send 9 Bytes] Read Baudrate from EEPROM (24h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `24` | `03` | `00` | `10` | `01` | `58` | `BC` |      |      |      |      |      |      |      | `..$....X.`                                             |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` 00`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 24`                                                   | `24h: Code: Read EEPROM Command`                               |
| `03`     | `1`     | ` 03`                                                   | `Data Length`                                                  |
| `04`     | `2`     | ` 00 10`                                                | `0010h: EEPROM Address: Baudrate`                              |
| `06`     | `1`     | ` 01`                                                   | `01h: Read EEPROM Length`                                      |
| `07`     | `2`     | ` 58 BC`                                                | `CRC16`                                                        |

<br />`[Receive 7 Bytes] Read Baudrate from EEPROM Response (06h: Code: ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `01` | `FF` | `E1` | `91` |      |      |      |      |      |      |      |      |      | `.......`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 06`                                                   | `06h: Code: ACK`                                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` FF`                                                   | `FFh: Data: Baudrate 115200 bps`                               |
| `05`     | `2`     | ` E1 91`                                                | `CRC16`                                                        |

## 4-4\. Write EEPROM

Request Code is 22h

### Write Device Settings to EEPROM (Registers)

* Parameter

| Offset | Bytes | Type | Name    | Description                                                                                                                                                                                                                                                                                                                        |
|:------:|:-----:|:----:| ------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0      | 2     | u16  | Address | Write [EEPROM Address](EEPROM%20Table.md#eeprom-table) (Big Endian)<br />Bit 0~14: EEPROM/Register Address please refer to [EEPROM Table](EEPROM%20Table.md#eeprom-table) <br /><br />Bit 15: Memory Option<br />0: EEPROM<br />1: Registers (Directly update the set value to Registers, and will force to enter the command mode) |
| 1      | N     | u8   | Data    | Write data.                                                                                                                                                                                                                                                                                                                        |

* Response
  * NAK: Response an [error code](#4\.-error-code).
  * ACK: Success with response data
    * Response Data

| Offset | Bytes | Type | Name | Description |
|:------:|:-----:|:----:| ---- | ----------- |
| 0      | N     | u8   | Data | EEPROM Data |

#### Example

For the NAK response of the example command, please refer to [GNetPlus's example](GNetPlus%20Protocol.md#response-a-nak-package)

* Write RF Power 10 dBm to EEPROM
  <br />`[Send 9 Bytes] Write RF Power 10 dBm to EEPROM (22h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `22` | `03` | `00` | `00` | `0A` | `5F` | `78` |      |      |      |      |      |      |      | `.."...._x`                                             |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` 00`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 22`                                                   | `24h: Code: Read EEPROM Command`                               |
| `03`     | `1`     | ` 03`                                                   | `Data Length`                                                  |
| `04`     | `2`     | ` 00 00`                                                | `0000h: EEPROM Address: RF Power`                              |
| `06`     | `1`     | ` 0A`                                                   | `0Ah: Data: RF Power 10 dBm`                                   |
| `07`     | `2`     | ` 5F 78`                                                | `CRC16`                                                        |

<br />`[Receive 7 Bytes] Write RF Power 10 dBm to EEPROM Response (06h: Code: ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `01` | `0A` | `A6` | `51` |      |      |      |      |      |      |      |      |      | `......Q`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 06`                                                   | `06h: Code: ACK`                                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` 0A`                                                   | `0Ah: Data: RF Power 10 dBm`                                   |
| `05`     | `2`     | ` A6 51`                                                | `CRC16`                                                        |

* Write Baudrate 115200 bps to EEPROM
  <br />`[Send 9 Bytes] Write Baudrate 115200 bps to EEPROM (22h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `22` | `03` | `00` | `10` | `FF` | `D8` | `B5` |      |      |      |      |      |      |      | `.."......`                                             |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` 00`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 22`                                                   | `24h: Code: Read EEPROM Command`                               |
| `03`     | `1`     | ` 03`                                                   | `Data Length`                                                  |
| `04`     | `2`     | ` 00 10`                                                | `0010h: EEPROM Address: Baudrate`                              |
| `06`     | `1`     | ` FF`                                                   | `FFh: Data: Baudrate 115200 bps`                               |
| `07`     | `2`     | ` D8 B5`                                                | `CRC16`                                                        |

<br />`[Receive 7 Bytes] Write Baudrate 115200 bps to EEPROM Response (06h: Code: ACK)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `FF` | `06` | `01` | `FF` | `E1` | `91` |      |      |      |      |      |      |      |      |      | `.......`                                               |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div> |
|:--------:|:-------:|:------------------------------------------------------- |:-------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                       |
| `01`     | `1`     | ` FF`                                                   | `Device Address`                                               |
| `02`     | `1`     | ` 06`                                                   | `06h: Code: ACK`                                               |
| `03`     | `1`     | ` 01`                                                   | `Data Length`                                                  |
| `04`     | `1`     | ` FF`                                                   | `FFh: Data: Baudrate 115200 bps`                               |
| `05`     | `2`     | ` E1 91`                                                | `CRC16`                                                        |

### Update EEPROM to Device Registers

Update EEPROM settings to device registers and EEPROM settings will take effect

* Parameter

| Offset | Bytes | Type | Name          | Description                                                                                                                      |
|:------:|:-----:|:----:| ------------- | -------------------------------------------------------------------------------------------------------------------------------- |
| 0      | 2     | u16  | Address       | Special update Address (Big Endian)<br />Bit 0~15: Address<br />FFFFh: Update EEPROM to Device Registers                         |
| 1      | 1     | u8   | Update Option | Update EEPROM to Reigsetrs option<br />0: Update All EEPROM to Registers<br />1: Update EEPROM without working Mode to Registers |

* Response
  * NAK: Response an [error code](#4\.-error-code).
  * ACK: Success

#### Example

For the response of the example command, please refer to [GNetPlus's example](GNetPlus%20Protocol.md#response-an-ack-package)

* Update EEPROM to Device Registers
  <br />`[Send 9 Bytes] Update EEPROM To Registers (22h: Code: Command)`

| `Offset` | `00` | `01` | `02` | `03` | `04` | `05` | `06` | `07` | `08` | `09` | `0A` | `0B` | `0C` | `0D` | `0E` | `0F` | <div style='min-width:8em' align='center'>`ASCII`</div> |
|:--------:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:----:|:------------------------------------------------------- |
| `00`     | `01` | `00` | `22` | `03` | `FF` | `FF` | `01` | `58` | `48` |      |      |      |      |      |      |      | `.."....XH`                                             |

| `Offset` | `Bytes` | <div style='min-width:12em' align='center'>`Data`</div> | <div style='min-width:32em' align='center'>`Description`</div>        |
|:--------:|:-------:|:------------------------------------------------------- |:--------------------------------------------------------------------- |
| `00`     | `1`     | ` 01`                                                   | `SOH (Start of Heading)`                                              |
| `01`     | `1`     | ` 00`                                                   | `Device Address`                                                      |
| `02`     | `1`     | ` 22`                                                   | `22h: Code: Command`                                                  |
| `03`     | `1`     | ` 03`                                                   | `Data Length`                                                         |
| `04`     | `2`     | ` FF FF`                                                | `FFFFh: EEPROM Address: Update to Registers`                          |
| `06`     | `1`     | ` 01`                                                   | `01h: Update Option: Update EEPROM without working Mode to Registers` |
| `07`     | `2`     | ` 58 48`                                                | `CRC16`                                                               |

<br />`[Receive 6 Bytes] Update EEPROM To Registers Response (06h: Code: ACK)`

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

## 5\. Error Code

| Value | Name          | Description                                                           |
|:-----:| ------------- | --------------------------------------------------------------------- |
| 00h   | ERR NONE      | No error occured                                                      |
| FFh   | ERR UNKNOWN   | Unknown error occured                                                 |
| FEh   | ERR NOMEM     | Not enough memory to perform the requested operation                  |
| FDh   | ERR BUSY      | Device or resource busy                                               |
| FCh   | ERR IO        | Generic IO error                                                      |
| FBh   | ERR TIMEOUT   | Error due to timeout                                                  |
| FAh   | ERR REQUEST   | Invalid request or requested function can't be executed at the moment |
| F9h   | ERR NOMSG     | No message of desired type                                            |
| F8h   | ERR PARAM     | Parameter error                                                       |
| F7h   | ERR PROTO     | Protocol error                                                        |
| F6h   | ERR PROTO CRC | Protocol CRC16 check error                                            |
