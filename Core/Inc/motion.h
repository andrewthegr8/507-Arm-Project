#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>
#include "TMC429.h"

typedef struct {
    double conversion_factor; //Steps (or microsteps) per radian
    MotionIC_t motionIC; //Which TMC429 chip the motor is connected to
    uint8_t MotionIC_motorNum; //Which axis of the TMC429 the motor is connected to (0, 1 or 2)
} motor_config_t;

double compute_motor_params(int microstep, int gearboxRatio, int stepsPerRev);
double get_current_pos(motor_config_t *motor);
void move_to_pos(motor_config_t *motor, double target_pos_rad);



void Motion_Init(void);
void Motion_StartMoveToBin(uint8_t bin);
void Motion_StartMoveHome(void);
uint8_t Motion_IsDone(void);
void StopAllMotion(void);

#endif