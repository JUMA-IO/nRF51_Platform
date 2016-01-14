#ifndef _LIS2DH12_NRF51822_H_
#define _LIS2DH12_NRF51822_H_

#include "nrf51.h"
#include "nrf51_bitfields.h"

//config pin
#define LIS2DH12_VDD         30
#define LIS2DH12_MISO        1
#define LIS2DH12_MOSI        0
#define LIS2DH12_CSN         3
#define LIS2DH12_SCK         2
#define LIS2DH12_INT1        5
#define LIS2DH12_INT2        6

#define LIS2DH12_FREQUENCY  SPI_FREQUENCY_FREQUENCY_M4


#define LIS2DH12_REG_WHO_AM_I         0x0F


#define LIS2DH12_REG_CTRL_REG1        0x20
#define LIS2DH12_REG_CTRL_REG2        0x21
#define LIS2DH12_REG_CTRL_REG3        0x22
#define LIS2DH12_REG_CTRL_REG4        0x23

#define LIS2DH12_REG_OUT_X_L          0x28
#define LIS2DH12_REG_OUT_X_H          0x29
#define LIS2DH12_REG_OUT_Y_L          0x2A
#define LIS2DH12_REG_OUT_Y_H          0x2B
#define LIS2DH12_REG_OUT_Z_L          0x2C
#define LIS2DH12_REG_OUT_Z_H          0x2D

typedef struct
{
  uint32_t MISO;
  uint32_t MOSI;
  uint32_t CSN;
  uint32_t SCK;
  uint32_t INT1;
  uint32_t INT2;
  uint32_t FREQUENCY;
  
}LIS2DH12_InitStruct;

typedef enum
{
    LIS2DH12_OM_LOW_POWER = 0x10,
    LIS2DH12_OM_NORMAL = 0x00,
    LIS2DH12_OM_HIGH_RESOLUTION =0x01,
    LIS2DH12_OM_NOT_ALLOWED = 0x11,
}LIS2DH12_OPERATE_MODE;

typedef enum
{
	LIS2DH12_ACC_CHAN_X = 0,
	LIS2DH12_ACC_CHAN_Y,
	LIS2DH12_ACC_CHAN_Z,
}LIS2DH12_ACC_CHAN;
  
typedef enum
{
    LIS2DH12_MS_ENABLE,
    LIS2DH12_MS_DISABLE,
}LIS2DH12_MODULE_STATE;

typedef enum
{
  LIS2DH12_DATA_RAT_0HZ = 0x00,
  LIS2DH12_DATA_RAT_1HZ = 0x01,
  LIS2DH12_DATA_RAT_10HZ = 0x02,
  LIS2DH12_DATA_RAT_25HZ = 0x03,
  LIS2DH12_DATA_RAT_50HZ = 0x04,
  LIS2DH12_DATA_RAT_100HZ = 0x05,
  LIS2DH12_DATA_RAT_200HZ = 0x06,
  LIS2DH12_DATA_RAT_400HZ = 0x07,
  LIS2DH12_DATA_RAT_L_1620HZ = 0x08,
  LIS2DH12_DATA_RAT_HN_1344HZ_L_5376HZ = 0x09,
}LIS2DH12_DATA_RAT;

typedef enum
{
  LIS2DH12_SELF_TEST_MODE_DISABLE = 0x00,
  LIS2DH12_SELF_TEST_MODE_MODE1 = 0x01,
  LIS2DH12_SELF_TEST_MODE_MODE2 = 0x02,
}LIS2DH12_SELF_TEST_MODE;

typedef enum
{
    LIS2DH12_MODULE_TEMP,
}LIS2DH12_MODULE;

extern uint32_t               LIS2DH12_Config(LIS2DH12_InitStruct * init_struct);
extern uint32_t               LIS2DH12_Init(void);
extern uint32_t               LIS2DH12_Read_Data(uint8_t addr, uint8_t data[], uint8_t len);
extern uint8_t                LIS2DH12_Read_Reg(uint8_t addr);
extern uint32_t               LIS2DH12_Write_Reg(uint8_t addr, uint8_t value);

extern uint8_t                LIS2DH12_Get_Chip_Id(void);



#endif //_LIS2DH12_NRF51822_H_


