/* Copyright (c) 2014 junfenggeng@gmail.com. All Rights Reserved.
 */
#include "sys_common.h"
#include "stdio.h"

void enter_ota_mode(uint8_t type, uint16_t length, uint8_t* data);
void ota_on_message(uint8_t type, uint16_t length, uint8_t* data);
void dev_inf_dev_id_req(uint8_t type, uint16_t length, uint8_t* data);
void device_current_statue_req(uint8_t type, uint16_t length, uint8_t* data);
void device_option(uint8_t type, uint16_t length, uint8_t* data);

void spi_test(void);

void system_on_message(uint8_t type, uint16_t length, uint8_t* data)
{
	uint8_t value[2];
    switch (type) {
    case 0x80:  //divice id
        dev_inf_dev_id_req(type, length, data);
        break;
    case 0x81: // OTA
        ota_on_message(type, length, data);
        break;
    case 0x82: //enter OTA Mode
        enter_ota_mode(type, length, data);
        break;
    case 0x83: //device statue req
        device_current_statue_req(type, length, data);
        break;
		case 0x84: //device option
			device_option(type, length, data);
			break;
//		case 0x04:
//			 value[0] = random_generate();
//			ble_device_send(0x04, 1, value);
//			break;
    //case 
    }
}
