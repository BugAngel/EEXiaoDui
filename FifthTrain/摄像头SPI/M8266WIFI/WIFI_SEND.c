
#include "sys.h"
#include "WIFI_SEND.h"
#include "led.h"

void M8266WIFI_Init(void)
{
	 u8  link_no=0;

  //u8 M8266WIFI_Reboot_And_Config_Connection(u8 tcp_udp, u16 local_port, u8* remote_ip, u16 remote_port, u8 link_no)
	//0±Ì æ ß∞‹¡À
	if(M8266WIFI_Config_Connection_via_SPI(TEST_CONNECTION_TYPE, TEST_LOCAL_PORT, TEST_REMOTE_IP_ADDR, TEST_REMOTE_PORT, link_no)==0)
	{		
		  // led flash in 1Hz when error
			while(1)
			{
	      LED_set(0, 0); LED_set(1, 0); M8266WIFI_Module_delay_ms(500);
	      LED_set(0, 1); LED_set(1, 1); M8266WIFI_Module_delay_ms(500);
			}
	}
}
