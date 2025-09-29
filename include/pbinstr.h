#ifndef PBINSTR_H
#define PBINSTR_H

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"

uint32_t time_ms();

typedef struct {
    uint32_t last;     
    uint32_t periodo;
} timer_t;

void init_timer(timer_t* T, uint32_t periodo_ms);

bool check_timer(timer_t* T);  

void gpio_blink(uint32_t gpio, uint32_t tiempo_ms, uint32_t intervalo_ms);


#define MAX_BUTTONS 8   // Máximo número de botones soportados

typedef enum {
    BUTTON_PULLUP,
    BUTTON_PULLDOWN
} button_mode_t;

typedef struct {
    uint8_t pin;
    button_mode_t mode;
    uint32_t debounce_delay;
    volatile bool pressed;    // Estado estable del botón
    uint32_t last_time;       // Última vez que cambió (para antirrebote)
} button_t;

// Inicializa un botón y lo registra en la lista global
void button_init(button_t *btn, uint8_t pin, button_mode_t mode, uint32_t debounce_ms);

// Devuelve true si está presionado
bool button_is_pressed(button_t *btn);


#endif