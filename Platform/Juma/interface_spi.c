#include "interface_spi.h"
#include "stdio.h"

static __INLINE void spi_clear_flog(void)
{
  SPI_INTERFACE->EVENTS_READY = 0;
}

static __INLINE void spi_interface_write(uint8_t data)
{
  SPI_INTERFACE->TXD = data;
}

static __INLINE uint8_t spi_interface_read(void)
{
  uint8_t RX;
  
  while(SPI_INTERFACE->EVENTS_READY == 0);
  SPI_INTERFACE->EVENTS_READY = 0;
  RX = SPI_INTERFACE->RXD;  
  
  return RX;
}

static __INLINE uint8_t spi_interface_rw(uint8_t data)
{
  spi_interface_write(data);
  return spi_interface_read();
}

void spi_setup(spi_init_struct_t * spi_struct)
{
	SPI_INTERFACE->PSELMISO  = spi_struct->MISO;       //MISO
  SPI_INTERFACE->PSELMOSI  = spi_struct->MOSI;       //MOSI
  SPI_INTERFACE->PSELSCK   = spi_struct->SCK;        //SCK
  //config frequency
  SPI_INTERFACE->FREQUENCY = spi_struct->FREQ;
 
	spi_struct->ORDER &= SPI_CONFIG_ORDER_Msk;
	spi_struct->CPHA  &= SPI_CONFIG_CPHA_Msk;
	spi_struct->CPOL  &= SPI_CONFIG_CPOL_Msk;
	
  //config config
  SPI_INTERFACE->CONFIG = spi_struct->CPHA | spi_struct->CPOL | spi_struct->ORDER;
  
  //enable nrf51822 spi interface 
  SPI_INTERFACE->ENABLE = SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos;
}

void spi_transmit_receive(uint8_t * tx_buff, uint8_t * rx_buff, uint32_t buff_len)
{
	uint32_t i = 0;
	
	if((tx_buff == NULL) || (rx_buff == NULL))
	{
		return;
	}
	
	if(buff_len == 0)
	{
		return;
	}

//spi option	
	spi_interface_write(tx_buff[i]);
	i++;
	
	while(i < buff_len)
	{
		rx_buff[i-1] = spi_interface_rw(tx_buff[i]);
		i++;
	}

	rx_buff[i-1] = spi_interface_read();
}

void spi_test()
{
	spi_init_struct_t spi_struct;
	
	spi_struct.MISO = 8;
	spi_struct.MOSI = 9;
	spi_struct.SCK  = 10;
	spi_struct.FREQ = SPI_FREQUENCY_FREQUENCY_M1;
	
	spi_struct.CPHA = SPI_CPHA_1EDGE;
	spi_struct.CPOL = SPI_CPOL_HIGH;
	spi_struct.ORDER = SPI_ORDER_MSB;
		
	spi_setup(&spi_struct);
		
	uint8_t tx_buff[] = "xiaojinglingfen";
	uint8_t rx_buff[20];
	
	spi_transmit_receive(tx_buff, rx_buff, 16);
	
	ble_device_send(0x55, 16, rx_buff);
}





