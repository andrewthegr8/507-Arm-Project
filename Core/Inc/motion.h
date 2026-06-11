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

/**
 * @brief Compute internal motor conversion parameters based on microstepping and gearing.
 * @param motor Pointer to motor_config_t to populate.
 * @param microstep Microstep setting (e.g. 1,2,4,...).
 * @param gearboxRatio Gearbox ratio applied to the motor.
 * @param stepsPerRev Native full steps per motor revolution.
 */
void compute_motor_params(motor_config_t * motor, int microstep, int gearboxRatio, int stepsPerRev);

/**
 * @brief Get the current motor position in radians.
 * @param motor Pointer to motor_config_t for the motor.
 * @return Motor position in radians.
 */
double get_current_pos(motor_config_t *motor);

/**
 * @brief Move the motor to the specified angular position (blocking or non-blocking depending on implementation).
 * @param motor Pointer to motor_config_t for the motor.
 * @param target_pos_rad Target position in radians.
 */
void move_to_pos(motor_config_t *motor, double target_pos_rad);

/**
 * @brief Command the motor to run at a given angular velocity.
 * @param motor Pointer to motor_config_t for the motor.
 * @param target_vel_rad_per_sec Target angular velocity in rad/s.
 */
void move_at_velocity(motor_config_t *motor, double target_vel_rad_per_sec);

/**
 * @brief Set the maximum acceleration for the motor.
 * @param motor Pointer to motor_config_t for the motor.
 * @param max_accel_rad_s2 Maximum acceleration in rad/s^2.
 */
void set_max_accel(motor_config_t *motor, double max_accel_rad_s2);

/**
 * @brief Zero the motor positions for a set of motors.
 * @param motorConfigs Array of motor_config_t structures.
 * @param num_motors Number of motors in the array.
 */
void zero_motors(motor_config_t *motorConfigs, int num_motors);



/** @name High-level Motion Control Helpers */
/**@{*/
/**
 * @brief Initialize motion subsystem and motion ICs.
 */
void Motion_Init(void);

/**
 * @brief Start a move to a predefined bin index.
 * @param bin Target bin index.
 */
void Motion_StartMoveToBin(uint8_t bin);

/**
 * @brief Start a move to the home position.
 */
void Motion_StartMoveHome(void);

/**
 * @brief Query whether the current motion has completed.
 * @return 1 if done, 0 otherwise.
 */
uint8_t Motion_IsDone(void);

/**
 * @brief Immediately stop all motion.
 */
void StopAllMotion(void);

void Motor2_RunPositive(void);
void Motor2_RunNegative(void);
void Motor2_Stop(void);
void Motor3_RunPositive(void);
void Motor3_RunNegative(void);
void Motor3_Stop(void);
uint8_t PlanMoveFromColor(uint8_t color_id, uint8_t *target_bin);

/**@}*/

/** @} */

#endif