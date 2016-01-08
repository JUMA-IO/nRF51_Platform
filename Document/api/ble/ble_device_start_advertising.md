
#开始BLE的广播
***
##函数声明
###Nordic
```
void ble_device_start_advertising(void);
```

###ST
```
tBleStatus ble_device_start_advertising(void);
```
***
##函数功能
开启BLE设备广播。

***
##函数特点
###Nordic
芯片上电的时候BLE广播是不开启的，如果要使用BLE，则需要在设置好各种参数之后调用本函数来开启广播。  

###ST
在ble_init_bluenrg();之后设置。

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
		ble_device_start_advertising();
	
		...
	}
```
