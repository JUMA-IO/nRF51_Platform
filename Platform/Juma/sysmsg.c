/* Copyright (c) 2014 junfenggeng@gmail.com. All Rights Reserved.
 */
#include "sys_common.h"
#include "stdio.h"
#include "firmware_version.h"
void enter_ota_mode(uint8_t type, uint16_t length, uint8_t* data);
void ota_on_message(uint8_t type, uint16_t length, uint8_t* data);
void dev_inf_dev_id_req(uint8_t type, uint16_t length, uint8_t* data);
void device_current_statue_req(uint8_t type, uint16_t length, uint8_t* data);
void device_option(uint8_t type, uint16_t length, uint8_t* data);
void spi_test(void);

		
void system_on_message(uint8_t type, uint16_t length, uint8_t* data)
{
	uint8_t vid_pid_info[8] ;
  
  uint32_t err_code;
  uint8_t len;
  uint8_t err_state[1] = {0x00};
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
		case 0x85:
			err_code = data_storage_read(0x10, &len, vid_pid_info);
			if(err_code == NRF_SUCCESS){
				ble_device_send(type,len,vid_pid_info);
			}else{
				ble_device_send(type,1,err_state);
			}
		break;
		case 0x86:
				ble_device_send(type,8,FIRMWARE_VERSION);
		break;
			case 0x87:
		if(length == 0x0008){
			err_code = manufacturer_storage_write(0x10, length, data);
			ble_device_send(type,1,err_state);
		}
		break;
		
    }
}
