#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>

/**
 * @addtogroup ServoControl Servo Control
 * @brief Executes commands to the servo-powered gripper.
 * @{
 */

void Servo_Init(void);
void Servo_Open(void);
void Servo_Close(void);
void Servo_Pos(uint16_t pulse_width);

/**
 * @brief Initialize hardware required for servo PWM (starts PWM timer/channel).
 */
// void Servo_Init(void);

/**
 * @brief Move the servo to the pre-defined "open" pulse width.
 */
// void Servo_Open(void);

/**
 * @brief Move the servo to the pre-defined "close" pulse width.
 */
// void Servo_Close(void);

/**
 * @brief Set servo PWM compare value directly.
 * @param pulse_width Timer compare value representing pulse width.
 */
// void Servo_Pos(uint16_t pulse_width);

/** @} */

#endif