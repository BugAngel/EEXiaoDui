/********************************************************************
 * M8266WIFIDrv.h
 * .Description
 *     Header file of M8266WIFI Dirver Layer
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 * .Author:	wzuo(wzuo@anylinkin.com, 1521340710@qq.com)
 * .History
 *     - V1.1.5-3, 20170405
 ********************************************************************/
#ifndef _M8266WIFI_H_
#define _M8266WIFI_H_

//#include "ht32f1655_56.h"

typedef unsigned          char uint8_t;
typedef unsigned short    int  uint16_t;
typedef unsigned          int  uint32_t;
typedef   signed          char sint8_t;

typedef uint32_t  				u32;
typedef uint16_t 					u16;
typedef uint8_t  					u8;
typedef sint8_t           s8;


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   1. APIs releated to Wifi Packet Data Send and Receive                        //
//       - M8266HostIf_SPI_Select                                                 //
//       - M8266WIFI_SPI_Interface_Communication_OK                               //
//       - M8266WIFI_SPI_Send_Data                                                //
//       - M8266WIFI_SPI_Has_DataReceived                                         //
//       - M8266WIFI_SPI_RecvData                                                 //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * M8266HostIf_SPI_Select                                                          *
 * Description                                                                     *
 *  1.To specify the SPI used by providing the base address of SPI used            *
 *  2.Called during M8266WIFI Module Initialisation before perform SPI             *
 *    access to M8266WIFI module                                                   *
 * Parameter(s):                                                                   *
 *  1. spi_base_addr: the base address of used                                     *
 *     you may use M8266WIFI_INTERFACE_SPI defined in brd_cfg.h as example         *
 *  2. spi_clock you set in HZ                                                     *
 *     e.g.  10500000 for 10.5MHz                                                  *
 * Return:                                                                         *
 *    none                                                                         *
 ***********************************************************************************/
void M8266HostIf_SPI_Select(uint32_t spi_base_addr, uint32_t spi_clock);


void M8266HostIf_SPI_DMA_Select(uint32_t spi_tx_dam_base_addr, uint32_t spi_tx_dam_stream_base_addr, uint32_t spi_rx_dam_base_addr, uint32_t spi_rx_dam_stream_base_addr);



/***********************************************************************************
 * M8266WIFI_SPI_Interface_Communication_OK                                        *
 * Description                                                                     *
 *  1. To write bytes and then read out from M8266WIFI module registers            *
 *     to check whether the fundamentai SPI communication (read/write)             *
 *     is stablly OK under the clock and interconnection                           *
 *  2. Called during the initialization of M8266WIFI Module                        *
 *     DONOT CALL IT AFTER MODULE HAS STARTED THE NORMAL OPERATION                 *
 *  3. You may repeat calling it in your debug code for speed stablility           *
 *     test during your stress performance test to your produc                     * 
 * Parameter(s):                                                                   *
 *  1. byte:  a pointer to the byte read out during test                           *
 *     - normally the data should be 0x41 during test.                             *
 *       if it is 0x45, this function will return 1 for success as well            *
 *     - If readout other value, it may indicating the fundamental SPI             *
 *       communication is not OK. e.g. 0xFF may indicate that your spi             *
 *       port has problem such as incoreect interconnection or spi host            *
 *       initialization                                                            *
 *     - user NULL if you don't expect this data                                   * 
 * Return:                                                                         *
 *   1. success                                                                    *
 *   0. failure                                                                    *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Interface_Communication_OK(u8* byte);	

/***********************************************************************************
 * M8266WIFI_SPI_TxdBuffer_Free                                                    *
 * .Description:                                                                   *
 *     To check whether the M8266WIFI module TxdBuffer is free                     *
 * .Parameters                                                                     *
 *     None                                                                        *
 * .Return value:                                                                  *
 *     1, if the TxdBuffer is free                                                 *
 *     0, if the TxdBuffer is not free                                             *
 ***********************************************************************************/
u8 M8266WIFI_SPI_TxdBuffer_Free(void);

