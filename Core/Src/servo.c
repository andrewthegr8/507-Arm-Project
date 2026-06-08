/* claude code*/

#include "servo.h"
#include "main.h"

extern TIM_HandleTypeDef htim15;

#define SERVO_OPEN    1000u // 1.5ms equivalent in timer counts for 20 ms period (2000*1/50)
#define SERVO_CLOSE   1500u  // 1.0ms equivalent in timer counts for 20 ms period (2000*1/20)

void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
}
    
void Servo_Open(void)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, SERVO_OPEN);
}

void Servo_Close(void)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, SERVO_CLOSE);
}

void Servo_Pos(uint16_t pulse_width)
{
    __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, pulse_width);
}