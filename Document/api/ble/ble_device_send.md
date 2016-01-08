#发送数据
***
##函数声明
###Nordic
```
	void ble_device_send(uint8_t type, uint32_t length, uint8_t* value);
```

###ST
```
	void ble_device_send(uint8_t type, uint32_t length, uint8_t* value);
```

***
##函数功能
向与之相连的 BLE 设备发送消息。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*type*   | uint8_t | 用户自定义消息类型，定义区间（0x00~0x7F）
*length* | uint32_t | 消息的长度
*value*  | uint8_t * | 消息的内容
*返回值*  | void     | 无

***
###关于 *type* 参数
在jumaSDK系统中对于*type*参数有这样的规定：

*type* 的数值|说明
:-------- | :------
0x00~0x7F | 留给用户自定义，用户用来区分自己的消息类型
0x80~0xFF | 非用户区，系统功能保留

***
##函数举例
```
	#define TYPE 0x01
	//当前函数
	void foo()
	{
		...
		
		//向与之相连的BLE设备发送消息
		ble_device_send( TYPE, strlen("JUMA_BLE"），"JUMA_BLE");
				
		...
	}
	
```