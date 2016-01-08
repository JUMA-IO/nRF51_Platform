
#ADC测量数据
***
##函数声明
```
void adc_measure(uint8_t pin, uint8_t bits, function_t on_complete);
```

***
##函数功能
获取指定的 PIN 脚上的模拟信号，并在获取完成后自动调用用户指定的函数。  
*on_complete* 用来处理获取到的模拟量。

***
##函数特点
用户需要指定 *pin* 和相应的处理函数，当模拟量获取完毕后系统自动调用处理函数。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*pin*|uint8_t|pin引脚号
*bits*|uint8_t|指定采集的精度
*on_complete*|function_t|模拟信号采集完毕后系统要执行的函数(函数指针)
*返回值*  | 无    | 无

***
###关于 *pin* 的描述

可用于ADC的引脚号：
1/2/3/4/5/26/27

***
###关于 *bits* 的描述

*bits* 的数值|采样精度
:-------- | :------
8| 0-255
9|	0-511
10|	0-1023 

***
###关于 *on_complete* 参数
其格式的定义为：  
```
	typedef void (*function_t)(void* args);
```

***
##函数举例
```	
	//用户自定义回调函数
	void on_adc_complete(void* args)
	{
		adc_result_t *result = (adc_result_t*)args; uint8_t buffer[2];
		//这里的result就是最终采集的到的数据
		
		...
		buffer[0] = result->value >> 8; buffer[1] = result->value & 0xff;
		ble_device_send(buffer, 2);
	}
	
	//当前函数
	void foo()
	{
		...
	
		//调用本函数向系统注册一个当ADC采集完成后会被执行的任务
		adc_measure(4, 8, on_adc_complete);
	
		...
	}
```
