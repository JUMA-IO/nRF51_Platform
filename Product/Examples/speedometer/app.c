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

/**********全局变量声明************/
uint8_t 	g_mode;
uint8_t 	light_en;
uint8_t 	acc_en;
uint32_t 	vcc_en;
uint8_t 	g_vcc_value;

/************函数声明区*************/
void on_vcc_complete(void* args);
void ble_device_on_connect(void);
void ble_device_on_disconnect(uint8_t reason);
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value);
void led_on(void* args);
void led_off(void* args);
void send_acc_data(void * args);


/****************功能函数区**************/
//打开LED
void led_on(void* args)
{
	if(light_en)
  {
		gpio_write(5, 1);
		switch(g_mode)
		{
			case 0: gpio_write(7, 0); break;
			case 1: gpio_write(6, 0); break;
		}
	}
	run_after_delay(led_off, NULL, 500);
}


//关闭LED
void led_off(void* args)
{
	gpio_write(5, 1);
	gpio_write(6, 1);
	gpio_write(7, 1);
	run_after_delay(led_on, NULL, 500);
}


//回传数据回手机端
//数据格式: 8个字节[x_h|x_l|y_h|y_l|z_h|z_l|vcc|temp]
void send_acc_data(void * args)
{
	uint8_t data[8];
	int16_t acc_value;
  
  if(acc_en)
  {
    //x轴
    acc_value = LIS2DH12_Get_Chan_Data(LIS2DH12_ACC_CHAN_X);
    data[0] = (acc_value >> 8) & 0xFF;
    data[1] = acc_value & 0xFF;
    //y轴
    acc_value = LIS2DH12_Get_Chan_Data(LIS2DH12_ACC_CHAN_Y);
    data[2] = (acc_value >> 8) & 0xFF;
    data[3] = acc_value & 0xFF;  
    //z轴
    acc_value = LIS2DH12_Get_Chan_Data(LIS2DH12_ACC_CHAN_Z);
    data[4] = (acc_value >> 8) & 0xFF;
    data[5] = acc_value & 0xFF;
		//电量
		data[6] = g_vcc_value;
		//温度
		data[7] = get_temperature();
		
    ble_device_send(0x01, 8, data);
  }
	if(!vcc_en)
	{
		//一分钟检测一次电量
		vcc_en = 600;
		vcc_measure(on_vcc_complete);
	}
	vcc_en--;
  run_after_delay(send_acc_data, NULL, 100);
}


//上电立即执行，整个程序入口函数
void on_ready()
{
	//初始化全局变量
	g_mode 		= 1;
	light_en 	= 0;
	acc_en 		= 0;
	vcc_en 		= 600;
	
	//初始化三轴传感器
	LIS2DH12_InitStruct LIS = 
		{
			 .MISO 	= 10,
			 .MOSI 	= 9,
			 .CSN 	= 11,
			 .SCK 	= 12,
			 .INT1 	= 13,
			 .INT2 	= 14,
			 .FREQUENCY = LIS2DH12_FREQUENCY, 
		};  
	LIS2DH12_Config(& LIS);  
	LIS2DH12_Set_Data_Rate(LIS2DH12_DATA_RAT_10HZ);  
	
	//初始化RGB的GPIO
	gpio_setup(5, GPIO_OUTPUT);
	gpio_setup(6, GPIO_OUTPUT);
	gpio_setup(7, GPIO_OUTPUT);

		
	//设置蓝牙的广播周期、广播名
	ble_device_set_advertising_interval(1000);
	ble_device_set_name("Speedometer");
	//开启蓝牙广播
	ble_device_start_advertising();
	
	led_off(NULL);
}


/************回调函数区**************/
//调用电量检测函数后，根据函数指针回调此函数
void on_vcc_complete(void* args)
{
	adc_result_t *result = (adc_result_t*)args;
	
	//低电量
	if(result->value <= 790 )
	{
		g_mode = 0;
		g_vcc_value = 1;
	}
	//一档
	if(790 < result->value <= 850)
	{
		g_mode = 1;
		g_vcc_value = 2;
	}
	//二挡
	if(850 < result->value <= 900)
	{
		g_mode = 1;
		g_vcc_value = 3;
	}
	//三挡
	if(900 < result->value <= 960)
	{
		g_mode = 1;
		g_vcc_value = 4;
	}
	//四挡
	if(960 < result->value <= 1024)
	{
		g_mode = 1;
		g_vcc_value = 5;
	}
	return;
}
//蓝牙被连接成功时，系统自动调用此函数
void ble_device_on_connect(void)
{
  acc_en 		= 1;
  light_en 	= 1;
	vcc_measure(on_vcc_complete);
  run_after_delay(send_acc_data, NULL, 100);
}

//蓝牙断开连接时，系统自动调用此函数
void ble_device_on_disconnect(uint8_t reason)
{
  acc_en 		= 0;
  light_en 	= 0;
  ble_device_start_advertising();
}

//设备端有接收到数据时，系统调用此函数
//参数：type:用户自定义类型;length:数据长度;value:数据内容
void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{
  g_mode = value[0];
  LIS2DH12_SET_SELF_TEST_MODE((LIS2DH12_SELF_TEST_MODE)g_mode);
}



