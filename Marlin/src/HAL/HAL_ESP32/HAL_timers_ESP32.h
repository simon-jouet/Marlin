/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _HAL_TIMERS_ESP32_H
#define _HAL_TIMERS_ESP32_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>
#include "driver/timer.h"

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------
//
#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint64_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFFFFFFFFFFULL

#define STEP_TIMER_NUM 0  // index of timer to use for stepper
#define TEMP_TIMER_NUM 1  // index of timer to use for temperature

#define HAL_TIMER_RATE APB_CLK_FREQ // frequency of timer peripherals

#define HAL_STEPPER_TIMER_RATE HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_TICKS_PER_US       ((HAL_STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs
#define STEPPER_TIMER_PRESCALE (CYCLES_PER_MICROSECOND / HAL_TICKS_PER_US)
#define STEP_TIMER_MIN_INTERVAL   8 // minimum time in µs between stepper interrupts

#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define PULSE_TIMER_NUM STEP_TIMER_NUM
#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#define HAL_TEMP_TIMER_ISR extern "C" void tempTC_Handler(void)
#define HAL_STEP_TIMER_ISR extern "C" void stepTC_Handler(void)

// #define PULSE_TIMER_NUM STEP_TIMER_NUM
// #define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE

// #define STEP_TIMER_PRESCALE 80
// #define TEMP_TIMER_PRESCALE 80

// #define STEPPER_TIMER_PRESCALE STEP_TIMER_PRESCALE

// #define STEP_TIMER_RATE APB_CLK_FREQ/STEP_TIMER_PRESCALE
// #define TEMP_TIMER_RATE APB_CLK_FREQ/TEMP_TIMER_PRESCALE


// #define HAL_STEPPER_TIMER_RATE HAL_TIMER_RATE
// #define HAL_TICKS_PER_US ((HAL_STEPPER_TIMER_RATE) / 1000000)


// #define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

// #define STEP_TIMER_MIN_INTERVAL    80 // minimum time in µs between stepper interrupts

// #define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt (STEP_TIMER_NUM)
// #define DISABLE_STEPPER_DRIVER_INTERRUPT()  HAL_timer_disable_interrupt (STEP_TIMER_NUM)
// #define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

// #define ENABLE_TEMPERATURE_INTERRUPT()  HAL_timer_enable_interrupt (TEMP_TIMER_NUM)
// #define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt (TEMP_TIMER_NUM)

// #define HAL_ENABLE_ISRs() do { if (thermalManager.in_temp_isr)DISABLE_TEMPERATURE_INTERRUPT(); else ENABLE_TEMPERATURE_INTERRUPT(); ENABLE_STEPPER_DRIVER_INTERRUPT(); } while(0)


extern "C" void tempTC_Handler(void);
extern "C" void stepTC_Handler(void);


// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

typedef struct {
  timer_group_t  group;
  timer_idx_t    idx;
} tTimerConfig;

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

extern const tTimerConfig TimerConfig[];

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

void HAL_timer_start (const uint8_t timer_num, uint32_t frequency);
void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t count);
hal_timer_t HAL_timer_get_compare(const uint8_t timer_num);
hal_timer_t HAL_timer_get_count(const uint8_t timer_num);

// if counter too high then bump up compare
FORCE_INLINE static void HAL_timer_restrain(const uint8_t timer_num, const uint16_t interval_ticks) {
  const hal_timer_t mincmp = HAL_timer_get_count(timer_num) + interval_ticks;
  if (HAL_timer_get_compare(timer_num) < mincmp) HAL_timer_set_compare(timer_num, mincmp);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
bool HAL_timer_interrupt_enabled(const uint8_t timer_num);

#define HAL_timer_isr_prologue(TIMER_NUM)
#define HAL_timer_isr_epilogue(TIMER_NUM)

#endif // _HAL_TIMERS_ESP32_H
