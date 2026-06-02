#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <stdint.h>
#include "driver_tcs34725.h"

typedef enum {
    COLOR_RED     = 0,
    COLOR_ORANGE  = 1,
    COLOR_YELLOW  = 2,
    COLOR_GREEN   = 3,
    COLOR_BLUE    = 4,
    COLOR_PURPLE  = 5,
    COLOR_UNKNOWN = 6,
} ColorResult_t;

void         COLOR_SENSOR_Init(tcs34725_handle_t *handle);
ColorResult_t COLOR_SENSOR_Read(tcs34725_handle_t *handle);
void COLOR_SENSOR_PrintRGB(uint16_t r, uint16_t g, uint16_t b, ColorResult_t color);

#endif