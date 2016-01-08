#红外发射


***
##函数声明
```
uint32_t ble_infrared_send(const uint8_t * signal_data, uint8_t lenth);
```

***
##函数功能
将一定内容的东西通过本接口发送出去。

***
##函数特点
红外发送过程中是按位发送的，这里的signal_data采用的是MSB的格式。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*signal_data*|const uint8_t * |要通过红外发射的数据
*lenth*|uint8_t |红外数据的长度（bit）
*返回值*  | 无    | 无

***
###关于 *Init_struct* 的描述
这个参数就是要用来对红外发送设备进行配置的全部内容，其整体定义在juma_sdk_types.h中有对其的详细说明。


***
##函数举例

```	
	//当前函数
	void foo()
	{
		...
		ble_infrared_init_struct init_struct;
		
		init_struct.Protocal_type = PWM_HEAD_END;
		
		init_struct.type_data.pwm.infrared_shock_pin = 6;
		init_struct.type_data.pwm.infrared_signal_pin = 5;
		
		init_struct.type_data.pwm.carrier_period = 214;
		init_struct.type_data.pwm.carrier_plus   = 150;
		
		init_struct.type_data.pwm.guidance_burst = 342;
		init_struct.type_data.pwm.guidance_space = 170;
		
		init_struct.type_data.pwm.logic_0_burst  = 21;
		init_struct.type_data.pwm.logic_0_space  = 21;
		
		init_struct.type_data.pwm.logic_1_burst  = 21;
		init_struct.type_data.pwm.logic_1_space  = 62;
		        
		init_struct.type_data.pwm.end_burst      = 21;
		
		ble_infrared_config(&init_struct);

		//使用方法类似下面这个
		uint8_t data[] = {0x55, 0xAA, 0xFF, 0x00};
		ble_infrared_send(data, 32);
		...
	}
```
