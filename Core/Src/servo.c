/* claude code*/

/**
 * @file servo.c
 * @brief Simple servo driver using TIM15 channel 1 PWM output.
 */

#include "servo.h"
#include "main.h"

extern TIM_HandleTypeDef htim15;

/**
 * @brief Pulse width value for the "open" servo position.
 *
 * Timer compare counts chosen for platform timer configuration.
 */
#define SERVO_OPEN    1000u // 1.5ms equivalent in timer counts for 20 ms period (2000*1/50)

/**
 * @brief Pulse width value for the "close" servo position.
 */
#define SERVO_CLOSE   1500u  // 1.0ms equivalent in timer counts for 20 ms period (2000*1/20)

/**
 * @brief Start PWM output for the servo timer channel.
 */
void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
}
    
/**
 * @brief Move servo to configured open position.
 */
void Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, SERVO_OPEN);
}

/**
 * @brief Move servo to configured close position.
 */
void Servo_Close(void)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, SERVO_CLOSE);
}

/**
 * @brief Set servo to an arbitrary pulse width value.
 * @param pulse_width Timer compare value to set.
 */
void Servo_Pos(uint16_t pulse_width)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, pulse_width);
}