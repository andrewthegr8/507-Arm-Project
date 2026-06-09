#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>

/**
 * @addtogroup ServoControl Servo Control
 * @{
 */

void Servo_Init(void);
void Servo_Open(void);
void Servo_Close(void);
void Servo_Pos(uint16_t pulse_width);

/** @} */

#endif