#ifndef _DEVICE_STATUE_H_
#define _DEVICE_STATUE_H_

#include "sys_common.h"
#include "juma_sys_res_manage.h"

typedef enum
{
  DEVICE_STATUE_APP = 0,
  DEVICE_STATUE_OTA = 1,
}device_statue_t;

void device_current_statue_req(uint8_t type, uint16_t length, uint8_t* data);
device_statue_t device_current_statue_get(void);
void device_current_statue_set(device_statue_t statue);

#endif
