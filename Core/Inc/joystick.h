#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

uint16_t Joystick_ReadX(void);
uint16_t Joystick_ReadY(void);
uint8_t  Joystick_ReadButton(void);
void Joystick_Init(void);

#endif