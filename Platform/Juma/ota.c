#include "sys_common.h"
#include "pstorage.h"
#include "simple_uart.h"
#include "device_statue.h"

#define OTA_FLASH_OPTION_TIME 15    

#define OTA_STATUE_NO_OTA     0x00
#define OTA_STATUE_INIT       0x01
#define OTA_STATUE_BEGIN      0x02
#define OTA_STATUE_CLEAR      0x03
#define OTA_STATUE_PROGRAM    0x04
#define OTA_STATUE_END        0x05

static pstorage_handle_t h_storage;
uint8_t ota_statue_step = 0; // 0 : first option 1: second option
uint8_t ota_current_statue = 0;

uint8_t ota_statue = OTA_STATUE_NO_OTA; 
uint16_t ota_data_length = 0;
uint8_t ota_data[256];
uint32_t start_byte;
uint8_t * start_point = (uint8_t *)&start_byte;

uint32_t ota_check_count = 0xFFFFFFFFUL;
uint32_t ota_file_count = 0xFFFFFFFFUL;

enum {
    OTA_BEGIN = 0x00,
    OTA_END   = 0x01,
    OTA_DATA  = 0x02,
    OTA_ERROR = 0x03,
};

enum {
    OTA_APP,
};

uint32_t pstorage_raw_register(pstorage_module_param_t * p_module_param,
                               pstorage_handle_t       * p_block_id);
uint32_t pstorage_raw_store(pstorage_handle_t * p_dest,
                            uint8_t           * p_src,
                            pstorage_size_t     size,
                            pstorage_size_t     offset);
uint32_t pstorage_raw_clear(pstorage_handle_t * p_dest, pstorage_size_t size);

static void ota_storage_callback_handler(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len);
static void ota_sys_reset(void * args)
{
  NVIC_SystemReset();
}


void ota_test(void* args)
{
    uint32_t                err_code;
    uint8_t data[4];
    data[0] = 0x5A;
    err_code = pstorage_raw_store(&h_storage, data, 4, 0);
    APP_ERROR_CHECK(err_code);
}

void ota_init(void)
{   
  ota_statue = OTA_STATUE_INIT;
  flash_set_function_callback_point(ota_storage_callback_handler);
    //run_when_idle(ota_test, NULL);
}

uint32_t ota_OE32_check_count_caculate(uint8_t * data, uint32_t len)
{
  uint32_t count=0;
  uint32_t value=0;
  uint32_t i=0;
  uint32_t j=0;
  uint8_t ch;
  
  for(i=0; i<len; i++)
  {
    ch = data[i];
    
    value <<= 8*j;
    value |= ch;
    
    j++;
    if(j >= 4)
    {
      j = 0;
      
      count += value;
      value = 0;
    }
  }
  
  return count;
}

void ota_uart_send(uint8_t * data, uint16_t len)
{
  uint8_t i;
  for(i=0; i<len; i++)
  {
    simple_uart_put(data[i]);
  }
}

const uint8_t ota_check_count_test[] = {0x55, 0x00, 0x33, 0x44};

void ota_on_message(uint8_t type, uint16_t length, uint8_t* data)
{
  uint8_t opcode = data[0];

#ifdef UART_DEBUT_OUTPUT

#endif
  
  
  if(ota_statue == OTA_STATUE_NO_OTA)
  {//no ota mode

  }
  else
  {//ota mode
    switch (opcode) 
    {
      case OTA_BEGIN:
      {
        if(ota_statue == OTA_STATUE_INIT)
        {
          app = NULL;
          ota_statue_step = 0;
          ota_statue = OTA_STATUE_BEGIN;
          flash_clear(APP_ZONE_BASE, APP_ZONE_SIZE);
        }
      }
      break;
      case OTA_END:
      {
        if(ota_statue == OTA_STATUE_PROGRAM)
        {
          ota_statue = OTA_STATUE_END;
          ota_statue_step = 0;
          
          uint32_t ota_check_value = ota_OE32_check_count_caculate((uint8_t *)APP_DESCRIPTOR_ADDR+4, ota_file_count-4);
          ota_check_value += ota_OE32_check_count_caculate((uint8_t *)start_point, 4);
          
          if((ota_check_count != 0xFFFFFFFFUL) && (ota_file_count != 0xFFFFFFFFUL))
          if(ota_check_value == ota_check_count)
          {
            flash_program(APP_DESCRIPTOR_ADDR, start_point, 4, 0);
            return;
          }

          run_after_delay(ota_sys_reset, NULL, 50);
        }
      }
      break;
      case OTA_DATA:
      {
        if(ota_statue == OTA_STATUE_PROGRAM)
        {
          app = NULL;
          ota_statue_step = 0;
          
          if(0 == data[1])
          {
            ota_check_count = *(uint32_t *)(&data[2+0]);
            ota_file_count = *(uint32_t *)(&data[2+4]);
            
            uint8_t value[1];
            value[0] = OTA_DATA;
            ble_device_send(0x81, 1, value);
          }
          else
          {
            data[1] --;
            
            if(0 == data[1])
            {
              for(uint8_t i=0; i<4; i++)
              {
                start_point[i] = data[2 + i];
                data[2 + i]    = 0xFF;
              }
            }
            
            ota_data_length = length;
            memcpy(ota_data, data, 256);
            
            flash_program(APP_DESCRIPTOR_ADDR, data + 2, length - 2, (data[1]) * 196);
          }
        }
      }
      break;
    }
  }
}


