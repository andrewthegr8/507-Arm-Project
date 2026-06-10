/*
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>


void     Joystick_Init(void);
//void     Joystick_Read(void);
uint16_t Joystick_ReadX(void);
uint16_t Joystick_ReadY(void);
uint8_t  Joystick_ReadButton(void);

#endif
*/

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdint.h>

void     Joystick_Init(void);
void     Joystick_Read(void);
uint16_t Joystick_ReadX(void);
uint16_t Joystick_ReadY(void);
uint8_t  Joystick_ReadButton(void);

#endif

