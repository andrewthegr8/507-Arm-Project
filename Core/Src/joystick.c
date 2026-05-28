#include "joystick.h"
#include "main.h"
#include "motion.h"
#include <stdlib.h>

#define JOY_CENTER_X        2048
#define JOY_CENTER_Y        2048
#define JOY_DEADBAND        300

// Dominance ratio threshold:
// dominant axis must be at least this much stronger than the other
#define DOMINANCE_NUM       3
#define DOMINANCE_DEN       2   // 3/2 = 1.5

extern ADC_HandleTypeDef hadc1;
extern uint16_t adc_buffer[2];

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

int16_t Joystick_GetXCentered(void)
{
    return (int16_t)Joystick_ReadXRaw() - JOY_CENTER_X;
}

int16_t Joystick_GetYCentered(void)
{
    return (int16_t)Joystick_ReadYRaw() - JOY_CENTER_Y;
}

uint8_t Joystick_ButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(JOYSW_GPIO_Port, JOYSW_Pin) == GPIO_PIN_RESET);
}

void Joystick_UpdateManualControl(void)
{
    int16_t x = Joystick_GetXCentered();
    int16_t y = Joystick_GetYCentered();

    int16_t abs_x = abs(x);
    int16_t abs_y = abs(y);

    if (Joystick_ButtonPressed()) {
        StopAllMotion();
        return;
    }

    // Both near center: stop both motors
    if (abs_x < JOY_DEADBAND && abs_y < JOY_DEADBAND) {
        StopAllMotion();
        return;
    }

    // X is dominant: control only motor 3
    if ((abs_x >= JOY_DEADBAND) &&
        (abs_x * DOMINANCE_DEN > abs_y * DOMINANCE_NUM)) {

        Motor2_Stop();

        if (x > 0) {
            Motor3_RunPositive();
        } else {
            Motor3_RunNegative();
        }
        return;
    }

    // Y is dominant: control only motor 2
    if ((abs_y >= JOY_DEADBAND) &&
        (abs_y * DOMINANCE_DEN > abs_x * DOMINANCE_NUM)) {

        Motor3_Stop();

        if (y > 0) {
            Motor2_RunPositive();
        } else {
            Motor2_RunNegative();
        }
        return;
    }

    // If neither axis is clearly dominant, ignore both
    StopAllMotion();
}