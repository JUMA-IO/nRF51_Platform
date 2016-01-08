#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf51_bitfields.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "device_manager.h"
#include "pstorage.h"
#include "app_trace.h"
#include "app_util_platform.h"
#include "juma_common.h"
#include "juma_data_service.h"
#include "dispatch.h"
#include "sys_common.h"

extern const system_descriptor_t sys;
app_descriptor_t* app;
extern juma_data_t m_juma_data_service;

void ble_device_send(uint8_t type, uint32_t length, uint8_t* value)
{
    uint8_t packet[20];

    if (length > 18) length = 18;

    packet[0] = type;
    packet[1] = length;
    memcpy(packet + 2, value, length);

    juma_event_send(&m_juma_data_service, packet, length + 2);
}

#define CONFIG_PIN 31

void load_app(void)
{
    //uint8_t* magic;
    //magic = (uint8_t*) APP_ZONE_BASE;


    app = (app_descriptor_t*)APP_DESCRIPTOR_ADDR;

    if(0xFFFFFFFFUL == *(uint32_t *)app)
    {
      app = NULL;
    }
    else
    {
      app->on_init(&sys);
    }
}

int8_t get_temperature(void)
{
    int32_t raw, temp;

    sd_temp_get(&raw);

    temp = raw/4 - 10;
    
    return temp;
}
