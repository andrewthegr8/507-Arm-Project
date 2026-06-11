#include "motion.h"
#include "TMC429_Register.h"
#include "math.h"
#include "TMC429.h"
#include <stdint.h>

/** @brief Pulse divider setting used by the motion controller (TMC429). */
#define PULSE_DIV 3 //Pulse div register value on motion planning chip (default is 3)

/** @brief Ramp divider setting used by the motion controller (TMC429). */
#define RAMP_DIV 7 //Ramp div register value on motion planning chip (default is 7)

/** @brief Clock frequency (Hz) supplied to the motion controller. */
#define F_CLK 8000000 //Frequency for clock input for MC

/**
 * @brief Compute conversion parameters used for a motor instance.
 *
 * Call at runtime once per motor to compute the conversions between
 * radians and controller step units, and to populate acceleration/velocity
 * conversion factors stored in @p motor.
 *
 * @param motor Pointer to the motor configuration to populate.
 * @param microstep Microstepping multiplier (e.g. 1, 2, 4, 8, ...).
 * @param gearboxRatio Gearbox ratio (motor revolutions per output revolution).
 * @param stepsPerRev Steps per motor revolution (full steps, e.g. 200).
 */
void compute_motor_params(motor_config_t * motor, int microstep, int gearboxRatio, int stepsPerRev)
//Call at ruuntime to compute the conversion factor for a motor once.
{
   motor->rad_to_steps = (double)gearboxRatio * (double)stepsPerRev * (double)microstep / (2 * M_PI);
   // = Steps/s * 2^Pulse_div * 2048 * 32 / f_clk
   motor->steps_sec_to_IC_units = (1UL << PULSE_DIV) * 2048.0 * 32.0 / (double)F_CLK;
   motor->accel_to_IC_units = ldexp(2, PULSE_DIV + RAMP_DIV + 29) / ((double)F_CLK * F_CLK);
}

/**
 * @brief Convert radians to controller step units (rounded to int).
 *
 * @param motor_conversion_factor Factor that converts radians -> steps.
 * @param rads Angle in radians to convert.
 * @return Integer step count representing the given angle.
 */
static int _rads_to_steps(double motor_conversion_factor, double rads) {
    //Convert radians to steps (or microsteps)
    return (int)round(motor_conversion_factor * rads);
}

/**
 * @brief Convert controller step units back to radians.
 *
 * @param motor_conversion_factor Factor that converts radians -> steps.
 * @param steps Step count from controller registers.
 * @return Angle in radians.
 */
static double _steps_to_rads(double motor_conversion_factor, int steps) {
    //Convert steps (or microsteps) to radians
    return (double)steps / motor_conversion_factor;
}

/**
 * @brief Command a motor to move to an absolute joint position (radians).
 *
 * Converts @p target_pos_rad into controller step units and writes the
 * XTARGET register for the motor channel on the selected TMC429.
 *
 * @param motor Pointer to the motor configuration.
 * @param target_pos_rad Target joint position in radians.
 */
void move_to_pos(motor_config_t *motor, double target_pos_rad) {
    int32_t steps = _rads_to_steps(motor->rad_to_steps, target_pos_rad); //compute number of steps we need to move
    //Need a signed 24-bit integer for motion planning regster
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Write429Datagram(TMC429_IDX_XTARGET(motor->MotionIC_motorNum), (steps >> 16) & 0xFF, (steps >> 8) & 0xFF, steps & 0xFF); //Write the target position to the TMC429
}

/**
 * @brief Command a motor to move at a target angular velocity (rad/s).
 *
 * Converts the requested angular velocity to controller internal units and
 * writes the VTARGET register for the given motor channel.
 *
 * @param motor Pointer to the motor configuration.
 * @param target_velo_rad_s Target angular velocity in radians per second.
 */
void move_at_velocity(motor_config_t *motor, double target_velo_rad_s) {
    int32_t steps = _rads_to_steps(motor->rad_to_steps, target_velo_rad_s); //convert rads to steps
    int32_t velocity = (int32_t)round(steps * motor->steps_sec_to_IC_units); //Convert steps/s to TMC429 velocity register units
    //Need a signed 24-bit integer for motion planning register
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Write429Datagram(TMC429_IDX_VTARGET(motor->MotionIC_motorNum), (velocity >> 16) & 0xFF, (velocity >> 8) & 0xFF, velocity & 0xFF); //Write the target velocity to the TMC429
}

