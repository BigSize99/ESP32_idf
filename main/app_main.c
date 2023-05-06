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

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO


void input_event_callback(int pin)
{
    if(pin == GPIO_NUM_0)
    {
        static int x = 0;
        gpio_set_level(BLINK_GPIO,x);
        x = 1 - x; // an lan 1 no set = 0; la 2 no set = 1; lan 3 no set = 0;....
    }
}

void app_main(void)
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    // Khai bao mot cai nut nhan, bat suon xuong
    input_io_create(GPIO_NUM_0,HI_TO_LO);

    // Set ham call back
    // Bat cu khi nao xay ra ngat no se goi ham "nput_event_callback()"
    input_set_callback(input_event_callback); 

}
