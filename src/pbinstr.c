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

static button_t *button_list[MAX_BUTTONS] = {0};
static uint8_t button_count = 0;

static void gpio_callback(uint gpio, uint32_t events) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    for (int i = 0; i < button_count; i++) {
        button_t *btn = button_list[i];
        if (btn->pin != gpio) continue;

        // Antirrebote
        if ((now - btn->last_time) < btn->debounce_delay) {
            return;
        }
        btn->last_time = now;

        bool raw = gpio_get(btn->pin);
        if (btn->mode == BUTTON_PULLUP) {
            raw = !raw;
        }

        btn->pressed = raw;
    }
}

void button_init(button_t *btn, uint8_t pin, button_mode_t mode, uint32_t debounce_ms) {
    if (button_count >= MAX_BUTTONS) {
        // No se pueden registrar más botones
        return;
    }

    btn->pin = pin;
    btn->mode = mode;
    btn->debounce_delay = debounce_ms;
    btn->pressed = false;
    btn->last_time = 0;

    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);

    if (mode == BUTTON_PULLUP) {
        gpio_pull_up(pin);
    } else {
        gpio_pull_down(pin);
    }

    // Guardar en la lista
    button_list[button_count++] = btn;

    // Registrar callback global (solo la primera vez)
    static bool irq_registered = false;
    if (!irq_registered) {
        gpio_set_irq_enabled_with_callback(pin,
                                           GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                           true,
                                           &gpio_callback);
        irq_registered = true;
    } else {
        gpio_set_irq_enabled(pin,
                             GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                             true);
    }
}

bool button_is_pressed(button_t *btn) {
    return btn->pressed;
}