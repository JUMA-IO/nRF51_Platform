#include "juma_sys_res_manage.h"
#include "juma_error_code.h"
#include "stdlib.h"

#define RES_CURRENT_BUSY      1
#define RES_CURRENT_FREE      0


///////////////////TIMER1///////////////////////////////

JUMA_Sys_Res juma_sys_res_timer1 = {
  .Current_Statue = RES_CURRENT_FREE,
  .Point          = NULL,
};

void TIMER1_IRQHandler(void)
{
  if(juma_sys_res_timer1.Point)
  {
    juma_sys_res_timer1.Point();
  }
}

uint32_t apply_sys_res_timer1(IRQHandler_Point Point)
{
  if(RES_CURRENT_BUSY == juma_sys_res_timer1.Current_Statue)
  {
    return JUMA_ERROR_CODE_SYS_RES_TIMER1_BUSY;
  }
  
  if(NULL == Point)
  {
    NRF_TIMER1->INTENCLR = 0xFFFFFFFFUL;
  }
  
  juma_sys_res_timer1.Current_Statue = RES_CURRENT_BUSY;
  juma_sys_res_timer1.Point = Point;
  
  return JUMA_ERROR_CODE_SUCCEED;
}

uint32_t free_sys_res_timer1(void)
{
  juma_sys_res_timer1.Point           = NULL;
  juma_sys_res_timer1.Current_Statue  = RES_CURRENT_FREE;
  
  return JUMA_ERROR_CODE_SUCCEED;
}

///////////////////////TIMER2////////////////////////////

JUMA_Sys_Res juma_sys_res_timer2 = {
  .Current_Statue = RES_CURRENT_FREE,
  .Point          = NULL,
};

void TIMER2_IRQHandler(void)
{
  if(juma_sys_res_timer2.Point)
  {
    juma_sys_res_timer2.Point();
  }
}

uint32_t apply_sys_res_timer2(IRQHandler_Point Point)
{
  if(RES_CURRENT_BUSY == juma_sys_res_timer2.Current_Statue)
  {
    return JUMA_ERROR_CODE_SYS_RES_TIMER2_BUSY;
  }
  
  if(NULL == Point)
  {
    NRF_TIMER2->INTENCLR = 0xFFFFFFFFUL;
  }
  
  juma_sys_res_timer2.Current_Statue = RES_CURRENT_BUSY;
  juma_sys_res_timer2.Point = Point;
  
  return JUMA_ERROR_CODE_SUCCEED;
}

uint32_t free_sys_res_timer2(void)
{
  juma_sys_res_timer2.Point           = NULL;
  juma_sys_res_timer2.Current_Statue  = RES_CURRENT_FREE;
  
  return JUMA_ERROR_CODE_SUCCEED;
}
