#include "motion.h"
#include "TMC429_Register.h"
#include "math.h"
#include "TMC429.h"
#include <stdint.h>

#define PULSE_DIV 3 //Pulse div register value on motion planning chip (default is 3)
#define RAMP_DIV 7 //Ramp div register value on motion planning chip (default is 7)
#define F_CLK 8000000 //Frequency for clock input for MC

void compute_motor_params(motor_config_t * motor, int microstep, int gearboxRatio, int stepsPerRev)
//Call at ruuntime to compute the conversion factor for a motor once.
{
   motor->rad_to_steps = (double)gearboxRatio * (double)stepsPerRev * (double)microstep / (2 * M_PI);
   // = Steps/s * 2^Pulse_div * 2048 * 32 / f_clk
   motor->steps_sec_to_IC_units = (1UL << PULSE_DIV) * 2048.0 * 32.0 / (double)F_CLK;
   motor->accel_to_IC_units = ldexp(2, PULSE_DIV + RAMP_DIV + 29) / ((double)F_CLK * F_CLK);
}

static int _rads_to_steps(double motor_conversion_factor, double rads) {
    //Convert radians to steps (or microsteps)
    return (int)round(motor_conversion_factor * rads);
}

static double _steps_to_rads(double motor_conversion_factor, int steps) {
    //Convert steps (or microsteps) to radians
    return (double)steps / motor_conversion_factor;
}

void move_to_pos(motor_config_t *motor, double target_pos_rad) {
    int32_t steps = _rads_to_steps(motor->rad_to_steps, target_pos_rad); //compute number of steps we need to move
    //Need a signed 24-bit integer for motion planning regster
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Write429Datagram(TMC429_IDX_XTARGET(motor->MotionIC_motorNum), (steps >> 16) & 0xFF, (steps >> 8) & 0xFF, steps & 0xFF); //Write the target position to the TMC429
}

void move_at_velocity(motor_config_t *motor, double target_velo_rad_s) {
    int32_t steps = _rads_to_steps(motor->rad_to_steps, target_velo_rad_s); //convert rads to steps
    int32_t velocity = (int32_t)round(steps * motor->steps_sec_to_IC_units); //Convert steps/s to TMC429 velocity register units
    //Need a signed 24-bit integer for motion planning register
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Write429Datagram(TMC429_IDX_VTARGET(motor->MotionIC_motorNum), (velocity >> 16) & 0xFF, (velocity >> 8) & 0xFF, velocity & 0xFF); //Write the target velocity to the TMC429
}

void set_max_accel(motor_config_t *motor, double max_accel_rad_s2) {
    //Set max acceleration for a single motor
    int32_t accel = (int32_t)round(max_accel_rad_s2*motor->rad_to_steps * motor->accel_to_IC_units);
    SelectMotionIC(motor->motionIC);
    SetAMax(motor->MotionIC_motorNum, accel); //Set max acceleration higher so we can reach target velocity faster
}

double get_current_pos(motor_config_t *motor) {
    uint8_t fromTMC[3]; //allocate buffer to store response from TMC
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Read429Bytes(TMC429_IDX_XACTUAL(motor->MotionIC_motorNum), (uint8_t *)fromTMC); //Read actual position from the TMC
    int32_t steps = ((int32_t)fromTMC[0] << 16) | ((int32_t)fromTMC[1] << 8) | ((int32_t)fromTMC[2]); //Combine the 3 bytes from the TMC into a single 24 bit integer
    if (steps & 0x00800000) { steps |= 0xFF000000;}     // Sign-extend 24-bit signed value to 32-bit signed value
    double pos_rad = _steps_to_rads(motor->rad_to_steps, steps); //Convert the position from steps to radians
    return pos_rad;
}

double get_current_velocity(motor_config_t *motor) {
    uint8_t fromTMC[3]; //allocate buffer to store response from TMC
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Read429Bytes(TMC429_IDX_VACTUAL(motor->MotionIC_motorNum), (uint8_t *)fromTMC); //Read actual velocity from the TMC
    int32_t steps = ((int32_t)fromTMC[0] << 16) | ((int32_t)fromTMC[1] << 8) | ((int32_t)fromTMC[2]); //Combine the 3 bytes from the TMC into a single 24 bit integer
    if (steps & 0x00800000) { steps |= 0xFF000000;}     // Sign-extend 24-bit signed value to 32-bit signed value
    double vel_rad_per_sec = _steps_to_rads(motor->rad_to_steps, steps)*1.0/motor->steps_sec_to_IC_units; //Convert the velocity from steps to radians per second
    return vel_rad_per_sec;
}

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

