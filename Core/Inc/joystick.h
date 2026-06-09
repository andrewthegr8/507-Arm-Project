#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

/**
 * @defgroup Joystick Joystick
 * @brief Functions for controlling arm motion and motor targets.
 * @{
 */

void     Joystick_Init(void);
//void     Joystick_Read(void);
uint16_t Joystick_ReadX(void);
uint16_t Joystick_ReadY(void);
uint8_t  Joystick_ReadButton(void);

/** @} */

#endif