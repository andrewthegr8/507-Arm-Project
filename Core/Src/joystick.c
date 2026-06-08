#include "joystick.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;

void Joystick_Init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
}

static uint16_t read_channel(uint32_t channel)
{
    HAL_ADC_Stop(&hadc1);
    ADC_ChannelConfTypeDef cfg = {0};
    cfg.Channel      = channel;
    cfg.Rank         = ADC_REGULAR_RANK_1;
    cfg.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &cfg);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    uint16_t val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return val;
}

uint16_t Joystick_ReadX(void)
{
    return read_channel(ADC_CHANNEL_17);
}

uint16_t Joystick_ReadY(void)
{
    return read_channel(ADC_CHANNEL_16);
}

uint8_t Joystick_ReadButton(void)
{
    return (HAL_GPIO_ReadPin(SEL_GPIO_Port, SEL_Pin) == GPIO_PIN_RESET);
}