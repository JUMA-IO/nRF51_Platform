#SPI接口发送和接收

***
##函数声明
```
	void spi_transmit_receive(uint8_t * tx_buff, uint8_t * rx_buff, uint32_t buff_len);
```

***
##函数用途
利用已经配置好的SPI接口发送和接收数据。

***
##函数参数

参数    | 数据类型   | 说明
:----- | :-------- | :------
*tx_buff*   | uint8_t * | 要发送的数据的
*rx_buff* | uint8_t * | 接收数据的缓冲区
*buff_len* | uint32_t | 数据的长度
*返回值*  | void     | 无



***
##函数举例
```
	void foo( void )
	{
		//config
		spi_init_struct_t spi_struct;
	
		spi_struct.MISO = 8;
		spi_struct.MOSI = 9;
		spi_struct.SCK  = 10;
		spi_struct.FREQ = SPI_FREQUENCY_FREQUENCY_M1;
	
		spi_struct.CPHA = SPI_CPHA_1EDGE;
		spi_struct.CPOL = SPI_CPOL_HIGH;
		spi_struct.ORDER = SPI_ORDER_MSB;
		
		spi_setup(&spi_struct);

		//send and receive
		uint8_t tx_buff[] = "juma sdk spi test";
		uint8_t rx_buff[20];
		
		spi_transmit_receive(tx_buff, rx_buff, sizeof(tx_buff));
		
		//ble send message
		ble_device_send(0x55, 16, rx_buff);
	}
```