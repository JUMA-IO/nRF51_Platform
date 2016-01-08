#include "infrared_send_pwm.h"
#include "juma_sys_res_manage.h"
#include "stdio.h"

//NRF_TIMERx   ....SHOCK
#define SHOCK_SIGNAL_TIMER__(a)                               (NRF_TIMER##a)
#define SHOCK_SIGNAL_TIMER_(a)                                SHOCK_SIGNAL_TIMER__(a)
#define SHOCK_SIGNAL_TIMER                                    SHOCK_SIGNAL_TIMER_(SHOCK_TIMER)

//NRF_TIMER0								
#define INFRARD_SIGNAL_SEND_MODULE_TIMER__(a)                 (NRF_TIMER##a)
#define INFRARD_SIGNAL_SEND_MODULE_TIMER_(a)                  INFRARD_SIGNAL_SEND_MODULE_TIMER__(a)
#define INFRARD_SIGNAL_SEND_MODULE_TIMER                      INFRARD_SIGNAL_SEND_MODULE_TIMER_(INFRARD_SIGNAL_SEND_MODULE_TIMER_CODE)

//TIMER0_IRQn			
#define INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn__(a)            (TIMER##a##_IRQn)
#define INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn_(a)             INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn__(a)
#define INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn                 INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn_(INFRARD_SIGNAL_SEND_MODULE_TIMER_CODE)

//TIMER0_IRQHandler
#define INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQHandler__(a)      (TIMER##a##_IRQHandler)
#define INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQHandler_(a)       INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQHandler__(a)
#define INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQHandler           INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQHandler_(INFRARD_SIGNAL_SEND_MODULE_TIMER_CODE)


//PPI_CHEN_CHx_Enabled
#define PPI_CHANx_ENABLE_(a)                                  (PPI_CHEN_CH##a##_Enabled)
#define PPI_CHANx_ENABLE(a)                                   PPI_CHANx_ENABLE_(a)

//PPI_CHEN_CHx_Pos
#define PPI_CHANx_POS_(a)                                     (PPI_CHEN_CH##a##_Pos)
#define PPI_CHANx_POS(a)                                      PPI_CHANx_POS_(a)

//param
extern uint8_t signal_data[];
extern uint8_t signal_data_lenth;


static uint16_t                           last_time = 0;
static ble_infrared_pwm_config_struct     config_struct;
static uint16_t                           get_next_value_index = 0;


/////////////////////////shock/////////////////////////////
static void shock_gpio_out()
{
  nrf_gpio_cfg_output(config_struct.infrared_shock_pin);  //config the pin to output mode 
}

static void shock_gpio_in()
{
  nrf_gpio_cfg_input(config_struct.infrared_shock_pin, NRF_GPIO_PIN_NOPULL);  //config the pin to input mode for save power
}


static void shock_gpiote_init()
{
  nrf_gpiote_task_configure(SHOCK_GPIOTE,									
                         config_struct.infrared_shock_pin, 
                         NRF_GPIOTE_POLARITY_TOGGLE, 
                         NRF_GPIOTE_INITIAL_VALUE_LOW);
  nrf_gpiote_task_enable(SHOCK_GPIOTE); 
}

static void shock_ppi_init()
{
  uint32_t err_code;
	
  err_code = sd_ppi_channel_assign(SHOCK_PPI_CHAN1,
                   &(SHOCK_SIGNAL_TIMER->EVENTS_COMPARE[0]),
                   &(NRF_GPIOTE->TASKS_OUT[SHOCK_GPIOTE]));
  APP_ERROR_CHECK(err_code);

  err_code = sd_ppi_channel_enable_set((PPI_CHANx_ENABLE(SHOCK_PPI_CHAN1) << PPI_CHANx_POS(SHOCK_PPI_CHAN1)));
  APP_ERROR_CHECK(err_code);
	
  err_code = sd_ppi_channel_assign(SHOCK_PPI_CHAN2,
                    &(SHOCK_SIGNAL_TIMER->EVENTS_COMPARE[1]),
                    &(NRF_GPIOTE->TASKS_OUT[SHOCK_GPIOTE]));
  APP_ERROR_CHECK(err_code);
	
  err_code = sd_ppi_channel_enable_set((PPI_CHANx_ENABLE(SHOCK_PPI_CHAN2) << PPI_CHANx_POS(SHOCK_PPI_CHAN2)));
  APP_ERROR_CHECK(err_code);
}

