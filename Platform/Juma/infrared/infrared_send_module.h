#ifndef _INFRARED_SEND_MODULE_H_
#define _INFRARED_SEND_MODULE_H_

#include "nrf51.h"
#include "infrared_send_pwm.h"
#include "juma_sys_res_manage.h"

#define MAX_SIGNAL_DATA_LENGTH 24



//function
extern void     ble_infrared_set_interrupt_function_point(IRQHandler_Point Point);
extern uint32_t ble_infrared_config(ble_infrared_init_struct * Init_struct);
extern uint32_t ble_infrared_send(const uint8_t * signal_data, uint8_t lenth);
extern uint32_t ble_infrared_get_statue(void);

#endif

