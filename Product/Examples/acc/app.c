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

#include "juma_sdk_api.h"
#include "LIS2DH12.h"
#include "stdio.h"

uint8_t mode;

void led_show(uint8_t mode)
{
  gpio_setup(5, GPIO_OUTPUT);
  gpio_setup(6, GPIO_OUTPUT);
  gpio_setup(7, GPIO_OUTPUT);
  
  gpio_write(5, 1);
  gpio_write(6, 1);
  gpio_write(7, 1);
  
  switch(mode)
  {
    case 0: gpio_write(5, 0); break;
    case 1: gpio_write(6, 0); break;
    case 2: gpio_write(7, 0); break;
  }
}

uint8_t light_en;
void light_blink(void * args)
{
  static uint8_t state;
  
  led_show(4);
  
  if(light_en)
  {
    state ++;
    
    if(state >= 200)
    {
      state = 0;
    }
    
    if(!state)
    {
      led_show(mode);
    }
  }
  
  run_after_delay(light_blink, NULL, 10);
}


uint8_t acc_en;
void send_acc_data(void * args)
{
    uint8_t data[6];
    int16_t acc_value;
    char buff[20];
  
  if(acc_en)
  {
    //x
    acc_value = LIS2DH12_Get_Chan_Data(LIS2DH12_ACC_CHAN_X);
    data[0] = (acc_value >> 8) & 0xFF;
    data[1] = acc_value & 0xFF;
  
//  sprintf(buff, "X:%d", acc_value);
//  ble_device_send(0x01, strlen(buff), (uint8_t *)buff);
  
    //y
    acc_value = LIS2DH12_Get_Chan_Data(LIS2DH12_ACC_CHAN_Y);
    data[2] = (acc_value >> 8) & 0xFF;
    data[3] = acc_value & 0xFF;
  
//  sprintf(buff, "Y:%d", acc_value);
//  ble_device_send(0x01, strlen(buff), (uint8_t *)buff);
  
    //z
    acc_value = LIS2DH12_Get_Chan_Data(LIS2DH12_ACC_CHAN_Z);
    data[4] = (acc_value >> 8) & 0xFF;
    data[5] = acc_value & 0xFF;
    
//    sprintf(buff, "Z:%d", acc_value);
//    ble_device_send(0x01, strlen(buff), (uint8_t *)buff);
  
    ble_device_send(0x01, 6, data);
  }
 
  run_after_delay(send_acc_data, NULL, 100);
}



void on_ready()
{
  mode = 0;
  
  led_show(4);
  
  LIS2DH12_InitStruct LIS = 
    {
       .MISO = 10,
       .MOSI = 9,
       .CSN = 11,
       .SCK = 12,
       .INT1 = 13,
       .INT2 = 14,
       .FREQUENCY = LIS2DH12_FREQUENCY, 
    }; 
    
  LIS2DH12_Config(& LIS);  
    
  LIS2DH12_Set_Data_Rate(LIS2DH12_DATA_RAT_10HZ);  
 
  ble_device_set_advertising_interval(2000);
  ble_device_set_name("NigeL");
  ble_device_start_advertising();
    
  light_en = 0;
  acc_en = 0;
    
  light_blink(NULL); 
}


void ble_device_on_connect(void)
{
  acc_en = 1;
  light_en = 1;
  run_after_delay(send_acc_data, NULL, 100);
}

void ble_device_on_disconnect(uint8_t reason)
{
  acc_en = 0;
  light_en = 0;
  ble_device_start_advertising();
}

void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{
  mode = value[0];
  if(mode >= 0x02)
  {
    mode = 0x00;
  }
  led_show(mode);
  
  LIS2DH12_SET_SELF_TEST_MODE(mode);
}

