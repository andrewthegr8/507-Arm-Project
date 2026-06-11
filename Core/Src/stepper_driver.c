#include "stepper_driver.h"

static StepperDriverConfig_t *stepperConfig;

/**
 * @brief Initialize stepper driver control pins and put drivers into a safe state.
 * @param config Pointer to configuration describing GPIO pins for drivers.
 */
void StepperDriver_Init(StepperDriverConfig_t *config)
{
    stepperConfig = config;

    /* Put drivers in a known disabled/sleeping state */
    StepperDriver_Disable();
    StepperDriver_Sleep();

    /* Release all reset pins */
    for (uint8_t i = 0; i < 4; i++)
    {
        HAL_GPIO_WritePin(
            stepperConfig->drivers[i].resetPort,
            stepperConfig->drivers[i].resetPin,
            GPIO_PIN_SET
        );
    }
}

/**
 * @brief Assert the driver's enable pin to enable outputs.
 */
void StepperDriver_Enable(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->enablePort,
        stepperConfig->enablePin,
        GPIO_PIN_SET
    );
}

/**
 * @brief Deassert the driver's enable pin to disable outputs.
 */
void StepperDriver_Disable(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->enablePort,
        stepperConfig->enablePin,
        GPIO_PIN_RESET
    );
}

/**
 * @brief Wake stepper drivers by asserting the sleep pin.
 */
void StepperDriver_Wake(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->sleepPort,
        stepperConfig->sleepPin,
        GPIO_PIN_SET
    );
}

/**
 * @brief Put stepper drivers to sleep by deasserting the sleep pin.
 */
void StepperDriver_Sleep(void)
{
    HAL_GPIO_WritePin(
        stepperConfig->sleepPort,
        stepperConfig->sleepPin,
        GPIO_PIN_RESET
    );
}

/**
 * @brief Reset a single stepper driver (pulse reset line).
 * @param motor Motor index (StepperID_t) to reset.
 * @note Contains a 1 ms delay to satisfy reset timing requirements.
 */
void StepperDriver_Reset(StepperID_t motor)
//Warning: Contains a 1 ms delay
{
    HAL_GPIO_WritePin(
        stepperConfig->drivers[motor].resetPort,
        stepperConfig->drivers[motor].resetPin,
        GPIO_PIN_SET
    );

    HAL_Delay(1);

    HAL_GPIO_WritePin(
        stepperConfig->drivers[motor].resetPort,
        stepperConfig->drivers[motor].resetPin,
        GPIO_PIN_RESET
    );
}

/**
 * @brief Reset all stepper driver reset lines.
 * @note Contains a 1 ms delay.
 */
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

/**
 * @brief Configure decay selection pins.
 * @param decay1 Pin state for decay1.
 * @param decay2 Pin state for decay2.
 */
void StepperDriver_SetDecay(GPIO_PinState decay1, GPIO_PinState decay2)
{
    HAL_GPIO_WritePin(stepperConfig->decay1Port, stepperConfig->decay1Pin, decay1);
    HAL_GPIO_WritePin(stepperConfig->decay2Port, stepperConfig->decay2Pin, decay2);
}