#include "pbinstr.h"
#include "pico/time.h"
#include "pico/stdlib.h"

uint32_t time_ms() {
    return time_us_32() / 1000;
}

void init_timer(timer_t* T, uint32_t periodo_ms) {
    T->periodo = periodo_ms;
    T->last = time_ms() + T->periodo;
}

bool check_timer(timer_t* T) {
    if (T->last <= time_ms()) {
        T->last += T->periodo;
        return true;
    } else {
        return false;
    }
}

void gpio_blink(uint32_t gpio, uint32_t tiempo_ms, uint32_t intervalo_ms) {
    gpio_init(gpio);
    gpio_set_dir(gpio, true);

    uint32_t start = time_ms();
    timer_t t;
    init_timer(&t, intervalo_ms);

    while (time_ms() - start < tiempo_ms) {
        if (check_timer(&t)) {
            gpio_xor_mask(1u << gpio); 
        }
    }

    gpio_put(gpio, false); 
}