#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>
#include "TMC429.h"

/**
 * @addtogroup MotionControl
 * @{
 */

typedef struct {
    double rad_to_steps; //Steps (or microsteps) per radian
    double steps_sec_to_IC_units; //Steps (or microsteps) per second per rad/s
    double accel_to_IC_units; //Acceleration in TMC429 units per rad/s^2
    MotionIC_t motionIC; //Which TMC429 chip the motor is connected to
    uint8_t MotionIC_motorNum; //Which axis of the TMC429 the motor is connected to (0, 1 or 2)
} motor_config_t;

void compute_motor_params(motor_config_t * motor, int microstep, int gearboxRatio, int stepsPerRev);
double get_current_pos(motor_config_t *motor);
void move_to_pos(motor_config_t *motor, double target_pos_rad);
void move_at_velocity(motor_config_t *motor, double target_vel_rad_per_sec);
void set_max_accel(motor_config_t *motor, double max_accel_rad_s2);
void zero_motors(motor_config_t *motorConfigs, int num_motors);




void Motion_Init(void);
void Motion_StartMoveToBin(uint8_t bin);
void Motion_StartMoveHome(void);
uint8_t Motion_IsDone(void);
void StopAllMotion(void);

void Motor2_RunPositive(void);
void Motor2_RunNegative(void);
void Motor2_Stop(void);
void Motor3_RunPositive(void);
void Motor3_RunNegative(void);
void Motor3_Stop(void);
uint8_t PlanMoveFromColor(uint8_t color_id, uint8_t *target_bin);

/** @} */

#endif