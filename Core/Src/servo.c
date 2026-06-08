/* claude code*/

#include "servo.h"
#include "main.h"

extern TIM_HandleTypeDef htim15;

#define SERVO_MIDDLE    1500u  // 1.5ms equivalent in timer counts for 20 ms period (20000*1.5/20)
#define SERVO_MIN_PULSE 1000u  // 1.0ms equivalent in timer counts for 20 ms period (20000*1.0/20)
#define SERVO_MAX_PULSE 2000u  // 2.0ms equivalent in timer counts for 20 ms period (20000*2.0/20)

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
}
    
void Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, SERVO_MIN_PULSE);
}

void Servo_Center(void)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, SERVO_MIDDLE);
}

void Servo_Close(void)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, SERVO_MAX_PULSE);
}