/***********************************************************************************
 * M8266WIFI_SPI_Send_Data                                                         *
 * .Description:                                                                   *
 *     To send Data to WIFI via M8266 module SPI                                   *
 * .Parameters                                                                     *
 *     1. Data   : the pointer to the Data buffer to be sent                       *
 *     2. len    : the length the Data buffer to be sent                           *
 *     3. link_no: the wifi service link number sent to                            *
 *     4. pointer to return errcode(LSB) and status(MSB) when error encountered    *
 *        use NULL if you don't expect errcode and status                          *
 *        errcode:                                                                 *
 *            = 0x10: timeout when wait Module spi rxd Buffer ready                *
 *            = 0x11: timeout when wait wifi to send data                          *
 *            = 0x12: Module Sending Buffer full                                   *
 *              Module Buffer full defined as                                      *
 *              - If total size of packets waiting to send via WIFI > 5*1024, or   *
 *              - If counts     of packets waiting to send via WIFI > 8            *
 *            = 0x13: Wrong link_no used                                           *
 *            = 0x14: connection by link_no not present                            *
 *            = 0x15: connection by link_no closed                                 *
 *            = 0x1F: Other errors                                                 *
 * .Return value:                                                                  *
 *     Actually length that has been sent successfuuly                             *
 ***********************************************************************************/
