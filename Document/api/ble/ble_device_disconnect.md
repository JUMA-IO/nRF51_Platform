#断开连接
***
##函数声明
###Nordic
```
void ble_device_disconnect(void);
```

###ST
```
tBleStatus ble_user_disconnect_device(void);
```

***
##函数功能
断开与当前的 BLE 设备的连接。


***
##函数参数 
###Nordic
参数    | 数据类型   | 说明
:----- | :-------- | :------
*返回值*  | 无    | 无

###ST
参数    | 数据类型   | 说明
:----- | :-------- | :------
*返回值*  | tBleStatus （uint8_t）   | Ble Status

***
##函数举例
###Nordic
```
	//当前函数
	void foo()
	{
		...
		
		ble_device_disconnect();	//断开与当前已经连接的ble设备的连接
		
		...
	}
```	

###ST
```
	//当前函数
	void foo()
	{
		...
		
		ble_user_disconnect_device();	//断开与当前已经连接的ble设备的连接
		
		...
	}

```