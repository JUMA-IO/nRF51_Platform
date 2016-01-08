#ifndef _JUMA_SYS_RES_MANAGE_H_
#define _JUMA_SYS_RES_MANAGE_H_

#include "nrf51.h"

typedef void (* IRQHandler_Point)(void);
typedef struct
{
  IRQHandler_Point    Point;   //function enter point
  uint8_t             Current_Statue;  //Res current statue
}JUMA_Sys_Res;


//timer1
extern uint32_t apply_sys_res_timer1(IRQHandler_Point Point);
extern uint32_t free_sys_res_timer1(void);

//timer2
extern uint32_t apply_sys_res_timer2(IRQHandler_Point Point);
extern uint32_t free_sys_res_timer2(void);

#endif  //_JUMA_SYS_RES_MANAGE_H_
