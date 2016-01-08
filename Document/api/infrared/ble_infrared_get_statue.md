#获取红外发射当前的状态


***
##函数声明
```
uint32_t ble_infrared_get_statue(void);
```

***
##函数功能
获取红外发射当前的状态

***
##函数特点
由于红外发射过程需要的时间比较长，因此，只能采用异步编程的方案，在红外的发射过程中，代码依旧是可以运行的，而在这个过程中任何的可能导致 红外中断的操作都是不允许存在的。因此可以通过这里的这个参数来知道当前红外发送的过程。只有当红外状态为0或者是1的时候才表示当前的红外发送机是没有运行的，是可以再次操作与红外发送有冲突的操作。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*返回值*  | uint32_t    | 当前红外发送机的状态



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
		

		uint8_t inf_statue = ble_infrared_get_statue();
		while(inf_statue != 0 && inf_statue != 1)
		{
			inf_statue = ble_infrared_get_statue();
		}
		
		ble_infrared_send(data, 32);

		...
	}
```
