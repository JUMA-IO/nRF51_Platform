#断开连接事件
***
##函数声明
###Nordic
```
void ble_device_on_disconnect(uint8_t  reason);
```

###ST
```
void ble_device_on_disconnect(uint8_t  reason);
```

***
##函数功能
当原来连接的 BLE 设备被断开后由系统调用。  
处理原来连接的 BLE 设备断开后的任务。 

***
###函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*reason*|uint8_t|断开的原因
*返回值*  | void      | 无

***
###函数举例
```
	//当前函数
	void ble_device_on_disconnect(uint8_t reason)
	{
		...
		
		switch(reason)
		{
			case 0: ...break;
			case 1: ...break;
			case 2: ...break;
			
			default: ... break;
		}
		
		...
	}
	
```
