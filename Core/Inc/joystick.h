#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

typedef enum {
    JOY_AXIS_NONE = 0,
    JOY_AXIS_X,
    JOY_AXIS_Y
} JoystickAxis;

void Joystick_Init(void);
uint16_t Joystick_ReadXRaw(void);
uint16_t Joystick_ReadYRaw(void);
int16_t Joystick_GetXCentered(void);
int16_t Joystick_GetYCentered(void);
uint8_t Joystick_ButtonPressed(void);
void Joystick_UpdateManualControl(void);

#endif