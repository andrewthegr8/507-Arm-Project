#include "joystick.h"
#include "main.h"
#include "motion.h"
#include <stdlib.h>

#define JOY_CENTER_X   2048
#define JOY_CENTER_Y   2048
#define JOY_DEADBAND   150
#define JOY_MAX_CMD    1000

extern ADC_HandleTypeDef hadc1;
extern uint16_t adc_buffer[2];

static int16_t ApplyDeadband(int16_t value)
{
    if (abs(value) < JOY_DEADBAND) {
        return 0;
    }
    return value;
}

static int16_t ScaleValue(int16_t value)
{
    int32_t scaled = ((int32_t)value * JOY_MAX_CMD) / 2048;

    if (scaled > JOY_MAX_CMD) scaled = JOY_MAX_CMD;
    if (scaled < -JOY_MAX_CMD) scaled = -JOY_MAX_CMD;

    return (int16_t)scaled;
}

void Joystick_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 2);
}

uint16_t Joystick_ReadXRaw(void)
{
    return adc_buffer[0];
}

uint16_t Joystick_ReadYRaw(void)
{
    return adc_buffer[1];
}

int16_t Joystick_GetX(void)
{
    int16_t v = (int16_t)Joystick_ReadXRaw() - JOY_CENTER_X;
    return ScaleValue(ApplyDeadband(v));
}

int16_t Joystick_GetY(void)
{
    int16_t v = (int16_t)Joystick_ReadYRaw() - JOY_CENTER_Y;
    return ScaleValue(ApplyDeadband(v));
}

uint8_t Joystick_ButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(JOYSW_GPIO_Port, JOYSW_Pin) == GPIO_PIN_RESET);
}

uint8_t Joystick_IsMoved(void)
{
    return (Joystick_GetX() != 0 || Joystick_GetY() != 0);
}

void ManualControl_UpdateFromJoystick(void)
{
    int16_t x_cmd = Joystick_GetX();
    int16_t y_cmd = Joystick_GetY();

    Motion_SetManualVelocity(x_cmd, y_cmd, 0, 0);
}

uint8_t ManualLimitReached(void)
{
    return 0;
}