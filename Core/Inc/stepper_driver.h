#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H
//This is all chat generated so use with caution.

#include "main.h"
#include <stdint.h>

/**
 * @addtogroup MotionControl
 * @{
 */

//Motor drivers all share the same sleep, enable and decay pins, but each has its own reset pin
typedef enum
{
    STEPPER_1 = 0,
    STEPPER_2,
    STEPPER_3,
    STEPPER_4
} StepperID_t;

typedef struct
{
    GPIO_TypeDef *resetPort;
    uint16_t resetPin;
} StepperDriver_t;

typedef struct
{
    GPIO_TypeDef *sleepPort;
    uint16_t sleepPin;

    GPIO_TypeDef *enablePort;
    uint16_t enablePin;

    GPIO_TypeDef *decay1Port;
    uint16_t decay1Pin;

    GPIO_TypeDef *decay2Port;
    uint16_t decay2Pin;

    StepperDriver_t drivers[4];
} StepperDriverConfig_t;

void StepperDriver_Init(StepperDriverConfig_t *config);

void StepperDriver_Enable(void);
void StepperDriver_Disable(void);

void StepperDriver_Wake(void);
void StepperDriver_Sleep(void);

void StepperDriver_Reset(StepperID_t motor);
void StepperDriver_ResetAll(void);

void StepperDriver_SetDecay(GPIO_PinState decay1, GPIO_PinState decay2);

/** @} */

#endif