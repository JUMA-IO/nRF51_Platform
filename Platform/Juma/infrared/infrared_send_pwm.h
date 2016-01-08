#ifndef _INFRARED_SEND_H_
#define _INFRARED_SEND_H_

#include "nrf51.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "app_error.h"
#include "nrf_soc.h"
#include "juma_sdk_types.h"

//resource of shock 

// one gpio pin  provite on config processing
#define SHOCK_TIMER         2

#define SHOCK_GPIOTE        1
#define SHOCK_PPI_CHAN1     0
#define SHOCK_PPI_CHAN2     1


#define SHOCK_PIN_NORMAL_LEVEL 0 

//resource of infrared signal 

//one gpio pin provite on config processing
#define GPIOTE_CHAN_OUTPUT                2

#define INFRARD_SIGNAL_PPI_CHAN_0         2
#define INFRARD_SIGNAL_PPI_CHAN_1         3
#define INFRARD_SIGNAL_PPI_CHAN_2         4
#define INFRARD_SIGNAL_PPI_CHAN_3         5

#define INFRARD_SIGNAL_PPI_CHAN_COUNT     6

#define INFRARD_SIGNAL_SEND_MODULE_TIMER_CODE 1					


extern uint32_t ble_infrared_pwm_config(ble_infrared_pwm_config_struct);
extern uint32_t ble_infrared_pwm_send(const uint8_t *, uint8_t);



#endif



