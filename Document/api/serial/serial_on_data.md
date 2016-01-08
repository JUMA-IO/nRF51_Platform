#串口收到数据事件

***
##函数声明
```
	void serial_on_data(uint8_t data);      
```

***
##函数用途

当串口收到数据的时候自动调用，收到一个字符送回来一个字符

***
##函数参数

参数    | 数据类型   | 说明
:----- | :-------- | :------
*data*   | uint8_t  | 串口收到的数据
*返回值*  | void      | 无

***

##函数举例
```
	//回调函数
	void serial_on_data( uint8_t data )
	{
		//对收到的数据进行处理
	}

	//当前函数
	void foo( void )
	{
		...
		
		/*配置收发引脚
		  假设收发引脚已宏定义*/
		serial_setup( RX_PIN, TX_PIN );
		
		...
	}
```