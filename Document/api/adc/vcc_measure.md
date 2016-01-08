
#内部电压vcc测量
***
##函数声明
```
void vcc_measure(function_t on_complete);
```

***
##函数功能
读取内部电压的值，读取完成后会调用用户指定的函数。  
*on_complete* 由用户指定被调用的函数，用来处理获取到的模拟量。

***
##函数特点
数据读取完成后，的操作由用户来指定。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*on_complete*|function_t|模拟信号采集完毕后系统要执行的函数(函数指针)
*返回值*  | 无    | 无

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
	
		//调用本函数向系统注册内部的电压采集完成后会被执行的任务
		vcc_measure(on_adc_complete);
	
		...
	}
```