static void shock_timer_init(uint32_t space_value, uint32_t period_value)
{
  if(space_value >= period_value)			// if param error config the param on default value
  {
    SHOCK_SIGNAL_TIMER->CC[0] = 150;                            //duty cycle
    SHOCK_SIGNAL_TIMER->CC[1] = 214;                            //cycle
  }
  else
  {
    SHOCK_SIGNAL_TIMER->CC[0] = period_value - space_value;     //duty cycle
    SHOCK_SIGNAL_TIMER->CC[1] = period_value;                   //cycle
  }
	
  //tiemr config
  //basic config
  SHOCK_SIGNAL_TIMER->MODE = TIMER_MODE_MODE_Timer;
  SHOCK_SIGNAL_TIMER->PRESCALER = 1;													//the if 1/8 us   on one time
  SHOCK_SIGNAL_TIMER->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
	
  SHOCK_SIGNAL_TIMER->SHORTS = TIMER_SHORTS_COMPARE1_CLEAR_Enabled << TIMER_SHORTS_COMPARE1_CLEAR_Pos;
}


static void shock_signal_init(uint32_t plus_value, uint32_t period_value)
{
  shock_gpio_in();
  shock_gpiote_init();
  shock_ppi_init();
  shock_timer_init(plus_value, period_value);
}

static void shock_signal_start()
{
  shock_gpio_out();
  shock_gpiote_init();
  SHOCK_SIGNAL_TIMER->TASKS_START = 1;
}

static void shock_signal_stop()
{
  SHOCK_SIGNAL_TIMER->TASKS_STOP = 1;
  shock_gpiote_init();
  shock_gpio_in();			
}


////////////////////shock_end//////////

static uint32_t ble_infrared_pwm_stop(void);

static void gpiote_init()
{
  nrf_gpiote_task_configure(GPIOTE_CHAN_OUTPUT,									
                        config_struct.infrared_signal_pin, 
                        NRF_GPIOTE_POLARITY_TOGGLE, 
                        NRF_GPIOTE_INITIAL_VALUE_LOW);
  nrf_gpiote_task_enable(GPIOTE_CHAN_OUTPUT);
}

static void ppi_init()
{
  uint32_t err_code;

  err_code = sd_ppi_channel_assign(INFRARD_SIGNAL_PPI_CHAN_0,
                    &(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[0]),
                    &(NRF_GPIOTE->TASKS_OUT[GPIOTE_CHAN_OUTPUT]));
  APP_ERROR_CHECK(err_code);
	
  err_code = sd_ppi_channel_assign(INFRARD_SIGNAL_PPI_CHAN_1,
                    &(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[1]),
                    &(NRF_GPIOTE->TASKS_OUT[GPIOTE_CHAN_OUTPUT]));
  APP_ERROR_CHECK(err_code);
	
  err_code = sd_ppi_channel_assign(INFRARD_SIGNAL_PPI_CHAN_2,
                    &(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[2]),
                    &(NRF_GPIOTE->TASKS_OUT[GPIOTE_CHAN_OUTPUT]));
  APP_ERROR_CHECK(err_code);
	
  err_code = sd_ppi_channel_assign(INFRARD_SIGNAL_PPI_CHAN_3,
                    &(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[3]),
                    &(NRF_GPIOTE->TASKS_OUT[GPIOTE_CHAN_OUTPUT]));
  APP_ERROR_CHECK(err_code);
	
	
  err_code = sd_ppi_channel_assign(INFRARD_SIGNAL_PPI_CHAN_COUNT,
                    &(SHOCK_SIGNAL_TIMER->EVENTS_COMPARE[1]),
                    &(INFRARD_SIGNAL_SEND_MODULE_TIMER->TASKS_COUNT));
  APP_ERROR_CHECK(err_code);
	
  // Enable PPI channel 2
  err_code = sd_ppi_channel_enable_set((PPI_CHANx_ENABLE(INFRARD_SIGNAL_PPI_CHAN_0) << PPI_CHANx_POS(INFRARD_SIGNAL_PPI_CHAN_0)));
  APP_ERROR_CHECK(err_code);
  err_code = sd_ppi_channel_enable_set((PPI_CHANx_ENABLE(INFRARD_SIGNAL_PPI_CHAN_1) << PPI_CHANx_POS(INFRARD_SIGNAL_PPI_CHAN_1)));
  APP_ERROR_CHECK(err_code);
  err_code = sd_ppi_channel_enable_set((PPI_CHANx_ENABLE(INFRARD_SIGNAL_PPI_CHAN_2) << PPI_CHANx_POS(INFRARD_SIGNAL_PPI_CHAN_2)));
  APP_ERROR_CHECK(err_code);
  err_code = sd_ppi_channel_enable_set((PPI_CHANx_ENABLE(INFRARD_SIGNAL_PPI_CHAN_3) << PPI_CHANx_POS(INFRARD_SIGNAL_PPI_CHAN_3)));
  APP_ERROR_CHECK(err_code);
  
  err_code = sd_ppi_channel_enable_set((PPI_CHANx_ENABLE(INFRARD_SIGNAL_PPI_CHAN_COUNT) << PPI_CHANx_POS(INFRARD_SIGNAL_PPI_CHAN_COUNT)));
  APP_ERROR_CHECK(err_code);
}

