#include "sys_common.h"
#include "app_uart.h"


static void serial_evt_callback(app_uart_evt_t* evt)
{
	switch( evt->evt_type )
	{
		case APP_UART_DATA:
			break;
		case APP_UART_DATA_READY:
			break;
		case APP_UART_TX_EMPTY:
			break;
		default:
			break;
	}
}

/**< Setup serial port with RX-pin and TX-pin */
void serial_setup(uint8_t rx_pin, uint8_t tx_pin, uint32_t baudrate)
{
	uint32_t err_code;
	app_uart_comm_params_t comm_params;
  
  comm_params.rx_pin_no  = rx_pin;
  comm_params.tx_pin_no  = tx_pin;
  comm_params.rts_pin_no = 0;
  comm_params.cts_pin_no = 0;
  comm_params.flow_control = APP_UART_FLOW_CONTROL_DISABLED;
  comm_params.use_parity   = false;
  comm_params.baud_rate    = baudrate;

  APP_UART_FIFO_INIT(&comm_params,
                      128,
                      128,
                      serial_evt_callback,
                      APP_IRQ_PRIORITY_LOW,
                      err_code);

  APP_ERROR_CHECK(err_code);	
}

/**< Send data to serial port */
void serial_send(uint8_t* data, uint32_t length)
{
	int i;
	uint32_t err_code;
	
	for( i = 0; i < length; i++ )
	{
		err_code = app_uart_put( data[i] );
		APP_ERROR_CHECK( err_code );
	}
}

/**< Get data from serial port and send to app */
void serial_get()
{
	uint8_t tmpbyte;
 
    while( app_uart_get( &tmpbyte ) == NRF_SUCCESS )
    {
		serial_on_data( tmpbyte );
    }
	
}



