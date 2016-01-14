#include "LIS2DH12.h"
#include "nrf51.h"
#include "nrf_gpio.h"
#include "juma_sdk_api.h"

#define LIS2DH12_SPI_INTERFACE NRF_SPI1
#define INVAILVALUE 0x55

//CHIP Reg addr desprition
static LIS2DH12_InitStruct LIS2DH12_Init_Struct = 
{
  .MISO       = 0xFFFFFFFFUL,
  .MOSI       = 0xFFFFFFFFUL,
  .CSN        = 0xFFFFFFFFUL,
  .SCK        = 0xFFFFFFFFUL,
  .INT1       = 0xFFFFFFFFUL,
  .INT2       = 0xFFFFFFFFUL,
  .FREQUENCY  = LIS2DH12_FREQUENCY
};

static __INLINE void spi_cs_enable(void)
{
  nrf_gpio_pin_clear(LIS2DH12_Init_Struct.CSN);
}

static __INLINE void spi_cs_disable(void)
{
  nrf_gpio_pin_set(LIS2DH12_Init_Struct.CSN);
}

static __INLINE void spi_clear_flog(void)
{
  LIS2DH12_SPI_INTERFACE->EVENTS_READY = 0;
}

static __INLINE void LIS2DH12_Write(uint8_t data)
{
  LIS2DH12_SPI_INTERFACE->TXD = data;
}

static __INLINE uint8_t LIS2DH12_Read(void)
{
  uint8_t RX;
  
  while(LIS2DH12_SPI_INTERFACE->EVENTS_READY == 0);
  LIS2DH12_SPI_INTERFACE->EVENTS_READY = 0;
  RX = LIS2DH12_SPI_INTERFACE->RXD;  
  
  return RX;
}

static __INLINE uint8_t LIS2DH12_RW(uint8_t data)
{
  LIS2DH12_Write(data);
  return LIS2DH12_Read();
}

uint32_t LIS2DH12_Init(void)
{
  //config nrf51822 spi interface
  nrf_gpio_cfg_output(LIS2DH12_Init_Struct.CSN);         //CS
  spi_cs_disable();
  
  LIS2DH12_SPI_INTERFACE->PSELMISO  = LIS2DH12_Init_Struct.MISO;       //MISO
  LIS2DH12_SPI_INTERFACE->PSELMOSI  = LIS2DH12_Init_Struct.MOSI;       //MOSI
  LIS2DH12_SPI_INTERFACE->PSELSCK   = LIS2DH12_Init_Struct.SCK;        //SCK
  //config frequency
  LIS2DH12_SPI_INTERFACE->FREQUENCY = LIS2DH12_Init_Struct.FREQUENCY;
 
  //config config
  LIS2DH12_SPI_INTERFACE->CONFIG = (SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos)|
                     (SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos)|
                     (SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos);
  
  //enable nrf51822 spi interface 
  LIS2DH12_SPI_INTERFACE->ENABLE = SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos;
  return 0;
}

uint32_t LIS2DH12_Config(LIS2DH12_InitStruct * initstruct)
{
  LIS2DH12_Init_Struct = *initstruct;
  
  LIS2DH12_Init();
  
  return 0;
}

uint32_t LIS2DH12_Read_Data(uint8_t addr, uint8_t data[], uint8_t len)
{
  uint8_t i;
  uint8_t RX;
  uint8_t cmd;
  
  if(len == 0) return 2;
  
  spi_clear_flog();
  spi_cs_enable();
  
  cmd = 0x80 | 0x40 | addr;
  
  LIS2DH12_Write(cmd);
  LIS2DH12_RW(INVAILVALUE);
  
  for(i=1; i<len; i++)
  {
    data[i-1] = LIS2DH12_RW(INVAILVALUE);
  }
  
  data[i-1] = LIS2DH12_Read();
  
  spi_cs_disable();
  
  return 0;
}

uint8_t  LIS2DH12_Read_Reg(uint8_t addr)
{
  uint8_t RX;
  uint8_t cmd;
  
  spi_clear_flog();
  spi_cs_enable();
  
  cmd = 0x80 | addr;
  cmd &= ~0x40;
  
  LIS2DH12_Write(cmd);
  LIS2DH12_RW(INVAILVALUE);
  RX = LIS2DH12_Read();
  
  spi_cs_disable();

  return RX;
}

uint32_t LIS2DH12_Write_Reg(uint8_t addr, uint8_t value)
{
  spi_clear_flog();
  spi_cs_enable();
  
  LIS2DH12_Write(addr & (~0x80));
  LIS2DH12_RW(value);
  LIS2DH12_Read();

  spi_cs_disable();
  
  return 0;
}

uint32_t LIS2DH12_Module_Enable()
{

}



#define LIS2DH12_REG_CTRL_REG1_LPEN 0x80
#define LIS2DH12_REG_CTRL_REG4_HR   0x80

