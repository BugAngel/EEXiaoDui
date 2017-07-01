/********************************************************************
 * test_m8266wifi.c
 * .Description
 *     Source file of M8266WIFI testing application 
 * .Copyright(c) Anylinkin Technology 2015.5-
 *     IoT@anylinkin.com
 *     http://www.anylinkin.com
 *     http://anylinkin.taobao.com
 *  Author
 *     wzuo
 *  Date
 *  Version
 ********************************************************************/
 
#include "stdio.h"
#include "brd_cfg.h"
#include "M8266WIFIDrv.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"
#include "led.h"

	 // Set TCP/UDP Connections  
	 #define TEST_CONNECTION_TYPE   1  		// 0 for UDP, 1 for TCP

#if (TEST_CONNECTION_TYPE==1)
	 #define TEST_LOCAL_PORT  			0			// local port=0 then an updated random port will be generated each time. 
	                                      //To avoid the rejection by some TCP server due to repeative same ip:port
#endif

#define TEST_REMOTE_IP_ADDR    "192.168.191.1"
#define TEST_REMOTE_PORT  	    7777
#define TEST_SEND_DATA_SIZE   1024 //2048 // 1472 //1460

void M8266WIFI_Test(void)
{
	 u16 status = 0;
	 u8  link_no=0;
   u8  snd_data[TEST_SEND_DATA_SIZE]="今天的风儿，甚是喧嚣啊\r\n";
	 u16 sent = 0;
	 u16 batch;	 

  //u8 M8266WIFI_Reboot_And_Config_Connection(u8 tcp_udp, u16 local_port, u8* remote_ip, u16 remote_port, u8 link_no)
	//0表示失败了
	if(M8266WIFI_Config_Connection_via_SPI(TEST_CONNECTION_TYPE, TEST_LOCAL_PORT, TEST_REMOTE_IP_ADDR, TEST_REMOTE_PORT, link_no)==0)
	{		
		  // led flash in 1Hz when error
			while(1)
			{
	      LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(500);
	      LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(500);
			}
	}
	 
   link_no=0;
	
 	 for(batch=0; ; batch++)
	 {
			
			//u16 M8266WIFI_SPI_Send_Data(u8 data[], u16 data_len, u8 link_no, u16* status)
      sent = M8266WIFI_SPI_Send_Data(snd_data, TEST_SEND_DATA_SIZE, link_no, &status);
			if( (sent!=TEST_SEND_DATA_SIZE) || ( (status&0xFF)!= 0 ) ) 
			{
				if( (status&0xFF)==0x12 )  				// 0x12 = Module send buffer full
				{                              
			      M8266HostIf_delay_us(250);       // if 500Kbytes/s, need 2ms for 1Kbytes packet to be sent. If use 250us here, may repeat query 8- times, but in a small interval
					  //M8266WIFI_Module_delay_ms(1);	 // if 500Kbytes/s, need 2ms for 1Kbytes packet to be sent. If use 1ms here,   may repeat query 2- times, but in a large interval				  
				}
				else if(  ((status&0xFF)==0x14)   // 0x14 = connection of link_no not present
					      ||((status&0xFF)==0x15) ) // 0x15 = connection of link_no closed
				{
					// do some work here
					  M8266HostIf_delay_us(250);
				}
				else
				{
					// do some work here
					  M8266HostIf_delay_us(250);
				}
				batch--; // resend
			}
	 } // end of for(batch=0; ; batch++)

} // end of M8266WIFI_Test
