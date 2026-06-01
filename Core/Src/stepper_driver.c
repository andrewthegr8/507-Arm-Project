#include "stepper_driver.h"

static StepperDriverConfig_t *stepperConfig;

void StepperDriver_Init(StepperDriverConfig_t *config)
{
    stepperConfig = config;

    // Put drivers in a known disabled/sleeping state
    StepperDriver_Disable();
    StepperDriver_Sleep();

    // Release all reset pins
    for (uint8_t i = 0; i < 4; i++)
    {
        HAL_GPIO_WritePin(
            stepperConfig->drivers[i].resetPort,
            stepperConfig->drivers[i].resetPin,
            GPIO_PIN_SET
        );
    }
}

void StepperDriver_Enable(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->enablePort,
        stepperConfig->enablePin,
        GPIO_PIN_SET
    );
}

void StepperDriver_Disable(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->enablePort,
        stepperConfig->enablePin,
        GPIO_PIN_RESET
    );
}

void StepperDriver_Wake(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->sleepPort,
        stepperConfig->sleepPin,
        GPIO_PIN_SET
    );
}

void StepperDriver_Sleep(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->sleepPort,
        stepperConfig->sleepPin,
        GPIO_PIN_RESET
    );
}

void StepperDriver_Reset(StepperID_t motor)
//Warning: Contains a 1 ms delay
{
    HAL_GPIO_WritePin(
        stepperConfig->drivers[motor].resetPort,
        stepperConfig->drivers[motor].resetPin,
        GPIO_PIN_RESET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        stepperConfig->drivers[motor].resetPort,
        stepperConfig->drivers[motor].resetPin,
        GPIO_PIN_SET
    );
}

void StepperDriver_ResetAll(void)
//Warning: Contains a 1 ms delay
{
    for (uint8_t i = 0; i < 4; i++)
    {
        HAL_GPIO_WritePin(
            stepperConfig->drivers[i].resetPort,
            stepperConfig->drivers[i].resetPin,
            GPIO_PIN_SET        );
    }

    HAL_Delay(1);

    for (uint8_t i = 0; i < 4; i++)
    {
        HAL_GPIO_WritePin(
            stepperConfig->drivers[i].resetPort,
            stepperConfig->drivers[i].resetPin,
            GPIO_PIN_RESET
        );
    }
}

void StepperDriver_SetDecay(GPIO_PinState decay1, GPIO_PinState decay2)
{
    HAL_GPIO_WritePin(stepperConfig->decay1Port, stepperConfig->decay1Pin, decay1);
    HAL_GPIO_WritePin(stepperConfig->decay2Port, stepperConfig->decay2Pin, decay2);
}