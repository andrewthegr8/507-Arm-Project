#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <stdint.h>
#include "driver_tcs34725.h"

/**
 * @defgroup ColorSensing Color Sensing
 * @brief Color sensor reading and color classification. Determines whether the reading is red, orange, yellow, green, blue, or purple.
 * @{
 */
 
typedef enum {
    COLOR_RED     = 0,
    COLOR_ORANGE  = 1,
    COLOR_YELLOW  = 2,
    COLOR_GREEN   = 3,
    COLOR_BLUE    = 4,
    COLOR_PURPLE  = 5,
    COLOR_UNKNOWN = 6,
} ColorResult_t;

/**
 * @brief Initialize the color sensor and enable RGB/C readings.
 * @param handle Pointer to an initialized tcs34725_handle_t for the device.
 */
void COLOR_SENSOR_Init(tcs34725_handle_t *handle);

/**
 * @brief Read raw RGB(C) values and classify the detected color.
 * @param handle Pointer to the tcs34725_handle_t for the device.
 * @return ColorResult_t enum value representing the classified color.
 */
ColorResult_t COLOR_SENSOR_Read(tcs34725_handle_t *handle);

/**
 * @brief Send an RGB printout and classification over the debug UART.
 * @param r Raw red channel value.
 * @param g Raw green channel value.
 * @param b Raw blue channel value.
 * @param color Classified color to print (ColorResult_t).
 */
void COLOR_SENSOR_PrintRGB(uint16_t r, uint16_t g, uint16_t b, ColorResult_t color);

/**
 * @}
 */

uint8_t my_i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);
uint8_t my_i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);
uint8_t my_i2c_init(void);
uint8_t my_i2c_deinit(void);
void my_delay_ms(uint32_t ms);
void my_debug_print(const char *fmt, ...);

#endif