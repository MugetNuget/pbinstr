#ifndef PBINSTR_H
#define PBINSTR_H

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"

/**
 * @brief Devuelve el tiempo en milisegundos desde el inicio del programa.
 * @return Tiempo actual en ms.
 */
uint32_t time_ms();

/**
 * @struct timer_t
 * @brief Estructura que representa un temporizador simple.
 * @var timer_t::last Último tiempo registrado en ms.
 * @var timer_t::periodo Periodo del temporizador en ms.
 */
typedef struct {
    uint32_t last;     
    uint32_t periodo;
} timer_t;

/**
 * @brief Inicializa un temporizador.
 * @param T Puntero a la estructura timer_t a inicializar.
 * @param periodo_ms Periodo del temporizador en milisegundos.
 */
void init_timer(timer_t* T, uint32_t periodo_ms);

/**
 * @brief Comprueba si ha transcurrido el periodo del temporizador.
 * @param T Puntero al temporizador a verificar.
 * @return true si el tiempo transcurrido supera el periodo, false si no.
 */
bool check_timer(timer_t* T);  

/**
 * @brief Parpadea un GPIO durante un tiempo determinado y con un intervalo.
 * @param gpio Número del pin GPIO a usar.
 * @param tiempo_ms Tiempo total que se mantiene el parpadeo en ms.
 * @param intervalo_ms Intervalo entre encendido y apagado en ms.
 */
void gpio_blink(uint32_t gpio, uint32_t tiempo_ms, uint32_t intervalo_ms);

#define MAX_BUTTONS 8   /**< Número máximo de botones soportados */

/**
 * @enum button_mode_t
 * @brief Define el modo de conexión del botón.
 * @var BUTTON_PULLUP Botón con resistencia pull-up.
 * @var BUTTON_PULLDOWN Botón con resistencia pull-down.
 */
typedef enum {
    BUTTON_PULLUP,
    BUTTON_PULLDOWN
} button_mode_t;

/**
 * @struct button_t
 * @brief Estructura que representa un botón con antirrebote.
 * @var button_t::pin Número del GPIO donde está conectado el botón.
 * @var button_t::mode Modo de conexión: pull-up o pull-down.
 * @var button_t::debounce_delay Tiempo de antirrebote en ms.
 * @var button_t::pressed Estado estable actual del botón.
 * @var button_t::last_time Último momento en que cambió el estado (para antirrebote).
 */
typedef struct {
    uint8_t pin;
    button_mode_t mode;
    uint32_t debounce_delay;
    volatile bool pressed;
    uint32_t last_time;
} button_t;

/**
 * @brief Inicializa un botón y lo registra en la lista global de botones.
 * @param btn Puntero a la estructura button_t que representa el botón.
 * @param pin Número del GPIO al que está conectado el botón.
 * @param mode Modo de conexión: BUTTON_PULLUP o BUTTON_PULLDOWN.
 * @param debounce_ms Tiempo de antirrebote en milisegundos.
 */
void button_init(button_t *btn, uint8_t pin, button_mode_t mode, uint32_t debounce_ms);

/**
 * @brief Consulta si un botón está presionado.
 * @param btn Puntero al botón a consultar.
 * @return true si el botón está presionado, false si no.
 */
bool button_is_pressed(button_t *btn);

#endif