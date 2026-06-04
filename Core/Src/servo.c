/* claude code*/

#include "servo.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;

#define SERVO_MIN_PULSE    // 1ms equivalent in timer counts
#define SERVO_MAX_PULSE    // 2ms equivalent in timer counts

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, SERVO_MIN_PULSE);
}

void Servo_Close(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, SERVO_MAX_PULSE);
}