#serial配置

***
##函数声明
```
	serial_setup(uint8_t rx_pin, uint8_t tx_pin, uint32_t baudrate);
```
##函数用途
配置需要作为串口收发的引脚  

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
*rx_pin* | uint8_t  | 接收数据的引脚
*tx_pin* | uint8_t  | 发送数据的引脚
*baudrate*|uint32_t | 波特率设置
*返回值*  | void     | 无



***
##函数举例
```
	#define RX_PIN 24
	#define TX_PIN 25
	
	void foo( void )
	{
		//配置串口收发引脚
		serial_setup( RX_PIN, TX_PIN, UART_BAUDRATE_Baud9600);	
	}
```