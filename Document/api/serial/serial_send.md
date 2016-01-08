#串口发送

***
##函数声明
```
	void serial_send(uint8_t* data, uint32_t length);      
```

***
##函数用途

向配置好的发送引脚发送数据

***
##函数参数

参数    | 数据类型   | 说明
:----- | :-------- | :------
*data*   | uint8_t * | 数据的起始地址
*length* | uint32_t | 数据的长度
*返回值*  | void     | 无



***
##函数举例
```
	void foo( void )
	{
		uint8_t* data = "test data";
		/*在调用此函数前
		需先调用serial_setup配置收发引脚*/
		serial_send( data, strlen(data) );
	}
```