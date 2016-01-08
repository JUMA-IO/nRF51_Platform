#include "sys_common.h"
#include "juma_sys_res_manage.h"

typedef struct _light_t {
    uint8_t on;
    uint8_t pins[3];
    uint8_t values[3];
    uint8_t is_active_high:1;
} light_t;

static light_t m_light;

void light_timer_interrupt(void);


void set_color(void* args)
{
  light_t* light = (light_t*) args;
  
  uint8_t i;
 
  PWM_TIMER->EVENTS_COMPARE[0] =
  PWM_TIMER->EVENTS_COMPARE[1] =
  PWM_TIMER->EVENTS_COMPARE[2] =
  PWM_TIMER->EVENTS_COMPARE[3] = 0;

  for(i = 0; i < 3; i++)
  {
      nrf_gpiote_task_disable(i);
      nrf_gpio_cfg_output(light->pins[i]);
      
      if(light->values[i] == 0x00)
      {
        nrf_gpio_pin_write(m_light.pins[i], (m_light.is_active_high)?0:1);
      }
      else if(light->values[i] == 0xFF)
      {
        nrf_gpio_pin_write(m_light.pins[i], (m_light.is_active_high)?1:0);
      }
      else
      {
        PWM_TIMER->CC[i] = light->values[i];

        nrf_gpio_cfg_output(light->pins[i]);
        
        if(light->is_active_high)
        {
          nrf_gpio_pin_set(light->pins[i]);
          nrf_gpiote_task_configure(i, light->pins[i], NRF_GPIOTE_POLARITY_HITOLO, NRF_GPIOTE_INITIAL_VALUE_HIGH);
        }
        else
        {
          nrf_gpio_pin_clear(light->pins[i]);
          nrf_gpiote_task_configure(i, light->pins[i], NRF_GPIOTE_POLARITY_LOTOHI, NRF_GPIOTE_INITIAL_VALUE_LOW);
        }
        nrf_gpiote_task_enable(i); 
        
        sd_ppi_channel_assign(i, &PWM_TIMER->EVENTS_COMPARE[i], &NRF_GPIOTE->TASKS_OUT[i]);
        sd_ppi_channel_enable_set(1 << (i));
      }        
  }
  
}

void light_setup(uint8_t* pins, uint8_t is_active_high)
{
    memcpy(m_light.pins, pins, 3);
    m_light.is_active_high = is_active_high;
    m_light.on = 0;
}


uint32_t light_on( void )
{ 
  uint32_t err_code;
  
  if((err_code = apply_sys_res_timer2(light_timer_interrupt)) != 0)
  {
    return err_code;
  }
  
  m_light.on = 1;
  
  PWM_TIMER->POWER = 1;
  
  PWM_TIMER->PRESCALER = 7; // 256k
  PWM_TIMER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
  PWM_TIMER->MODE = TIMER_MODE_MODE_Timer;
  
  PWM_TIMER->CC[3] = 255;

  //interrupt
  
  sd_nvic_SetPriority(PWM_IRQn, 3);
  sd_nvic_EnableIRQ(PWM_IRQn);

  PWM_TIMER->INTENSET = TIMER_INTENSET_COMPARE3_Msk;  
  
  set_color((void*)&m_light);
  
  PWM_TIMER->TASKS_START = 1;
  
  return JUMA_ERROR_CODE_SUCCEED;
}

void light_off( void )
{
  int i;
  
  m_light.on = 0;

  PWM_TIMER->TASKS_CLEAR = 1;
  PWM_TIMER->TASKS_STOP = 1;
  PWM_TIMER->POWER = 0;
  
  for(i = 0; i < 3; i++)
  {
    nrf_gpiote_task_disable(i);
    nrf_gpio_cfg_output(m_light.pins[i]);
    nrf_gpio_pin_write(m_light.pins[i], (m_light.is_active_high)?0:1);
  }
  
  free_sys_res_timer2();
}

void light_set_color(const uint8_t* rgb_values)
{ 
  memcpy(m_light.values, rgb_values, 3);
  
  if(m_light.on == 1)
  {
    PWM_TIMER->TASKS_STOP = 1;
    PWM_TIMER->TASKS_CLEAR = 1;

    set_color((void*)&m_light);

    PWM_TIMER->TASKS_START = 1;
  }
}

void light_timer_interrupt(void)
{
  if(PWM_TIMER->EVENTS_COMPARE[3])
  {
    PWM_TIMER->EVENTS_COMPARE[3] = 0;
    
    if(m_light.on)
    {
      PWM_TIMER->TASKS_STOP = 1;
      PWM_TIMER->TASKS_CLEAR = 1;
      
      set_color((void*)&m_light);
      
      PWM_TIMER->TASKS_START = 1;
    }
  }
}


