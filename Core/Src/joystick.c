#include "joystick.h"
#include "main.h"
#include "motion.h"
#include <stdlib.h>

#define JOY_CENTER_X        2048
#define JOY_CENTER_Y        2048
#define JOY_DEADBAND        250
#define JOY_RELEASE_BAND    250

extern ADC_HandleTypeDef hadc1;
extern uint16_t adc_buffer[2];

static JoystickAxis locked_axis = JOY_AXIS_NONE;

void Joystick_Init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 2);
    locked_axis = JOY_AXIS_NONE;
}

uint16_t Joystick_ReadXRaw(void)
{
    return adc_buffer[0];
}

uint16_t Joystick_ReadYRaw(void)
{
    return adc_buffer[1];
}

int16_t Joystick_Get_XCentered(void)
{
    return (int16_t)Joystick_ReadXRaw() - JOY_CENTER_X;
}

int16_t Joystick_Get_YCentered(void)
{
    return (int16_t)Joystick_ReadYRaw() - JOY_CENTER_Y;
}

uint8_t Joystick_ButtonPressed(void)
{
    return (HAL_GPIO_ReadPin(JOYSW_GPIO_Port, JOYSW_Pin) == GPIO_PIN_RESET);
}

void Joystick_UpdateManualControl(void)
{
    int16_t x = Joystick_Get_XCentered();
    int16_t y = Joystick_Get_YCentered();

    int16_t abs_x = abs(x);
    int16_t abs_y = abs(y);

    if (Joystick_ButtonPressed()) {
        StopAllMotion();
        locked_axis = JOY_AXIS_NONE;
        return;
    }

    // Release lock only when joystick is near center
    if (abs_x < JOY_RELEASE_BAND && abs_y < JOY_RELEASE_BAND) {
        locked_axis = JOY_AXIS_NONE;
        StopAllMotion();
        return;
    }

    // If no axis locked yet, choose the stronger one
    if (locked_axis == JOY_AXIS_NONE) {
        if (abs_x > abs_y && abs_x > JOY_DEADBAND) {
            locked_axis = JOY_AXIS_X;
        } else if (abs_y > abs_x && abs_y > JOY_DEADBAND) {
            locked_axis = JOY_AXIS_Y;
        } else {
            StopAllMotion();
            return;
        }
    }

    if (locked_axis == JOY_AXIS_X) {
        Motor2_Stop();

        if (x > JOY_DEADBAND) {
            Motor3_RunPositive();
        } else if (x < -JOY_DEADBAND) {
            Motor3_RunNegative();
        } else {
            Motor3_Stop();
        }
    }
    else if (locked_axis == JOY_AXIS_Y) {
        Motor3_Stop();

        if (y > JOY_DEADBAND) {
            Motor2_RunPositive();
        } else if (y < -JOY_DEADBAND) {
            Motor2_RunNegative();
        } else {
            Motor2_Stop();
        }
    }
}