u16 M8266WIFI_SPI_Send_Data(u8 Data[], u16 Data_len, u8 link_no, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Has_DataReceived                                                  *
 * .Description:                                                                   *
 *     To check whether the M8266WIFI module has received data awaiting master     *
 *     to fetch away                                                               *
 * .Parameters                                                                     *
 *     None                                                                        *
 * .Return value:                                                                  *
 *     1, if the M8266WIFI module has received data from wifi                      *
 *     0, if the M8266WIFI module has not received data from wifi                  *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Has_DataReceived(void);

/***********************************************************************************
 * M8266WIFI_SPI_RcvData                                                           *
 * .Description:                                                                   *
 *     To receive the wifi data from M8266WIFI                                     *
 * .Parameters                                                                     *
 *     1. Data           - the buffer to contained the received Data               *
 *     2. max_len        - the max length of Data to fetech                        *
 *     3. max_wait_in_ms - the max timeout to wait for the Data                    *
 *     4. link_no        - pointer to return the link_no that current wifi Data    *
 *                         come from. use NULL if you don't expect it returned     *
 *     5. status         - pointer to return errcode(LSB) and status(MSB)          *
 *                         when error encountered. Use NULL if you don't expect    *
 *                         them returned                                           *
 *        errcode:                                                                 *
 *            = 0x20: timeout when wait module has received data via WIFI          *
 *            = 0x22: no date in Module wifi receive buffer                        *
 *            = 0x23: Read data from the left of last packets in Module wifi       *
 *                    receive buffer                                               *
 *            = 0x24: The packet in the Module wifi receive buffer is larger       *
 *                    in size than the max_len specified here. Only part of        *
 *                    the packet received                                          *
 *            = 0x2F: Other errors                                                 *
 * .Return value:                                                                  *
 *     - the size of larger packet first in the Module wifi receive buffer         *
 *       if errcode = 0x24                                                         *
 *     - the actual length of wifi packet received successfully                    *
 *       if others                                                                 *
 ***********************************************************************************/
u16 M8266WIFI_SPI_RecvData(u8 Data[], u16 max_len, uint16_t max_wait_in_ms, u8* link_no, u16* status);


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   2. APIs releated to Module Query and Configurations via SPI                  //
//       - M8266HostIf_SPI_Select                                                 //
//                                                                                //
//       - M8266WIFI_SPI_Get_Opmode                                               //
//       - M8266WIFI_SPI_Set_Opmode                                               //
//       - M8266WIFI_SPI_STA_Connect_Ap                                           //
//       - M8266WIFI_SPI_Get_STA_Connection_Status                                //
//       - M8266WIFI_SPI_Get_STA_IP_Addr                                          //
//       - M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI                          //
//                                                                                //
//       - M8266WIFI_SPI_StartSmartConfig                                         //
//       - M8266WIFI_SPI_StartWpsConfig                                           //
//                                                                                //
//       - M8266WIFI_SPI_Setup_Connection                                         //
//       - M8266WIFI_SPI_Delete_Connection                                        //
//       - M8266WIFI_SPI_Query_Connection                                         //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************
 * M8266WIFI_SPI_Get_Opmode                                                        *
 * .Description:                                                                   *
 *     To get the op_mode(STA, AP, or STA+AP) of M8266WIFI module via SPI          *
 * .Parameter(s)                                                                   *
 *     1. op_mode : pointer to the op_mode returned                                *
 *        =1, 			STA mode                                                       *
 *        =2, 			AP mode                                                        *
 *        =3, 			STA+AP mode                                                    *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Get_Opmode(u8* op_mode, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Set_Opmode                                                        *
 * .Description:                                                                   *
 *     To set op_mode(STA, AP, or STA+AP) of M8266WIFI module via SPI              *
 * .Parameter(s)                                                                   *
 *     1. op_mode : the op_mode to set                                             *
 *        =1, 			set to STA mode                                                *
 *        =2, 			set to AP mode                                                 *
 *        =3, 			set to STA+AP mode                                             *
 *        =others, 	set to STA+AP mode                                             *
 *     2. saved   : to save into flash the opmode or not                           *
 *        =0,       not saved, i.e. after reboot setting will restore to previous  *
 *        =others,  saved, i.e. after reboot, the saved setting will be loaded     *
 *                  PLEASE DO NOT CALL IT EACH TIME OF BOOTUP WITH SAVED != 0      *
 *                  OR, THE FLASH ON MODULE MIGHT GO TO FAILURE DUE TO LIFT CYCLE  *
 *                  OF WRITE                                                       *
 *     3. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Set_Opmode(u8 op_mode, u8 saved, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_STA_Connect_Ap                                                    *
 * .Description:                                                                   *
 *     To connect the M8266WIFI STA to an AP or router via SPI                     *
 * .Parameter(s)                                                                   *
 *     1. ssid    : the ssid of AP connected to. Max len = 13 Bytes                *
 *     2. password: the passowrd of AP connecting to. Max len = 13 Bytes           *
 *     3. saved   : to save the ssid and password into flash the opmode or not     *
 *        =0,       not saved, i.e. after reboot setting will restore to previous  *
 *        =others,  saved, i.e. after reboot, the saved setting will be loaded     *
 *                  PLEASE DO NOT CALL IT EACH TIME OF BOOTUP WITH SAVED != 0      *
 *                  OR, THE FLASH ON MODULE MIGHT GO TO FAILURE DUE TO LIFT CYCLE  *
 *                  OF WRITE                                                       *
 *     4. timeout_in_s: max time in seconds waiting for being connected            *
 *     5. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_STA_Connect_Ap(u8 ssid[13+1], u8 password[13+1], u8 saved, u8 timeout_in_s, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Get_STA_Connection_Status                                         *
 * .Description:                                                                   *
 *     To get the connecting status of M8266WIFI STA to AP or routers via SPI      *
 * .Parameter(s)                                                                   *
 *     1. connection_status :  the connection status returned                      *
 *          =0, if station is in idle                                              *
 *          =1, if station is connecting to an AP                                  *
 *          =2, if station is to connect to an AP but with an wrong password       *
 *          =3, if station is to connect to an AP which is not found               *
 *          =4, if station failed to connect to the AP                             *
 *          =5, if station is connected to the AP and has got the IP successfully  *
 *          =255, if the module is in AP-only mode                                 *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Get_STA_Connection_Status(u8* connection_status, u16* status);


/***********************************************************************************
 * M8266WIFI_SPI_Get_STA_IP_Addr                                                   *
 * .Description:                                                                   *
 *     To get ip address of M8266WIFI STA via SPI                                  *
 * .Parameter(s)                                                                   *
 *     1. sta_ip :  the sta ip address returned if successful                      *
 *                  "0.0.0.0" returned if in AP-only mode or ip not achieved       *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Get_STA_IP_Addr(char* sta_ip , u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI                                   *
 * .Description:                                                                   *
 *     To get current AP info (SSID and RSSI) M8266WIFI STA connected to           *
 * .Parameter(s)                                                                   *
 *     1. ssid    : the current SSID returned which m8266wifi sta connected to     *
 *     2. rssi    : the rssi of current connected ssid. 31 if error                *
 *     3. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_STA_Query_Current_SSID_And_RSSI(char* ssid, s8* rssi, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Set_Tx_Max_Power                                                  *
 * .Description:                                                                   *
 *     To set the RF Tx Max Power via SPI comannd                                  *
 * .Parameter(s)                                                                   *
 *     1. tx_max_power : the max tx power in 0.25 dBm.                             *
 *                       range = 0~82, i.e. 0~20.5dBm, or 1mW~112mW                *
 *                               tx_max_power  dBm        P/mW                     *
 *                                 0            0          1.0                     *
 *                                 4            1          1.3                     *
 *                                12            3          2.0                     *
 *                                28            7          5.0                     *
 *                                40           10         10.0                     *
 *                                52           13         20.0                     *
 *                                68           17         50.0                     *
 *                                80           20        100.0                     *
 *                                82           20.5      112.20                    *
 *        upon bootup default, tx_max_power = 82, i.e. 20.5dBm or 112.20mW         *
 *                                                                                 *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Set_Tx_Max_Power(u8 tx_max_power, u16 *status);


/***********************************************************************************
 * M8266WIFI_SPI_StartSmartConfig                                                  *
 * .Description:                                                                   *
 *     To start/stop smart config of M8266WIFI via SPI                             *
 *     After perform a start operation, the M8266WIFI will be set in STA mode      *
 * .Parameter(s)                                                                   *
 *     1. en      : to start or stop                                               *
 *                  =0, to stop                                                    *
 *                  others, to start                                               *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_StartSmartConfig(u8 en, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_StartWpsConfig                                                    *
 * .Description:                                                                   *
 *     To start/stop wps config of M8266WIFI via SPI                               *
 *     After perform a start operation, the M8266WIFI will be set in STA mode      *
 * .Parameter(s)                                                                   *
 *     1. en      : to start or stop                                               *
 *                  =0, to stop                                                    *
 *                  others, to start                                               *
 *     2. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_StartWpsConfig(u8 en, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Setup_Connection                                                  *
 * .Description:                                                                   *
 *     To setup a UDP connection or an TCP client connection via SPI               *
 * .Parameter(s)                                                                   *
 *     1. tcp_udp      : connection type                                           *
 *                       =0, udp                                                   *
 *                       =1, tcp client                                            *
 *     2. local_port   : local_port specified                                      *
 *                       =0, M8266WIFI module will generate a random local port    *
 *                      !=0, the local_port specified here will be used            *
 *     3. remote_ip    : string of ip address of remote connection                 *
 *     4. remote_port  : port of remote connection                                 *
 *     5. link_no      : the number of link used for multiple links. Max 4         *
 *     6. timeout_in_s : the max timeout connecting to a remote, unit in seconds   *
 *     7. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Setup_Connection(u8 tcp_udp, u16 local_port, u8* remote_ip, u16 remote_port, u8 link_no, u8 timeout_in_s, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Delete_Connection                                                 *
 * .Description:                                                                   *
 *     To delete/disconnect a client connection on M8266WIFI via SPI               *
 * .Parameter(s)                                                                   *
 *     1. link_no : the number of link to be deleted/disconnected                  *
 *     2. status  : pointer to the status upon failure                             *
 *     6. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Delete_Connection(u8 link_no, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Query_Connection                                                  *
 * .Description:                                                                   *
 *     To Query the information about a connection on M8266WIFI via SPI            *
 *     including connection type, state, local port, remote_ip and port, etc       *
 * .Parameter(s)                                                                   *
 *     1. link_no 				: the number of link to query                            *
 *     2. connection_type	:	pointer to the connection type returned                *
 *                          = 0, udp                                               *
 *                          = 1, tcp                                               *
 *                          = others, invalid                                      *
 *                          Use NULL if you don't expect it returned               *
 *     3. connection_state:	pointer to the connection state returned               *
 *                          = 0, invalid                                           *
 *                          = 1, wait                                              *
 *                          = 2, listen                                            *
 *                          = 3, connect                                           *
 *                          = 4, write                                             *
 *                          = 5, read                                              *
 *                          = 6, close                                             *
 *                          = others, invalid                                      *
 *                          Use NULL if you don't expect it returned               *
 *     4. local_port      : pointer to the local port returned                     *
 *                          Use NULL if you don't expect it returned               *
 *     5. remote_ip       : pointer to the remote ip returned                      *
 *                          Use NULL if you don't expect it returned               *
 *     6. remote_port     : pointer to the remote port returned                    *
 *                          Use NULL if you don't expect it returned               *
 *     7. status  : pointer to return errcode(LSB) and status(MSB) upon error      *
 *                  Use NULL if you don't expect them returned                     *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
u8 M8266WIFI_SPI_Query_Connection(u8 link_no, u8* connection_type, u8* connection_state, u16* local_port, u8* remote_ip, u16* remote_port, u16* status);


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   3. APIs releated to query driver and Module infomation                       //
//       - M8266WIFI_SPI_Get_Module_Info                                          //
//       - M8266WIFI_SPI_Get_Driver_Info                                          //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

 /**********************************************************************************
 * M8266WIFI_SPI_Get_Module_Info                                                   *
 * .Description:                                                                   *
 *     To get the M8266WIFI module information, such as ESP8266_ID,                *
 *     Flash Size, Firmware Version                                                *
 * .Parameter(s)                                                                   *
 *     1. esp8266_id: the pointer to the returned esp8266_id                       *
 *                    Use NULL if you don't expect it returned                     *
 *     2. flash_size: the pointer to the returned flash_size                       *
 *                    returned value                                               *
 *                      = 0, if 512KB                                              *
 *                      = 1, if 1MB                                                *
 *                      = 2, if 2MB                                                *
 *                      = 3, if 4MB                                                *
 *                      = others, invalid                                          *
 *                    Use NULL if you don't expect it returned                     *
 *     3. fw_ver    : the pointer to the returned firmware version                 *
 *                    buffer size should be no less than 16 bytes                  *
 *                    returned value e.g. "1.1.4-4"                                *
 *                    Use NULL if you don't expect it returned                     *
 *     4. status    : pointer to return errcode(LSB) and status(MSB) upon error    *
 *                    Use NULL if you don't expect them returned                   *
 * .Return value:                                                                  *
 *     =1, success                                                                 *
 *     =0, has error(s)                                                            *
 ***********************************************************************************/
 u8 M8266WIFI_SPI_Get_Module_Info(u32* esp8266_id, u8* flash_size, char* fw_ver, u16* status);

/***********************************************************************************
 * M8266WIFI_SPI_Get_Driver_Info                                                   *
 * .Description:                                                                   *
 *     To get the M8266WIFI driver information                                     *
 * .Parameter(s)                                                                   *
 *     1. drv_info: the pointer to the returned driver information buffer.         *
 *                  buffer size should be no less than 64 Bytes                   *
 *                  e.g. "ANYLINKIN M8266WIFI SPI DRIVER V1.4, 20170316"           *
 * .Return value                                                                   *
 *     driver_info: the pointer to the returned driver information                 *
 ***********************************************************************************/
char* M8266WIFI_SPI_Get_Driver_Info(char* drv_info);


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//   Below specifications are required by M8266WIFIDrv.lib                        //
//   And should be implemented according to the hardware in M8266HostIf.c         //
//   Put here kust for purpose ofheader specification                             //
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void M8266HostIf_Set_nRESET_Pin(u8 level);
void M8266HostIf_Set_SPI_nCS_Pin(u8 level);
u8   M8266HostIf_SPI_ReadWriteByte(u8 byte);
void M8266HostIf_delay_us(u8 nus);


#endif
