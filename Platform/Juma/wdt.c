#include "sys_common.h"

#define JUMA_SDK_WDT_REQ_RR_VALUE 0x6E524635


uint32_t watchDog_Config(uint32_t juma_wdt_en)
{
  NRF_WDT->RREN = juma_wdt_en; 
  
  return JUMA_ERROR_CODE_SUCCEED;
}

uint32_t watchDog_Start(uint32_t juma_wdt_timer_out_value)
{
  uint32_t wdt_config;
  
  NRF_WDT->EVENTS_TIMEOUT = 0;
  NRF_WDT->CRV = juma_wdt_timer_out_value;
  
  wdt_config = 0x00;
  wdt_config |= WDT_CONFIG_SLEEP_Pause << WDT_CONFIG_SLEEP_Pos;
  wdt_config |= WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos;
  NRF_WDT->CONFIG = wdt_config;
  
  NRF_WDT->INTENSET = WDT_INTENSET_TIMEOUT_Set;
  
  NVIC_SetPendingIRQ(WDT_IRQn);
  NVIC_SetPriority(WDT_IRQn, 0);
  NVIC_EnableIRQ(WDT_IRQn);
  
  NRF_WDT->TASKS_START = 1;
  
  return JUMA_ERROR_CODE_SUCCEED;
}

void watchDog_sys_SDK_dog1_RR()
{
  NRF_WDT->RR[7] = JUMA_SDK_WDT_REQ_RR_VALUE;
}

void watchDog_user_dog1_RR()
{
  NRF_WDT->RR[0] = JUMA_SDK_WDT_REQ_RR_VALUE;
}

void watchDog_user_dog2_RR()
{
  NRF_WDT->RR[1] = JUMA_SDK_WDT_REQ_RR_VALUE;
}

void watchDog_user_dog3_RR()
{
  NRF_WDT->RR[2] = JUMA_SDK_WDT_REQ_RR_VALUE;
}

void watchDog_user_dog4_RR()
{
  NRF_WDT->RR[3] = JUMA_SDK_WDT_REQ_RR_VALUE;
}

void WDT_IRQHandler()
{
  if(NRF_WDT->EVENTS_TIMEOUT == 1)
  {    
    watchDog_on_timerout(NRF_WDT->REQSTATUS);
    
    NRF_WDT->EVENTS_TIMEOUT = 0;
  }
}









