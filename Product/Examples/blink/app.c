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

void led_on_task(void* args);
void led_off_task(void* args);

void on_ready()
{
  gpio_setup(LED_1, GPIO_OUTPUT);
  run_after_delay(led_on_task, NULL, 100);
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

