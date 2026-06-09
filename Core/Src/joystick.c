/* #include "joystick.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

static uint16_t x_val = 0;
static uint16_t y_val = 0;

void Joystick_Init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
}

void Joystick_Read(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    x_val = HAL_ADC_GetValue(&hadc1);  // rank 1, IN16

    HAL_ADC_PollForConversion(&hadc1, 100);
    y_val = HAL_ADC_GetValue(&hadc1);  // rank 2, IN17
    HAL_ADC_Stop(&hadc1);
}

uint16_t Joystick_ReadX(void) { return x_val; }
uint16_t Joystick_ReadY(void) { return y_val; }

uint8_t Joystick_ReadButton(void)
{
    return (HAL_GPIO_ReadPin(SEL_GPIO_Port, SEL_Pin) == GPIO_PIN_RESET);
} */

#include "joystick.h"
#include "main.h"

extern volatile uint16_t joy_x_raw;
extern volatile uint16_t joy_y_raw;

void Joystick_Init(void) {}

uint16_t Joystick_ReadX(void) { return joy_x_raw; }
uint16_t Joystick_ReadY(void) { return joy_y_raw; }

uint8_t Joystick_ReadButton(void)
{
    return (HAL_GPIO_ReadPin(SEL_GPIO_Port, SEL_Pin) == GPIO_PIN_RESET);
}