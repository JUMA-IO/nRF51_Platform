#include "device_option.h"

void device_option_sys_reset(uint8_t type, uint16_t length, uint8_t* data)
{
	NVIC_SystemReset();
}

void device_option_ble_disconnect(uint8_t type, uint16_t length, uint8_t* data)
{
	ble_device_disconnect();
}

void device_option(uint8_t type, uint16_t length, uint8_t* data)
{
	uint8_t i = 0;
	
	if(length >= 1)
	{
		length = 1;
	}
	
	for(i=0; ((data[i] & 0x80) && (i < length)); i++);
	
	if(i<length)
	{
		switch(data[i])
		{
			//sys
			case 0x01:
				device_option_sys_reset(type, length, data);
				break;
			case 0x02:
				break;
			case 0x03:
				break;
			//ble
			case 0x11:
				device_option_ble_disconnect(type, length, data);
				break;
			case 0x12:
				break;
			default:
				break;
		}
	}
}







