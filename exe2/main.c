#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events & 0x4) { 
        if (gpio == BTN_PIN_R) { // Se for o botão vermelho
            flag_r = !flag_r;
            if (!flag_r) gpio_put(LED_PIN_R, 0); // Garante que apague se desativado
        } 
        
        else if (gpio == BTN_PIN_G) { // Se for o botão verde
            flag_g = !flag_g;
            if (!flag_g) gpio_put(LED_PIN_G, 0); // Garante que apague se desativado
        }
    }
}

int main() {
    stdio_init_all();

    // Configuração dos LEDs
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_put(LED_PIN_R, 0); // 🚀 Inicializa desligado

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_put(LED_PIN_G, 0); // 🚀 Inicializa desligado

    // Configuração dos botões
    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    // Ativa interrupções com o MESMO callback para ambos os botões
    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);
    // Inicializa os tempos de controle
    uint64_t future_time_r = time_us_64() + 500000; // 500ms
    uint64_t future_time_g = time_us_64() + 250000; // 250ms

    while (true) {
        uint64_t now = time_us_64(); // Obtém o tempo atual

        // Controle do LED Vermelho (500ms)
        if (now >= future_time_r) {
            future_time_r = now + 500000; // Próximo ciclo em 500ms

            if (flag_r) {
                gpio_put(LED_PIN_R, !gpio_get(LED_PIN_R)); // 🚀 Alterna o LED R
            } else {
                gpio_put(LED_PIN_R, 0); // 🚀 Mantém apagado caso flag_r = 0
            }
        }

        // Controle do LED Verde (250ms)
        if (now >= future_time_g) {
            future_time_g = now + 250000; // 🚀 Agora pisca corretamente a cada 250ms

            if (flag_g) {
                gpio_put(LED_PIN_G, !gpio_get(LED_PIN_G)); // 🚀 Alterna o LED G
            } else {
                gpio_put(LED_PIN_G, 0); // 🚀 Mantém apagado caso flag_g = 0
            }
        }

        sleep_ms(1); // 🚀 Evita uso excessivo de CPU
    }

    return 0;
}
