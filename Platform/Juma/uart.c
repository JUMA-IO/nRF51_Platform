#include "sys_common.h"
#include "app_uart.h"

#define SERIAL_HEADER_LENGTH 2

#define SERIAL_SYNC_BYTE 0x81

#define BUFFER_LENGTH (SERIAL_HEADER_LENGTH + SERIAL_FRAME_LENGTH)

static uint8_t m_data_array[BUFFER_LENGTH];
static uint8_t m_index = 0;

static void uart_evt_callback(app_uart_evt_t * uart_evt)
{
    //uint32_t err_code;
	
    switch (uart_evt->evt_type)
    {
        case APP_UART_DATA:	
			//Data is ready on the UART					
            break;
						
		case APP_UART_DATA_READY:
            //Data is ready on the UART FIFO		
            break;
						
        case APP_UART_TX_EMPTY:
			//Data has been successfully transmitted on the UART
            break;
						
        default:
            break;
    }
    
}

void hif_setup(uint8_t rx_pin, uint8_t tx_pin)
{   
    uint32_t err_code;
    app_uart_comm_params_t comm_params;
    
    comm_params.rx_pin_no  = rx_pin;
    comm_params.tx_pin_no  = tx_pin;
    comm_params.rts_pin_no = 0;
    comm_params.cts_pin_no = 0;
    comm_params.flow_control = APP_UART_FLOW_CONTROL_DISABLED;
    comm_params.use_parity   = false;
    comm_params.baud_rate    = UART_BAUDRATE_BAUDRATE_Baud9600;

    APP_UART_FIFO_INIT(&comm_params,
                        128,
                        128,
                        uart_evt_callback,
                        APP_IRQ_PRIORITY_LOW,
                        err_code);

    APP_ERROR_CHECK(err_code);
}

/*
 * Frame Structure
 *
 * Byte 0: SYNC_BYTE
 * Byte 1: Data Length
 */

void uart_process(void)
{
    uint8_t newbyte;
    uint8_t* params;
    uint8_t plen;

    while(app_uart_get(&newbyte) == NRF_SUCCESS)
    {
        if (m_index < BUFFER_LENGTH) {
            m_data_array[m_index++] =  newbyte;
        } else {
            memset(m_data_array, 0, BUFFER_LENGTH);
            m_index = 0;
        }

        if (m_data_array[0] != SERIAL_SYNC_BYTE) {
            memset(m_data_array, 0, BUFFER_LENGTH);
            m_index = 0;
            return;
        }

        if (m_index < SERIAL_HEADER_LENGTH) {
            // receiving frame header
            return;
        }

        plen = m_data_array[1];

        if (m_index < plen + SERIAL_HEADER_LENGTH) {
            // receiving frame body
            return;
        }

        params = m_data_array + SERIAL_HEADER_LENGTH;

        hif_on_message(params, plen);

        memset(m_data_array, 0, BUFFER_LENGTH);
        m_index = 0;
    }
}

void hif_send(uint8_t* data, uint32_t size)
{
    int i;
	uint32_t err_code;
    uint8_t hdr[SERIAL_HEADER_LENGTH];

    hdr[0] = SERIAL_SYNC_BYTE;
    hdr[1] = size;

    for (i=0; i<SERIAL_HEADER_LENGTH; i++) {
        err_code = app_uart_put(hdr[i]);
        APP_ERROR_CHECK(err_code);
    }

    for (i=0; i<size; i++) {
        err_code = app_uart_put(data[i]);
        APP_ERROR_CHECK(err_code);
    }
}
