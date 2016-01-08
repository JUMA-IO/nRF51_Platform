#设置设备名称
***
##函数声明
###Nordic
```
void ble_device_set_name(const char* device_name);
```

###ST
```
void ble_device_set_name(const char* device_name);
```

***
##函数功能
设置当前的 BLE 模块的名字。  

###ST
对于ST平台，请在ble_init_bluenrg();之前调用。  

***
###函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*device_name*|const char * |要设置的名字
*返回值*  | 无    | 无

***
##函数举例
```
	//当前函数
	void foo()
	{
		...
		
		ble_device_set_name("JUMA_BLE");	//设置本BLE模块的名字
				
		...
	}
	
```