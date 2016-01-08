#设置设备发射功率
***
##函数声明
###Nordic
```
void ble_device_set_tx_power(int8_t tx_power);
```

###ST
```
tBleStatus ble_device_set_tx_power(uint8_t level)；
```

***
##函数功能
###Nordic
设置当前的 BLE 模块的发射功率。
模块可接受的功率值为 -40，-30，-20，-16，-12，-8，-4，0和4dBm。  

###ST
设置当前模块发射功率等级, PA Level:0~7.
默认发射功率为最大值8dbm。  

***
###函数参数
###Nordic
参数    | 数据类型   | 说明
:----- | :-------- | :------
*tx_power*|int8_t|需要设置的发射功率
*返回值*  | 无    | 无

###ST
参数    | 数据类型   | 说明
:----- | :-------- | :------
*level*|uint8_t|需要设置的发射功率等级
*返回值*  | ret    | status

***
##函数举例
###Nordic
```
	//当前函数
	void foo()
	{
		...
		
		ble_device_set_tx_power(-40);	//设置本BLE模块的发射功率
				
		...
	}
	
```

###ST
```
	//当前函数
	void foo()
	{
		...
		tBleStatus ret；
		uint8_t level = 4；
		ret = ble_device_set_tx_power(level)；	//设置本BLE模块的发射功率等级
				
		...
	}
```	
