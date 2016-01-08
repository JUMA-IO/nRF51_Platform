#建立连接事件
***
##函数声明
###Nordic
```
void ble_device_on_connect(void);
```

###ST
```
void ble_device_on_connect(void);
```

***
##函数功能
当有 BLE 设备连接本设备后由系统调用。  
用于处理 BLE 设备连接后的任务。 

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*返回值*  | void      | 无

***
##函数举例
```
	//当前函数
	void ble_device_on_connect()
	{
		...
		
		red_led_blink();	//伪指令
		
		...
	}
```	

