#include "joystick.h"
#include "main.h"
#include "motion.h"
#include <stdlib.h>
#include <stdio.h>
extern UART_HandleTypeDef huart3;
#define JOY_CENTER_X        32768
#define JOY_CENTER_Y        32768
#define JOY_DEADBAND        4000
#define JOY_RELEASE_BAND    4000

extern ADC_HandleTypeDef hadc1;

static JoystickAxis locked_axis = JOY_AXIS_NONE;

void Joystick_Init(void)
{
    HAL_ADC_Start(&hadc1);
    locked_axis = JOY_AXIS_NONE;
}

uint16_t Joystick_ReadXRaw(void)
{
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}

uint16_t Joystick_ReadYRaw(void)
{
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
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
    return (HAL_GPIO_ReadPin(SEL_GPIO_Port, SEL_Pin) == GPIO_PIN_RESET);
}
uint8_t Joystick_IsMoved(void)
{
    int16_t x = Joystick_Get_XCentered();
    int16_t y = Joystick_Get_YCentered();
    return (abs(x) > JOY_DEADBAND || abs(y) > JOY_DEADBAND);
}
void Joystick_UpdateManualControl(void)
{
    int16_t x = Joystick_Get_XCentered();
    int16_t y = Joystick_Get_YCentered();

    //JOYSTICK TESTING 
    char dbg[100];
    int len = sprintf(dbg, "X: %d  Y: %d  BTN: %d\r\n", x, y, Joystick_ButtonPressed());
    HAL_UART_Transmit(&huart3, (uint8_t *)dbg, len, HAL_MAX_DELAY);
    //END JOYSTICK TESTING

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
            //Motor3_RunPositive();
            sprintf(dbg, "+x: %d\r\n", x);
            HAL_UART_Transmit(&huart3, (uint8_t *)dbg, strlen(dbg), HAL_MAX_DELAY);
        } else if (x < -JOY_DEADBAND) {
            //Motor3_RunNegative();
            sprintf(dbg, "-x: %d\r\n", x);
            HAL_UART_Transmit(&huart3, (uint8_t *)dbg, strlen(dbg), HAL_MAX_DELAY);
        } else {
            Motor3_Stop();
        }
    }
    else if (locked_axis == JOY_AXIS_Y) {
        Motor3_Stop();

        if (y > JOY_DEADBAND) {
            //Motor2_RunPositive();
            sprintf(dbg, "+y: %d\r\n", y);
            HAL_UART_Transmit(&huart3, (uint8_t *)dbg, strlen(dbg), HAL_MAX_DELAY);
        } else if (y < -JOY_DEADBAND) {
            //Motor2_RunNegative();
            sprintf(dbg, "-y: %d\r\n", y);
            HAL_UART_Transmit(&huart3, (uint8_t *)dbg, strlen(dbg), HAL_MAX_DELAY);
        } else {
            Motor2_Stop();
        }
    }
}