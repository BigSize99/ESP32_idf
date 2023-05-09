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
#include "freertos/event_groups.h"


EventGroupHandle_t xEventGroup;              // static thì biến này dùng trong app_main, tranh khai bao trung cac file
/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/


#define BLINK_GPIO 2
#define BIT_PRESS_SHORT   	( 1 << 0 ) 
#define BIT_PRESS_NORMAL	( 1 << 1 ) 
#define BIT_PRESS_LONG	    ( 1 << 2 ) 

void input_event_callback(int pin, uint64_t tick)
{
    if(pin == GPIO_NUM_0)
    {
        BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
        int press_ms = tick * portTICK_PERIOD_MS;   // Chuyen tick sang ms   1 tick = 10ms => N tick = 10ms * N ; tần số Freertos mặc định 100HZ
                                                    // portTICK_PERIOD_MS = 1000 / configTICK_RATE_HZ
        if(press_ms < 1000)
        {
            /* Set bit 0 and bit 1 in xEventGroup. */// Tringer sự kiện lên báo hiệu có event
            xEventGroupSetBitsFromISR(xEventGroup, BIT_PRESS_SHORT, &pxHigherPriorityTaskWoken);  /* The bits being set. */
        }else if(press_ms < 3000)
        {
            xEventGroupSetBitsFromISR(xEventGroup, BIT_PRESS_NORMAL, &pxHigherPriorityTaskWoken);
        }
        else if(press_ms > 3000)   
            // xEventGroupSetBitsFromISR(xEventGroup, BIT_PRESS_LONG, &pxHigherPriorityTaskWoken);
        {

        }
    }
}


void vTaskCode( void * pvParameters)
{
   for(;;)
   {
        // Chờ 1 hoặc nhóm bit event được set trước đó
        EventBits_t uxBits = xEventGroupWaitBits(
                    xEventGroup,   /* The event group being tested. */
                    BIT_PRESS_SHORT| BIT_PRESS_NORMAL | BIT_PRESS_LONG, /* The bits within the event group to wait for. */
                    pdTRUE,           // Xoá các bit event yêu cầu trước khi hàm trả về. Xét TRUE để chạy 1 lần, xoá đợi tiếp
                    pdFALSE,          // Không cần đợi hết các event mới vượt qua, chỉ cần có event là qua xét được event
                    portMAX_DELAY );  // Đợi max thời gian
        // Hàm trên đứng đợi chờ event nào vượt qua
        // Xét các event vượt qua
        if(uxBits & BIT_PRESS_SHORT)
        {
            printf("SHORTTTTTTTTTTTTTTTTTTTTT \n");
        }
        if(uxBits & BIT_PRESS_NORMAL)
        {
            printf("NORAMALLLLLLLLLLLLLLLLLLL \n");
        }
        if(uxBits & BIT_PRESS_LONG)
        {
            printf("LONGGGGGGGGGGGGGGGGGGGGGG \n");
        }
    }
}



// Hàm xử lý call back timout
void button_timeout_event_callback(int pin)
{
    if(pin == BUTTON0)
    {
        printf("timoutttttttttttttttttttttt \n");
        output_io_toggle(2);
    }
}

void app_main(void)
{
    /* Attempt to create the event group. */
    xEventGroup = xEventGroupCreate();  


    // Khai bao mot cai nut nhan, bat suon xuong
    output_io_create(BLINK_GPIO);
    input_io_create(GPIO_NUM_0, ANY_EDLE); // cho nó nhan 2 cạnh để tính thời gian

    // Set ham call back
    // Bat cu khi nao xay ra ngat no se goi ham "output_event_callback()"
    input_set_callback(input_event_callback);
    input_set_timeout_callback(button_timeout_event_callback);

    // Tạo task xu ly data
    xTaskCreate(
                  vTaskCode,          /* Function that implements the task. */
                  "vTaskCode",        /* Text name for the task. */
                  1024*2,            /* Stack size in words, not bytes. */
                  NULL,    /* Parameter passed into the task. */
                  4,               /* Priority at which the task is created. */
                  NULL );      /* Used to pass out the created task's handle. */
}
