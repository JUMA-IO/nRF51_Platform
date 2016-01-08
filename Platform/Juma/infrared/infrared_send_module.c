#include "infrared_send_module.h"
#include "sys_common.h"

//param
uint8_t signal_data[MAX_SIGNAL_DATA_LENGTH];
uint8_t signal_data_lenth =0;
static uint8_t current_statue = 0;        //status  0 is uninitialize
static infraredProtocal Protocal_type;
static ble_infrared_init_struct infrared_init_struct;
static IRQHandler_Point ble_infrared_interrupt_function_point = NULL;

static void ble_infrared_interrupt_handler(void)
{
  if(ble_infrared_interrupt_function_point)
  {
    ble_infrared_interrupt_function_point();
  }
}

void ble_infrared_set_interrupt_function_point(IRQHandler_Point Point)
{
  ble_infrared_interrupt_function_point = Point;
}

static void pin10_sigal()
{
  nrf_gpio_cfg_output(10);
  nrf_gpio_pin_set(10);
  __nop();
  __nop();
  __nop();
  __nop();
  nrf_gpio_pin_clear(10);
}

//for init the infrared module
uint32_t ble_infrared_config(ble_infrared_init_struct * Init_struct)
{
  if(current_statue > 1)
  {
    return 1;           //on the sended status
  }
  
  infrared_init_struct = *Init_struct ;

  current_statue = 1;	                    //go the ready mode
	
  return 0;
}

static uint32_t ble_infrared_apply_sys_res()
{
  uint32_t err_code;

  //apply timer resource
  if((err_code = apply_sys_res_timer1(ble_infrared_interrupt_handler)) != JUMA_ERROR_CODE_SUCCEED)
  {
    return err_code;
  }
  
    
  
  //apply timer resource
  if((err_code = apply_sys_res_timer2(NULL)) != JUMA_ERROR_CODE_SUCCEED)
  {
    free_sys_res_timer1();
    
    return err_code;
  }
  

  
  return JUMA_ERROR_CODE_SUCCEED;
}

//for send
uint32_t ble_infrared_send(const uint8_t * signal_data, uint8_t lenth)
{
    
  uint32_t err_code;
  //now is send mode
  if(current_statue > 1)
  {
    return 1;									
  }

  //now on the send mode
  if(current_statue == 0)		
  {
    return 2;
  }
	
  //apply sys res
  if((err_code = ble_infrared_apply_sys_res()) != JUMA_ERROR_CODE_SUCCEED)
  {
    return err_code;
  }
  
  ///config send 
  if(PWM_HEAD_END == infrared_init_struct.Protocal_type)       //for normal pwm mode				
  {
    ble_infrared_pwm_config(infrared_init_struct.type_data.pwm);
  }
  
  //enter sending statue
  current_statue = 2;					//go the send mode
	
  //sending ....
  if(Protocal_type == PWM_HEAD_END)					//call the protocal pwm to send				
  {
    ble_infrared_pwm_send(signal_data, lenth);
  }
  
  return 0;
}

//get the statue
uint32_t ble_infrared_get_statue()
{
  return current_statue;
}

//for change the statue on stop sending
uint32_t ble_infrared_stop()
{
  //free all sys res
  free_sys_res_timer1();
  free_sys_res_timer2();
  
  current_statue = 1;				//for change the statue on stop send
	
  return 0;
}

