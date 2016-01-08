

#include "sys_common.h"



void dev_inf_dev_id_req(uint8_t type, uint16_t length, uint8_t* data)
{
  uint8_t buff[128];
  if(length==0x01)
  {
    if(data[0] == 0x00)
    {
      ble_device_get_id(buff, 16);
      ble_device_send(0x80, 16, buff);
    }
  }
}


