
#关闭BLE广播
***
##函数声明
###Nordic
```
void ble_device_stop_advertising(void);
```

###ST
```
tBleStatus ble_device_stop_advertising(void);
```

***
##函数功能
关闭BLE设备的广播。

***
##函数参数
###Nordic
参数    | 数据类型   | 说明
:----- | :-------- | :------
*返回值*  | 无    | 无

###ST
参数    | 数据类型   | 说明
:----- | :-------- | :------
*返回值*  | ret    | status

***
##函数举例

```	

	//当前函数
	void foo()
	{
		...
	
		//调用本函数开启BLE的广播
		ble_device_stop_advertising();
	
		...
	}
```
