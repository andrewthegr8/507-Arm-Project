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

/**
 * @file joystick.c
 * @brief ADC-backed joystick driver providing simple directional state.
 */

#include "joystick.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

static uint16_t joy_x_raw = 0;
static uint16_t joy_y_raw = 0;
uint8_t x_dir = 0;
uint8_t y_dir = 0;

/**
 * @brief Read a single ADC channel from the configured ADC instance.
 * @param channel ADC channel identifier (e.g. ADC_CHANNEL_16).
 * @return Raw 12/16-bit ADC sample.
 */
static uint16_t Joystick_ReadADCChannel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    sConfig.OffsetSignedSaturation = DISABLE;

    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        Error_Handler();
    }

    uint16_t value = (uint16_t)HAL_ADC_GetValue(&hadc1);

    HAL_ADC_Stop(&hadc1);

    return value;
}

void Joystick_Init(void)
{
    /**
     * @brief Calibrate ADC used by joystick and perform a short settling delay.
     */
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
    HAL_Delay(10);
}

void Joystick_Read(void)
{
    /**
     * @brief Sample joystick ADC channels and update direction flags.
     */
    joy_y_raw = Joystick_ReadADCChannel(ADC_CHANNEL_16);
    joy_x_raw = Joystick_ReadADCChannel(ADC_CHANNEL_17);
    if (joy_x_raw > 3000) x_dir = 1;
    else if (joy_x_raw < 1000) x_dir = 2;
    else x_dir = 0;

    if (joy_y_raw > 3000) y_dir = 2;
    else if (joy_y_raw < 1000) y_dir = 1;
    else y_dir = 0;

}

uint16_t Joystick_ReadX(void)
{
    return x_dir;
}

uint16_t Joystick_ReadY(void)
{
    return y_dir;
}

uint8_t Joystick_ReadButton(void)
{
    /**
     * @brief Read the joystick push-button (select) state.
     * @return 1 when pressed, 0 when released.
     */
    return (HAL_GPIO_ReadPin(SEL_GPIO_Port, SEL_Pin) == GPIO_PIN_RESET);
}