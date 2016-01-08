#include "device_statue.h"


device_statue_t device_current_statue;


void device_current_statue_req(uint8_t type, uint16_t length, uint8_t* data)
{
  ble_device_send(0x83, 1, (uint8_t *)&device_current_statue);
}

device_statue_t device_current_statue_get(void)
{
	return device_current_statue;
}

void device_current_statue_set(device_statue_t statue)
{
	device_current_statue = statue;
}

