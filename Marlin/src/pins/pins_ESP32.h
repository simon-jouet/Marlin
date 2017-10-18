#ifndef BOARD_NAME
  #define BOARD_NAME "ESP32"
#endif

#define LARGE_FLASH true

//
// Limit Switches
//
#define ENDSTOP_INTERRUPTS_PIN 35
#define X_MIN_PIN          143
#define Y_MIN_PIN          142
#define Z_MIN_PIN          141

//
// Z Probe (when not Z_MIN_PIN)
//

//#define Z_MIN_PROBE_PIN  5

#define SLED_PIN           -1

//
// Steppers
//
#define X_STEP_PIN         25
#define X_DIR_PIN          33
#define X_ENABLE_PIN       128
#define X_CS_PIN           -1

#define Y_STEP_PIN         16
#define Y_DIR_PIN          5
#define Y_ENABLE_PIN       129
#define Y_CS_PIN           -1

#define Z_STEP_PIN         2
#define Z_DIR_PIN          0
#define Z_ENABLE_PIN       130
#define Z_CS_PIN           -1

#define E0_STEP_PIN        27
#define E0_DIR_PIN         26
#define E0_ENABLE_PIN      131
#define E0_CS_PIN          -1

//
// Temperature Sensors
//
#define TEMP_0_PIN         39   // Analog Input
#define TEMP_BED_PIN       36   // Analog Input

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
#define HEATER_0_PIN     32
#define FAN_PIN          4
#define HEATER_BED_PIN   17