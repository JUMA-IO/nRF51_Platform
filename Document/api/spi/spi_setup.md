#SPI接口参数配置

***
##函数声明
```
	void spi_setup(spi_init_struct_t * spi_struct);
```
##函数用途
spi总线的配置。提供一个3线的spi总线接口，如果需要CSN等使能线，需要自行用gpio来模拟。

***
##函数参数
参数    | 数据类型   | 说明
:----- | :-------- | :------
spi_struct| spi_init_struct_t *  | spi相关的配置参数
*返回值*  | void     | 无

###关于 *spi_init_struct_t* 的描述
这个参数详细的定义了spi总线使用的时候需要的一些必要参数，其整体定义在juma_sdk_types.h中有对其的详细说明。

```
typedef struct
{
	uint32_t MISO;			//miso pin
	uint32_t MOSI;			//mosi pin
	uint32_t SCK;				//sck	pin
	uint32_t FREQ;			//bus speed
	
	uint32_t ORDER;			//order msb lsb
	uint32_t CPOL;			//cpol
	uint32_t CPHA;			//cpha
}spi_init_struct_t;
```
关于内部的 ORDER、CPOL、CPHA等有以下的定义。

```
ORDER：
SPI_ORDER_MSB
SPI_ORDER_LSB
```
```
CPOL:
SPI_CPOL_HIGH
SPI_CPOL_LOW
```
```
CPHA:
SPI_CPHA_1EDGE
SPI_CPHA_2EDGE
```

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