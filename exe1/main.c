#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        if(flag_f_r){
            gpio_put(LED_PIN_R, 0);
            flag_f_r = 0;
        }else{
            flag_f_r = 1;
        }
    } else if (events == 0x8) { // rise edge
    }
}

int main() {
    uint64_t future_time = 0;
    stdio_init_all();
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);
    future_time = time_us_64() + 500000;
    while (true) {
        uint64_t now = time_us_64(); 
        if (now >= future_time) {
            future_time = now + 500000; 

            if (flag_f_r) {
                gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R));
            }
        }
    }
}
