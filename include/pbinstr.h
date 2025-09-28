#ifndef PBINSTR_H
#define PBINSTR_H

#include <stdint.h>
#include <stdbool.h>

uint32_t time_ms();

typedef struct {
    uint32_t last;     
    uint32_t periodo;
} timer_t;

void init_timer(timer_t* T, uint32_t periodo_ms);

bool check_timer(timer_t* T);  

void gpio_blink(uint32_t gpio, uint32_t tiempo_ms, uint32_t intervalo_ms);



#endif