static void ota_storage_clear_callback_handler()
{
  uint8_t data[1];
  if(ota_statue == OTA_STATUE_BEGIN)
  {
    //check clear area
    uint8_t clear_check = 1;
    for(uint32_t addr = APP_ZONE_BASE; addr < APP_ZONE_BASE + APP_ZONE_SIZE; addr+=4)
    {
      if((*(uint32_t *)addr) != 0xFFFFFFFFUL)
      {
        clear_check = 0;
        break;
      }
    }
    
    if(clear_check)
    {//clear success
      ota_statue = OTA_STATUE_PROGRAM;
      data[0] = OTA_BEGIN;
      ble_device_send(0x81, 1, data);
    }
    else
    {//clear fail
      ota_statue_step ++;
      if(ota_statue_step >= OTA_FLASH_OPTION_TIME)
      {
        data[0] = OTA_ERROR;
        ble_device_send(0x81, 1, data);
      }
      else
      {
        flash_clear(APP_ZONE_BASE, APP_ZONE_SIZE);
      }
    }
  }
  else if(ota_statue == OTA_STATUE_END)
  {
    run_after_delay(ota_sys_reset, NULL, 50);
  }
}

static void ota_storage_program_callback_handler()
{
  uint8_t data[1];
  if(ota_statue == OTA_STATUE_PROGRAM)
  { 
    //check write
    //if(1)
    if(!memcmp((void *)((uint32_t)APP_DESCRIPTOR_ADDR + ota_data[1]*196), ota_data+2, ota_data_length-2))
    {
      data[0] = OTA_DATA;
      ble_device_send(0x81, 1, data);
    }
    else
    {
      ota_statue_step ++;
      if(ota_statue_step >= OTA_FLASH_OPTION_TIME)
      {
        data[0] = OTA_ERROR;
        ble_device_send(0x81, 1, data);
      }
      else 
      {
        flash_program(APP_DESCRIPTOR_ADDR, ota_data + 2, ota_data_length - 2, ota_data[1] * 196);
      }
    }
  }
  else if(ota_statue == OTA_STATUE_END)
  {
    //check start address value
    if((*(uint32_t *)APP_DESCRIPTOR_ADDR) == start_byte)
    {//success
      ble_device_disconnect();
      run_after_delay(ota_sys_reset, NULL, 50);
    }
    else
    {//fail
      ble_device_disconnect();
      flash_clear(APP_ZONE_BASE, APP_ZONE_SIZE);
    }
  }
  else
  {
    ble_device_disconnect();
    run_after_delay(ota_sys_reset, NULL, 50);
  }  
}

static void ota_storage_callback_handler(pstorage_handle_t * p_handle,
                                    uint8_t             op_code,
                                    uint32_t            result,
                                    uint8_t           * p_data,
                                    uint32_t            data_len)
{
  switch (op_code)
  {
    case PSTORAGE_STORE_OP_CODE:  ota_storage_program_callback_handler();   break;
    case PSTORAGE_CLEAR_OP_CODE:  ota_storage_clear_callback_handler();     break;
    default:break;
  }
  APP_ERROR_CHECK(result);
}

/*-------------------------ota entry point----------------------------------------------------*/
uint8_t enter_ota_mode_statue = 0;
static void enter_ota_mode_callback(pstorage_handle_t * p_handle,
                                      uint8_t             op_code,
                                      uint32_t            result,
                                      uint8_t           * p_data,
                                      uint32_t            data_len)
{
  if(enter_ota_mode_statue)
  {
    if(op_code == PSTORAGE_CLEAR_OP_CODE)
    {
      //check clear 
    }
    NVIC_SystemReset();
  }
}

void enter_ota_mode(uint8_t type, uint16_t length, uint8_t* data)
{  
	if(device_current_statue_get() == DEVICE_STATUE_APP)
  if(!(strcmp((char *)data, "OTA_MODE")))
  {
    enter_ota_mode_statue = 1;
  }
}

void enter_ota_process(void * args)
{
  flash_set_function_callback_point(enter_ota_mode_callback);
  flash_clear(APP_ZONE_BASE, APP_ZONE_SIZE);
}

/*-----------------------------------------------------------------------------------*/
