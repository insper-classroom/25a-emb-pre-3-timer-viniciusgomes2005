#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <inttypes.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;
volatile uint64_t press_start_time = 0;  

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        press_start_time = time_us_64();
    } else if (events & GPIO_IRQ_EDGE_RISE) { 
        uint64_t now = time_us_64();
        uint64_t press_duration = now - press_start_time;
        if (press_duration >= 500000) {
            flag_f_r = !flag_f_r;  
        } else {
            flag_f_r = flag_f_r;  
        }
    }
}
int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    while (true) {

        if (flag_f_r) {
            gpio_put(LED_PIN_R,1);
        }else{
            gpio_put(LED_PIN_R,0);
        }
    }
}
