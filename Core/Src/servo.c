/* claude code*/

#include "servo.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;

#define SERVO_MIDDLE    1500u // 1.5ms equivalent in timer counts for 20 ms period (2000*1/50)
#define SERVO_MIN_PULSE 1000u  // 1.0ms equivalent in timer counts for 20 ms period (2000*1/20)
#define SERVO_MAX_PULSE 2000u  // 2.0ms equivalent in timer counts for 20 ms period (2000*1/10)

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, SERVO_MIN_PULSE);
}

void Servo_Center(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, SERVO_MIDDLE);
}

void Servo_Close(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, SERVO_MAX_PULSE);
}