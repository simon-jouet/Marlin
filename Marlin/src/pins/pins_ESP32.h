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

/**
 * Espressif ESP32 (Tensilica Xtensa LX6) pin assignments
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "Espressif ESP32"
#endif

#define LARGE_FLASH true

//
// Limit Switches
//
#define ENDSTOP_INTERRUPTS_PIN 35
#define X_MIN_PIN          141
#define Y_MIN_PIN          139
#define Z_MIN_PIN          137

//
// Z Probe (when not Z_MIN_PIN)
//

//#define Z_MIN_PROBE_PIN    5

#define SLED_PIN            -1

//
// Steppers
//
#define X_STEP_PIN          32
#define X_DIR_PIN           33
#define X_ENABLE_PIN       130

#define Y_STEP_PIN          16
#define Y_DIR_PIN            5
#define Y_ENABLE_PIN       129

#define Z_STEP_PIN           2
#define Z_DIR_PIN           27
#define Z_ENABLE_PIN       129

#define E0_STEP_PIN         17
#define E0_DIR_PIN          16
#define E0_ENABLE_PIN      131

//
// Temperature Sensors
//
#define TEMP_0_PIN          36   // Analog Input
#define TEMP_BED_PIN        39   // Analog Input

//
// Augmentation for auto-assigning RAMPS plugs
//
#if DISABLED(IS_RAMPS_EEB) && DISABLED(IS_RAMPS_EEF) && DISABLED(IS_RAMPS_EFB) && DISABLED(IS_RAMPS_EFF) && DISABLED(IS_RAMPS_SF) && !PIN_EXISTS(MOSFET_D)
  #if HOTENDS > 1
    #if TEMP_SENSOR_BED
      #define IS_RAMPS_EEB
    #else
      #define IS_RAMPS_EEF
    #endif
  #elif TEMP_SENSOR_BED
    #define IS_RAMPS_EFB
  #else
    #define IS_RAMPS_EFF
  #endif
#endif

//
// Heaters / Fans
//
#define HEATER_0_PIN        23
#define FAN_PIN             19
#define HEATER_BED_PIN      18
