#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

#include <stdint.h>

uint8_t ColorSensor_Read(uint8_t *color_id);
void ColorSensor_Init(void);
uint8_t PlanMoveFromColor(uint8_t color_id, uint8_t *target_bin);

#endif