/**
 * @brief Set the motor's maximum acceleration (rad/s^2) in the controller.
 *
 * Converts @p max_accel_rad_s2 into the controller's acceleration register
 * units and writes the AMax register for the motor channel.
 *
 * @param motor Pointer to the motor configuration.
 * @param max_accel_rad_s2 Maximum acceleration in radians per second squared.
 */
void set_max_accel(motor_config_t *motor, double max_accel_rad_s2) {
    //Set max acceleration for a single motor
    int32_t accel = (int32_t)round(max_accel_rad_s2*motor->rad_to_steps * motor->accel_to_IC_units);
    SelectMotionIC(motor->motionIC);
    SetAMax(motor->MotionIC_motorNum, accel); //Set max acceleration higher so we can reach target velocity faster
}

/**
 * @brief Read the current motor position from the controller and return radians.
 *
 * Reads the 24-bit signed XACTUAL register from the TMC429 and converts it
 * to radians using the motor's `rad_to_steps` factor.
 *
 * @param motor Pointer to the motor configuration.
 * @return Current position in radians.
 */
double get_current_pos(motor_config_t *motor) {
    uint8_t fromTMC[3]; //allocate buffer to store response from TMC
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Read429Bytes(TMC429_IDX_XACTUAL(motor->MotionIC_motorNum), (uint8_t *)fromTMC); //Read actual position from the TMC
    int32_t steps = ((int32_t)fromTMC[0] << 16) | ((int32_t)fromTMC[1] << 8) | ((int32_t)fromTMC[2]); //Combine the 3 bytes from the TMC into a single 24 bit integer
    if (steps & 0x00800000) { steps |= 0xFF000000;}     // Sign-extend 24-bit signed value to 32-bit signed value
    double pos_rad = _steps_to_rads(motor->rad_to_steps, steps); //Convert the position from steps to radians
    return pos_rad;
}

/**
 * @brief Read the current motor velocity from the controller and return rad/s.
 *
 * Reads the 24-bit signed VACTUAL register and converts to radians/sec.
 *
 * @param motor Pointer to the motor configuration.
 * @return Current angular velocity in radians per second.
 */
double get_current_velocity(motor_config_t *motor) {
    uint8_t fromTMC[3]; //allocate buffer to store response from TMC
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Read429Bytes(TMC429_IDX_VACTUAL(motor->MotionIC_motorNum), (uint8_t *)fromTMC); //Read actual velocity from the TMC
    int32_t steps = ((int32_t)fromTMC[0] << 16) | ((int32_t)fromTMC[1] << 8) | ((int32_t)fromTMC[2]); //Combine the 3 bytes from the TMC into a single 24 bit integer
    if (steps & 0x00800000) { steps |= 0xFF000000;}     // Sign-extend 24-bit signed value to 32-bit signed value
    double vel_rad_per_sec = _steps_to_rads(motor->rad_to_steps, steps)*1.0/motor->steps_sec_to_IC_units; //Convert the velocity from steps to radians per second
    return vel_rad_per_sec;
}

/**
 * @brief Zero the controller position registers for all motors in the array.
 *
 * Useful for homing procedures where the current physical position should be
 * declared as the zero reference. Temporarily switches the controller to
 * velocity mode to allow direct modification of the XACTUAL register.
 *
 * @param motorConfigs Array of motor configurations.
 * @param num_motors Number of entries in `motorConfigs`.
 */
void zero_motors(motor_config_t *motorConfigs, int num_motors) {
    //Function to set current position of all motors to be the zero position. This can be useful for homing at the beginning of the program.
    for (int i = 0; i < num_motors; i++) {
        SelectMotionIC(motorConfigs[i].motionIC); //Select the correct TMC429 chip
        Set429RampMode(motorConfigs[i].MotionIC_motorNum, TMC429_RM_VELOCITY); //Set velocity mode so we can change xactual register
        Write429Datagram(TMC429_IDX_XACTUAL(motorConfigs[i].MotionIC_motorNum), 0x00, 0x00, 0x00); //Set XACTUAL register to zero
        Write429Datagram(TMC429_IDX_XTARGET(motorConfigs[i].MotionIC_motorNum), 0x00, 0x00, 0x00); //Set XTARGET register to zero
        Set429RampMode(motorConfigs[i].MotionIC_motorNum, TMC429_RM_RAMP); //Set back to position mode
    }
}