LIS2DH12_OPERATE_MODE LIS2DH12_Get_Operat_Mode()
{
  uint8_t reg;
  uint8_t data[2];
  
  data[0] = LIS2DH12_Read_Reg(LIS2DH12_REG_CTRL_REG1);
  data[1] = LIS2DH12_Read_Reg(LIS2DH12_REG_CTRL_REG4);
  
  reg = ((data[0]&LIS2DH12_REG_CTRL_REG1_LPEN)?0x10:0x00) |
          ((data[1]&LIS2DH12_REG_CTRL_REG4_HR)?0x01:0x00);
  
  switch(reg)
  {
    case LIS2DH12_OM_LOW_POWER: break;
    case LIS2DH12_OM_NORMAL:    break;
    case LIS2DH12_OM_HIGH_RESOLUTION: break;
    default : break;
  }
}

uint32_t LIS2DH12_Set_Operat_Mode(LIS2DH12_OPERATE_MODE operate_mode)
{

}

LIS2DH12_DATA_RAT LIS2DH12_Get_Data_Rate()
{
  uint8_t reg;
  
  reg = LIS2DH12_Read_Reg(LIS2DH12_REG_CTRL_REG1);
  
  reg >>= 4;
  
  return (LIS2DH12_DATA_RAT)reg;
}

uint32_t LIS2DH12_Set_Data_Rate(LIS2DH12_DATA_RAT data_rat)
{
  uint8_t reg;
  
  reg = LIS2DH12_Read_Reg(LIS2DH12_REG_CTRL_REG1);
  
  reg &= 0x0F;
  reg |= data_rat << 4;
  
  LIS2DH12_Write_Reg(LIS2DH12_REG_CTRL_REG1, reg);
  
  return 0;
}

uint32_t LIS2DH12_Work_Model()
{

}

/*
return : Chip Id
*/
uint8_t LIS2DH12_Get_Chip_Id(void)
{
  return LIS2DH12_Read_Reg(LIS2DH12_REG_WHO_AM_I);
}

uint32_t LIS2DH12_Chan_Enable(LIS2DH12_ACC_CHAN chan)
{
  uint8_t reg;
  
  reg = LIS2DH12_Read_Reg(LIS2DH12_REG_CTRL_REG1);
  
  switch(chan)
  {
    case LIS2DH12_ACC_CHAN_X: reg |= 0x01; break;
    case LIS2DH12_ACC_CHAN_Y: reg |= 0x02; break;
    case LIS2DH12_ACC_CHAN_Z: reg |= 0x04; break;
    default:break;
  }
  
  LIS2DH12_Write_Reg(LIS2DH12_REG_CTRL_REG1, reg);
  
  return 0;
}

uint32_t LIS2DH12_Chan_Disable(LIS2DH12_ACC_CHAN chan)
{
  uint8_t reg;
  
  reg = LIS2DH12_Read_Reg(LIS2DH12_REG_CTRL_REG1);
  
  switch(chan)
  {
    case LIS2DH12_ACC_CHAN_X: reg &= ~0x01; break;
    case LIS2DH12_ACC_CHAN_Y: reg &= ~0x02; break;
    case LIS2DH12_ACC_CHAN_Z: reg &= ~0x04; break;
    default:break;
  }
  
  LIS2DH12_Write_Reg(LIS2DH12_REG_CTRL_REG1, reg);
  
  return 0;
}

int16_t LIS2DH12_Get_Chan_Data(LIS2DH12_ACC_CHAN chan)
{
  uint8_t addr;
  uint8_t data[2];
  int16_t value;
  
  switch(chan)
  {
    case LIS2DH12_ACC_CHAN_X: addr = LIS2DH12_REG_OUT_X_L;  break;
    case LIS2DH12_ACC_CHAN_Y: addr = LIS2DH12_REG_OUT_Y_L;  break;
    case LIS2DH12_ACC_CHAN_Z: addr = LIS2DH12_REG_OUT_Z_L;  break;
    default: addr = LIS2DH12_REG_OUT_X_L; break;
  }
  
  LIS2DH12_Read_Data(addr, data, 2);  
  
  value = data[1];
  value <<= 8;
  value |= data[0];
  value >>= 6;
  
  return value;
}

uint32_t LIS2DH12_SET_SELF_TEST_MODE(LIS2DH12_SELF_TEST_MODE mode)
{
  uint8_t reg;
  
  reg = LIS2DH12_Read_Reg(LIS2DH12_REG_CTRL_REG4);
  
  reg &= ~0x06;
  
  switch(mode)
  {
    case LIS2DH12_SELF_TEST_MODE_DISABLE :  reg |= 0x00; break;
    case LIS2DH12_SELF_TEST_MODE_MODE1 :   reg |= 0x02; break;
    case LIS2DH12_SELF_TEST_MODE_MODE2 :   reg |= 0x06; break;
    default: reg |= 0x00; break;
  }
  
  LIS2DH12_Write_Reg(LIS2DH12_REG_CTRL_REG4, reg);
  
  return 0;
}
