#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

void Joystick_Init(void);
uint8_t Joystick_IsMoved(void);
uint8_t Joystick_ButtonPressed(void);
void ManualControl_UpdateFromJoystick(void);
uint8_t ManualLimitReached(void);

#endif