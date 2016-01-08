
#设定BLE的广播间隔
***
##函数声明
###Nordic
```
void ble_device_set_advertising_interval(uint16_t interval);
```

###ST
```
void ble_device_set_advertising_interval(uint16_t interval);
```

***
##函数功能
设定BLE的广播间隔。  

***
##函数特点
###Nordic
在广播开始之前调用。  

###ST
在ble_init_bluenrg();之前调用。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*interval*|uint16_t |广播间隔
*返回值*  | 无    | 无

***
###关于 *interval* 的描述
两次广播包的发送间隔。单位为ms。


***
##函数举例

```	
	//当前函数
	void foo()
	{
		...
	
		//调用本函数设定两次广播包发送的间隔
		ble_device_set_advertising_interval(1600);
	
		...
	}
```
