#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf51_bitfields.h"
#include "nrf_soc.h"
#include "psm_api.h"
#include "dispatch.h"
#include "pwm.h"
#include "juma_common.h"
#include "app_util_platform.h"

typedef struct _light_t {
    uint8_t pins[3];
    uint8_t values[3];
} light_t;

static light_t m_light;

void ppi_enable_channel(uint32_t ch_num, volatile uint32_t *event_ptr, volatile uint32_t *task_ptr)
{
    if(ch_num >= 16)
    {
        return;
    }
    else
    {
    }
}

static void set_color(void* user_data, uint32_t time)
{
    light_t* light = (light_t*) user_data;
    int i;

    PWM_TIMER->TASKS_CLEAR = 1;

    // setup PWM
    PWM_TIMER->POWER = 1;

    PWM_TIMER->PRESCALER = 7; // 256k
    PWM_TIMER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    PWM_TIMER->CC[3] = 255;
    PWM_TIMER->MODE = TIMER_MODE_MODE_Timer;
    PWM_TIMER->SHORTS = TIMER_SHORTS_COMPARE3_CLEAR_Msk;
    PWM_TIMER->EVENTS_COMPARE[0] =
        PWM_TIMER->EVENTS_COMPARE[1] =
        PWM_TIMER->EVENTS_COMPARE[2] =
        PWM_TIMER->EVENTS_COMPARE[3] = 0;

    for(i = 0; i < 3; i++)
    {
        nrf_gpio_cfg_output(light->pins[i]);
        PWM_TIMER->CC[i] = light->values[i];

        nrf_gpio_cfg_output(light->pins[i]);
        ppi_enable_channel(i*2,  &PWM_TIMER->EVENTS_COMPARE[i], &NRF_GPIOTE->TASKS_OUT[i]);
        ppi_enable_channel(i*2+1,&PWM_TIMER->EVENTS_COMPARE[3], &NRF_GPIOTE->TASKS_OUT[i]);
        nrf_gpiote_task_config(i, light->pins[i], GPIOTE_CONFIG_POLARITY_Toggle, NRF_GPIOTE_INITIAL_VALUE_HIGH);  
    }

    PWM_TIMER->TASKS_START = 1;
}

void light_setColor(uint8_t* pins, uint8_t* values)
{
    memcpy(m_light.pins, pins, 3);
    memcpy(m_light.values, values, 3);

    scheduleNow(set_color, (void*)&m_light);
}
