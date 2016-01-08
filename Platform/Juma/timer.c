#include "sys_common.h"

static uint32_t timer_prescalar;
static uint32_t timer_type = TIMER_PERIODIC;

#define USER_TIMER    NRF_TIMER1
#define TIMER_IRQn                TIMER1_IRQn
//#define TIMER_IRQHandler        TIMER2_IRQHandler

void TIMER_IRQHandler(void);

void timer_init(uint8_t prescalar, uint8_t type)
{
  timer_prescalar = prescalar;
  timer_type = type;
}

uint32_t timer_start(uint16_t value)
{
  uint32_t err_code;
  
  if((err_code = apply_sys_res_timer1(TIMER_IRQHandler)) != 0)
  {
    return err_code;
  }
  
  USER_TIMER->PRESCALER = timer_prescalar;
  USER_TIMER->TASKS_CLEAR = 1;
  USER_TIMER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
  USER_TIMER->CC[0] = value;
  USER_TIMER->MODE = TIMER_MODE_MODE_Timer;
  USER_TIMER->EVENTS_COMPARE[0] = USER_TIMER->EVENTS_COMPARE[1] = USER_TIMER->EVENTS_COMPARE[2] = USER_TIMER->EVENTS_COMPARE[3] = 0;     

  sd_nvic_SetPriority(TIMER_IRQn, 3);
  sd_nvic_EnableIRQ(TIMER_IRQn);

  if(timer_type == TIMER_ONECE)
  {
    USER_TIMER->SHORTS = TIMER_SHORTS_COMPARE0_STOP_Msk;
  }
  else 
  {
    USER_TIMER->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk;
  }
  
  USER_TIMER->INTENSET = TIMER_INTENSET_COMPARE0_Msk;  
  USER_TIMER->TASKS_START = 1;
    
  return JUMA_ERROR_CODE_SUCCEED;
}

void timer_stop(void)
{
  USER_TIMER->TASKS_STOP = 1;
  
  free_sys_res_timer1();
}

void TIMER_IRQHandler(void)
{
  USER_TIMER->EVENTS_COMPARE[0] = 0;
  
  if(TIMER_ONECE == timer_type)
  {
    timer_stop();
  }
  
  timer_on_fired();
}
