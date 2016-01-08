
#选择当前设备的广播地址
***
##函数声明
###Nordic
```
void ble_device_select_address(uint8_t index);
```
###ST
```
tBleStatus Advertise_Address(uint8_t* AdvAddress);
```

***
##函数功能
选择本设备要使用的广播地址。

***
##函数特点
###Nordic
每个采用JUMA SDK的模块其内部都已经内置了256个地址，通过该函数可以选择设备要使用的地址。  

###ST
对于ST平台，请在ble_init_bluenrg();之前调用。  

***
##函数参数
###Nordic
参数    | 数据类型   | 说明
:----- | :-------- | :------
*index*|uint8_t|要选择的设备地址索引
*返回值*  | 无    | 无

###ST
参数    | 数据类型   | 说明
:----- | :-------- | :------
*AdvAddress*|uint8_t * |广播名称
*返回值*  | ret   | 设置广播名称是否成功

***
###关于 *index* 的描述
index代表的时内置的地址选择的索引，其范围为 0-255，通过设定该值，可以让设备使用不同的地址。

***
##函数举例
###Nordic
```	

	//当前函数
	void foo()
	{
		...
	
		//调用本函数选择要使用的地址。
		ble_device_select_address(0);
	
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
		uint8_t* AdvAddress = "BlueNRG"；
		//调用本函数设置广播地址。
		ret = Advertise_Address(AdvAddress);
		
		...
	}
```