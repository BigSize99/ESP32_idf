/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "input_iot.h"
#include "output_iot.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 2


void input_event_callback(int pin)
{
    if(pin == GPIO_NUM_0)
    {
        output_io_toggle(BLINK_GPIO);
    }
}

void app_main(void)
{
    // Khai bao mot cai nut nhan, bat suon xuong
    output_io_create(BLINK_GPIO);
    input_io_create(GPIO_NUM_0, HI_TO_LO);
    // Set ham call back
    // Bat cu khi nao xay ra ngat no se goi ham "output_event_callback()"
    input_set_callback(input_event_callback); 
}