static void timer_init()
{
  //tiemr config
  //basic config
  INFRARD_SIGNAL_SEND_MODULE_TIMER->MODE = TIMER_MODE_MODE_Counter;
  INFRARD_SIGNAL_SEND_MODULE_TIMER->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
  
  
  //interrupt config
  INFRARD_SIGNAL_SEND_MODULE_TIMER->INTENCLR = 0xffffffffUL;
  INFRARD_SIGNAL_SEND_MODULE_TIMER->INTENSET =  (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos ) |
                                                (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos ) |
                                                (TIMER_INTENSET_COMPARE2_Enabled << TIMER_INTENSET_COMPARE2_Pos ) |
                                                (TIMER_INTENSET_COMPARE3_Enabled << TIMER_INTENSET_COMPARE3_Pos );
  
  NVIC_ClearPendingIRQ(INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn);
  NVIC_SetPriority(INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn, 10);
  NVIC_EnableIRQ(INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQn);
}

static void gpio_start()
{
  nrf_gpio_cfg_output(config_struct.infrared_signal_pin);
}

static void gpio_stop()
{
  nrf_gpio_cfg_input(config_struct.infrared_signal_pin, NRF_GPIO_PIN_NOPULL);				//config the pin on input mode to save the power
}

static void ble_infrard_send_module_init(void)
{
  gpio_stop();
  gpiote_init();
  ppi_init();
  timer_init();
}

static __INLINE uint16_t ble_infrared_get_next_toggle_time()
{
  uint8_t index = 0;
  uint8_t polarity;
	
  if(get_next_value_index/2 >= signal_data_lenth )
  {
    get_next_value_index = 0;
    return 0;
  }
		
  index = get_next_value_index / 2;
  polarity = get_next_value_index % 2;
	
  get_next_value_index ++;
	
  if((signal_data[index/8]) & (0x80 >> (index%8)))
  {//logic 1
    if(!polarity)
    {//burst
      return config_struct.logic_1_burst;
    }
    else
    {//space
      return config_struct.logic_1_space;
    }
  }
  else 
  {//logic 0
    if(!polarity)
    {//burst
      return config_struct.logic_0_burst;
    }
    else 
    {//space
      return config_struct.logic_0_space;
    }
  }
}


