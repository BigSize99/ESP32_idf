#include "stdio.h"
#include "esp_log.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <input_iot.h>

input_callback_t input_callback = NULL;
static uint64_t _start, _stop, _pressTick;                   // Khai báo đánh dấu giá trị bắt đầu và kết thúc
// Ham xu ly callback
static void IRAM_ATTR gpio_input_handler(void* arg)
{
    int gpio_num = (uint32_t) arg;
    uint32_t rtc = xTaskGetTickCountFromISR();  // Lấy giá trị thời gian hiện tại từ khi hệ thống bắt đầu
    // get mức logic để biết ngắt nào xảy ra
    if(input_io_get_level(gpio_num) == 0)      // check ngắt sườn xuống, trạng thái bình thường nút nhấn esp mức 1
    {
        _start = rtc;                           // Nhan nut
    }else{
        _stop = rtc;                            // Tha tay
        _pressTick = _stop - _start;
        input_callback(gpio_num, _pressTick);   
    }
}

// Ham nay la ham khoi tao, connect chan bao nhieu, bat suon nao
void input_io_create(gpio_num_t gpio_num, interrupt_type_edle_t type)
{
    gpio_pad_select_gpio(gpio_num);
    gpio_set_direction(gpio_num,GPIO_MODE_INPUT);
    gpio_set_pull_mode(gpio_num,GPIO_PULLUP_ONLY);
    gpio_set_intr_type(gpio_num,type);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(gpio_num,gpio_input_handler,(void*)gpio_num);
}

//get muc logic
int input_io_get_level(gpio_num_t gpio_num)
{
    return gpio_get_level(gpio_num);
}

//set callback de ra ngoai xu ly
// bao ra cai su kien de biet ma xu ly
void input_set_callback(void * cb )
{
    input_callback = cb;
}