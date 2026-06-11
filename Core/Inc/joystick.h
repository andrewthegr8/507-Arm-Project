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

/**
 * @defgroup Joystick Joystick
 * @brief Color sensor reading and classification.
 * @{
 */

void     Joystick_Init(void);
void     Joystick_Read(void);
uint16_t Joystick_ReadX(void);
uint16_t Joystick_ReadY(void);
uint8_t  Joystick_ReadButton(void);

/**
 * @brief Initialize and calibrate the ADC used by the joystick.
 */
// void Joystick_Init(void);

/**
 * @brief Sample the joystick ADC channels and update internal state.
 */
// void Joystick_Read(void);

/**
 * @brief Get the current X direction state of the joystick.
 * @return 0 = centered, 1 = negative, 2 = positive
 */
// uint16_t Joystick_ReadX(void);

/**
 * @brief Get the current Y direction state of the joystick.
 * @return 0 = centered, 1 = negative, 2 = positive
 */
// uint16_t Joystick_ReadY(void);

/**
 * @brief Read the joystick select/button input.
 * @return 1 if button pressed, 0 otherwise.
 */
// uint8_t Joystick_ReadButton(void);

/**
 * @}
 */

#endif