static __INLINE int32_t ble_infrared_config_timer_chan(uint8_t chan)
{
  uint16_t current_value = 0;
  static uint8_t stop_send = 5;				//5: normal mode   if there is 0~3 mean the send will stop on the number chan

  if(stop_send == chan)								//send is over need to stop
  {
    ble_infrared_pwm_stop();
    stop_send = 5;
    return 1;
  }
	
  if(stop_send != 5)									//there will to stop no more value need to fill
  {
    return -1;
  }
	
  current_value = ble_infrared_get_next_toggle_time();
	
  if(current_value != 0)		//fill the value on chan
  {
    current_value += last_time;
    INFRARD_SIGNAL_SEND_MODULE_TIMER->CC[chan] = current_value;							//fill the value in chan
    last_time = current_value;																							//save current timer as the start point of next fill
  }
  else
  {	//there is no more value need to fill the stop code
    INFRARD_SIGNAL_SEND_MODULE_TIMER->SHORTS |= 1UL << (TIMER_SHORTS_COMPARE0_STOP_Pos + chan);				//config stop time point
    stop_send = chan;				//remember the stop chan
		
    current_value = last_time + config_struct.end_burst;
    INFRARD_SIGNAL_SEND_MODULE_TIMER->CC[chan] = current_value;							//fill the chan
  }
  return 0;
}


//start to sending
static uint32_t ble_infrared_pwm_start()
{
  INFRARD_SIGNAL_SEND_MODULE_TIMER->SHORTS = 0;						//clear all the config on timer
  
  //config the chan
  last_time = 100;
  INFRARD_SIGNAL_SEND_MODULE_TIMER->CC[0] = last_time;
  last_time += config_struct.guidance_burst;
  INFRARD_SIGNAL_SEND_MODULE_TIMER->CC[1] = last_time;		//fill guidance_burst
  last_time += config_struct.guidance_space;
  INFRARD_SIGNAL_SEND_MODULE_TIMER->CC[2] = last_time;		//fill guidance space
  ble_infrared_config_timer_chan(3);																		//fill a value on chan 3
  
  //start shack
  shock_signal_start();		
  
  //start sending timer
  INFRARD_SIGNAL_SEND_MODULE_TIMER->TASKS_CLEAR = 1;
  INFRARD_SIGNAL_SEND_MODULE_TIMER->TASKS_START = 1;
  
  return 0;
}

uint32_t ble_infrared_stop(void);

//stop sending
static uint32_t ble_infrared_pwm_stop()
{
  //stop shock sending
  shock_signal_stop();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
  //stop infrared signal pin 
  gpio_stop();
  
  //call the ble_infrared_stop to change the status
  ble_infrared_stop();
  
  return 0;
}

static void infrared_signal_send_module_timer_irqhandler(void);
void ble_infrared_set_interrupt_function_point(IRQHandler_Point Point);
uint32_t ble_infrared_pwm_config(ble_infrared_pwm_config_struct init_struct)
{
  //save struct data
  config_struct = init_struct;
  
  //config interrupt function point
  ble_infrared_set_interrupt_function_point(infrared_signal_send_module_timer_irqhandler);
  
  //config shock mode
  shock_signal_init(init_struct.carrier_plus, init_struct.carrier_period);
  
  //config infrared signal send mode
  ble_infrard_send_module_init();
  
  return 0;
}

uint32_t ble_infrared_pwm_send(const uint8_t data[], uint8_t length)
{
  //save the data will to sending
  for(int i=0; i<=length/8; i++)
  {
    signal_data[i] = data[i];
  }
  
  signal_data_lenth = length;
  
  //clear the index of send data
  get_next_value_index = 0;
  
  //start GPIO
  gpio_start();
  
  //start sending
  ble_infrared_pwm_start();
  
  return 0;
}


//interrupt for infrared signal sending
//void INFRARD_SIGNAL_SEND_MODULE_TIMER_IRQHandler ()
static void infrared_signal_send_module_timer_irqhandler(void)
{	
  if(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[0] == 1)
  {
    INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[0] = 0;
    
    ble_infrared_config_timer_chan(0);				//fill the chan 0
  }
  
  if(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[1] == 1)
  {
    INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[1] = 0;
    
    ble_infrared_config_timer_chan(1);				//fill the chan 1
  }
  
  if(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[2] == 1)
  {
    INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[2] = 0;
    
    ble_infrared_config_timer_chan(2);				//fill the chan 2
  }
  
  if(INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[3] == 1)
  {
    INFRARD_SIGNAL_SEND_MODULE_TIMER->EVENTS_COMPARE[3] = 0;
    
    ble_infrared_config_timer_chan(3);				//fill the chan 3
  }
}


