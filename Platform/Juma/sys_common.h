#ifndef _SYS_COMMON_H_
#define _SYS_COMMON_H_

#include <stdint.h>
#include <string.h>
#include "juma_error_code.h"
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf51_bitfields.h"
#include "nrf_soc.h"
#include "dispatch.h"
#include "pwm.h"
#include "juma_common.h"
#include "app_util_platform.h"
#include "pstorage.h"
#include "juma_sys_res_manage.h"

#define SYSTEM_MESSAGE_CHANNEL 0
#define CLOUD_MESSAGE_CHANNEL 1
#define APPLICATION_MESSAGE_CHANNEL 2

enum { 
    SYSTEM_ERROR,
    SYSTEM_GET_VERSION_INFOMATION,
    SYSTEM_GET_DEVICE_TYPE,
    SYSTEM_GET_FEATURE_BITS,
    SYSTEM_GET_GLOBAL_ID,
    SYSTEM_LOOPBACK,
    SYSTEM_ALERT_CONTROL,
    SYSTEM_ALERT,
    SYSTEM_WRITE_DISPLAY_BUFFER,
    SYSTEM_BUTTON_CONTROL,
    SYSTEM_BUTTON_EVENT,
    SYSTEM_READ_BATTERY_LEVEL,
    SYSTEM_LIGHT_SENSOR_CONTROL,
    SYSTEM_LIGHT_SENSOR_EVENT,
    SYSTEM_ACCELERATOR_CONTROL,
    SYSTEM_ACCELERATOR_EVENT,
    SYSTEM_SET_BEACON_URL,
    SYSTEM_GET_BEACON_URL,
    SYSTEM_SWITCH_CONTROL,
    SYSTEM_LIGHT_CONTROL,
    SYSTEM_READ_CURRENT_TEMPERATURE,
};

// SERIAL MESSAGE
enum {
    SERIAL_GET_STATUS,
    SERIAL_GET_DEVICE_ID,
    SERIAL_GET_DEFAULT_KEY,
    SERIAL_SET_WORKING_KEY,
};

// SYSTEM STATUS

#define SYSTEM_STATUS_CONNECTED   0x01
#define SYSTEM_STATUS_LOCKED      0x02

// APP ZONE

#define APP_ZONE_BASE  0x20000  //128k
#define APP_ZONE_SIZE  0x8000   //32k
#define APP_DESCRIPTOR_ADDR (APP_ZONE_BASE + 0x100)  //0x20100 相对于起始地址偏移256个字节,什么意思?

//flash operation callback function point
typedef void (* flash_callback_function) (pstorage_handle_t * p_handle,uint8_t op_code,uint32_t result, uint8_t * p_data,uint32_t data_len) ;


extern app_descriptor_t* app;
extern uint8_t system_status;
extern uint8_t m_has_irq_pin;
extern uint8_t m_irq_pin;
extern uint8_t device_id[16];
extern uint8_t unlock_key[16];

void load_app(void);
void set_system_status( uint8_t new_status );
void system_on_message(uint8_t type, uint16_t length, uint8_t* data);
void gpio_init( void );
void ota_init(void);
void data_storage_init(void);

//flash operation
void 			flash_init(void);
uint32_t flash_clear(uint32_t addr, uint32_t size);
uint32_t flash_program(uint32_t basic_addr, uint8_t * data, uint32_t len, uint32_t offset);
uint32_t flash_read(uint32_t addr, uint8_t * data, uint32_t len);
uint32_t flash_set_function_callback_point(flash_callback_function flash_callback);


//random
uint8_t random_generate(void);

#endif
