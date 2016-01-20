/*
  Copyright 2014-2015 juma.io

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef _PSM_TYPES_H_
#define _PSM_TYPES_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "nrf51_bitfields.h"

typedef void (*function_t)(void* args);

#define SERIAL_FRAME_LENGTH 32

#define TICKS_1_SECOND 1024
#define TICKS_1_MINUTE (TICKS_1_SECOND * 60)

enum 
{
  GPIO_OUTPUT = 1,
  GPIO_INPUT_NOPULL,
  GPIO_INPUT_PULLUP,
  GPIO_INPUT_PULLDOWN,
};

enum 
{
  GPIO_RISING = 1,
  GPIO_FALLING,
};

enum 
{
  GPIO_WATCH_LEVEL_LOW    = 0,
  GPIO_WATCH_LEVEL_HIGHT  = 1,
};

/* UART baudrate. */
#define UART_BAUDRATE_Baud1200    (0x0004F000UL) /*!< 1200 baud. */
#define UART_BAUDRATE_Baud2400    (0x0009D000UL) /*!< 2400 baud. */
#define UART_BAUDRATE_Baud4800    (0x0013B000UL) /*!< 4800 baud. */
#define UART_BAUDRATE_Baud9600    (0x00275000UL) /*!< 9600 baud. */
#define UART_BAUDRATE_Baud14400   (0x003B0000UL) /*!< 14400 baud. */
#define UART_BAUDRATE_Baud19200   (0x004EA000UL) /*!< 19200 baud. */
#define UART_BAUDRATE_Baud28800   (0x0075F000UL) /*!< 28800 baud. */
#define UART_BAUDRATE_Baud38400   (0x009D5000UL) /*!< 38400 baud. */
#define UART_BAUDRATE_Baud57600   (0x00EBF000UL) /*!< 57600 baud. */
#define UART_BAUDRATE_Baud76800   (0x013A9000UL) /*!< 76800 baud. */
#define UART_BAUDRATE_Baud115200  (0x01D7E000UL) /*!< 115200 baud. */
#define UART_BAUDRATE_Baud230400  (0x03AFB000UL) /*!< 230400 baud. */
#define UART_BAUDRATE_Baud250000  (0x04000000UL) /*!< 250000 baud. */
#define UART_BAUDRATE_Baud460800  (0x075F7000UL) /*!< 460800 baud. */
#define UART_BAUDRATE_Baud921600  (0x0EBED000UL) /*!< 921600 baud. */
#define UART_BAUDRATE_Baud1M      (0x10000000UL) /*!< 1M baud. */


enum {
    UART_8N1 = 1,
};

enum {
    BLE_DEVICE_NORMAL,
    BLE_DEVICE_LINK_LOST,
};

// Per Pixel Operations
enum {
    GFX_SET_BLACK,
    GFX_SET_WHITE,
    GFX_INVERT,
};

//Timer TYPE
enum{
  TIMER_ONECE,
  TIMER_PERIODIC,
};

#define GFX_BLACK_IS_1   0x01

typedef struct _uart_config_t {
    uint32_t speed;
    uint8_t  mode;
    uint8_t  flow_control;
    uint8_t  has_irq_pin;
    uint8_t  rx_pin, tx_pin, rts_pin, cts_pin, irq_pin;
} uart_config_t;

typedef struct _light_config_t {
    uint8_t pins[4]; // pins for RGBW
} light_config_t;

typedef struct _adc_result_t {
    uint16_t value;
} adc_result_t;

typedef struct {
	const uint8_t *font_table;
    uint8_t space;
	uint8_t width;
	uint8_t height;
	char start_char;
	char end_char;
} gfx_font_t;


//////////////////ble infrared////////////////////////////////
typedef struct ble_infrared_pwm_config_struct_
{
	uint8_t infrared_signal_pin;          //pin num for infrared signal pin
	uint8_t infrared_shock_pin;           //pin num for shock signal pin
	
	//carrier
	uint16_t carrier_period;              //the time value of carrier period
	uint16_t carrier_plus;	              //the time value of carrier hight time
                
	//guidance              
	uint16_t guidance_burst;              //the carrier period number of guidance burst
	uint16_t guidance_space;              //the carrier period number of guidance space
                
	//logic 0             
	uint16_t logic_0_burst;	              //the carrier period number of logic 0 sending burst
	uint16_t logic_0_space;	              //the carrier period number of logic 0 sending space
                
	//logic 1             
	uint16_t logic_1_burst;	              //the carrier period number of logic 1 sending burst
	uint16_t logic_1_space;	              //the carrier period number of logic 1 sending space
	
	//end
	uint16_t end_burst;                   //the carrier period number of end gap burst 
	
}ble_infrared_pwm_config_struct;

typedef enum
{
  PWM_HEAD_END = 0			
}infraredProtocal;

typedef struct ble_infrared_init_struct_
{
  infraredProtocal Protocal_type;
	
  union {
    ble_infrared_pwm_config_struct pwm;
  }type_data;
	
}ble_infrared_init_struct;

///////////////////////////SPI/////////////////////////////////////////////
typedef struct
{
	uint32_t MISO;			//miso pin
	uint32_t MOSI;			//mosi pin
	uint32_t SCK;				//sck	pin
	uint32_t FREQ;			//bus speed
	
	uint32_t ORDER;			//order msb lsb
	uint32_t CPOL;			//cpol
	uint32_t CPHA;			//cpha
}spi_init_struct_t;

#define SPI_ORDER_MSB		(SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos)
#define SPI_ORDER_LSB		(SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos)

#define SPI_CPOL_HIGH		(SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos)
#define SPI_CPOL_LOW		(SPI_CONFIG_CPOL_ActiveLow  << SPI_CONFIG_CPOL_Pos)

#define SPI_CPHA_1EDGE	(SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos)
#define SPI_CPHA_2EDGE	(SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos)

////////////////////////////WatchDog////////////////////////////////////////
#define JUMA_WDT_EN_USER_DOG1   0x01
#define JUMA_WDT_EN_USER_DOG2   0x02
#define JUMA_WDT_EN_USER_DOG3   0x04
#define JUMA_WDT_EN_USER_DOG4   0x08
              
#define JUMA_WDT_EN_SYS_SDK_DOG 0x80

#define JUMA_WDT_USER_DOG1      0x01
#define JUMA_WDT_USER_DOG2      0x02  
#define JUMA_WDT_USER_DOG3      0x04
#define JUMA_WDT_USER_DOG4      0x08

#endif //_PSM_TYPES_H_
