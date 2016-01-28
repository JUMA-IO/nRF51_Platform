/*
  Copyright 2014-2015 juma.io

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "juma_sdk_api.h"


#define LED_1 18
#define LED_2 19

#define ADC_PIN 4

void led_on_task(void* args);
void led_off_task(void* args);
void adc_task(void* args);

void on_ready()
{
  ble_device_set_name("JUMA_ADC_DEMO");
  gpio_setup(LED_1, GPIO_OUTPUT);

  run_after_delay(led_on_task, NULL, 100);
  
  run_after_delay(adc_task, NULL, 100);

  ble_device_set_advertising_interval(200);
  ble_device_start_advertising();
}

void ble_device_on_connect(void)
{

}


void ble_device_on_disconnect(uint8_t reason)
{
  ble_device_start_advertising();
}


void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{

}

void led_on_task(void* args)
{
  // Pull Up
  gpio_write(LED_1, 1);

  run_after_delay(led_off_task, NULL, 250);
}

void led_off_task(void* args)
{
  // Pull Down
  gpio_write(LED_1, 0);

  run_after_delay(led_on_task, NULL, 750);
}

void on_adc_complete(void* args)
{
  adc_result_t *result = (adc_result_t*)args;
  uint8_t buffer[2];

  buffer[0] = result->value >> 8;
  buffer[1] = result->value & 0xff;

  ble_device_send(0, 2, buffer);
}

void adc_task(void* args)
{
  adc_measure(ADC_PIN, 10, on_adc_complete);
  
  run_after_delay(adc_task, NULL, 500);
}
