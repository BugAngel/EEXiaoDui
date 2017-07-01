
#ifndef __WIFI_SEND_H
#define __WIFI_SEND_H

#include "stdio.h"
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"

// Set TCP/UDP Connections
#define TEST_CONNECTION_TYPE   1        // 0 for UDP, 1 for TCP

#if (TEST_CONNECTION_TYPE==1)
#define TEST_LOCAL_PORT            0           // local port=0 then an updated random port will be generated each time. 
//To avoid the rejection by some TCP server due to repeative same ip:port
#endif

#define TEST_REMOTE_IP_ADDR    "192.168.191.1"
#define TEST_REMOTE_PORT        7777
#define TEST_SEND_DATA_SIZE   1024 //2048 // 1472 //1460

void M8266WIFI_Init(void);
#endif
