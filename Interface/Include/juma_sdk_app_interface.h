
#ifndef _JUMA_SDK_APP_INTERFACE_H_
#define _JUMA_SDK_APP_INTERFACE_H_
#include "juma_sdk_vectors.h"

extern app_descriptor_t* app;

#define on_ready() \
	if(app) app->on_ready()
#define gpio_on_change(pins_state) \
	if(app) app->gpio_on_change(pins_state)
#define timer_on_fired() \
	if(app) app->timer_on_fired()
#define ble_device_on_connect() \
	if(app) app->ble_device_on_connect()
#define ble_device_on_disconnect(reason) \
	if(app) app->ble_device_on_disconnect(reason)
#define ble_device_on_message(type, length, value) \
	if(app) app->ble_device_on_message(type, length, value)
#define serial_on_data(data) \
	if(app) app->serial_on_data(data)
#define data_storage_on_finish(op_code) \
	if(app) app->data_storage_on_finish(op_code)
#define watchDog_on_timerout(juma_wdt_statue) \
	if(app) app->watchDog_on_timerout(juma_wdt_statue)
#endif
