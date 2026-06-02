#include "motion.h"
#include "TMC429_Register.h"
#include "math.h"
#include "TMC429.h"
#include <stdint.h>

extern motor_config_t motorConfigs[4];

#define PULSE_DIV 3 //Pulse div register value on motion planning chip (default is 3)
#define F_CLK 8000000 //Frequency for clock input for MC

void compute_motor_params(motor_config_t * motor, int microstep, int gearboxRatio, int stepsPerRev)
//Call at ruuntime to compute the conversion factor for a motor once.
{
   motor->rad_to_steps = (double)gearboxRatio * (double)stepsPerRev * (double)microstep / (2 * M_PI);
   // = Steps/s * 2^Pulse_div * 2048 * 32 / f_clk
   motor->steps_sec_to_IC_units = (1UL << PULSE_DIV) * 2048.0 * 32.0 / F_CLK;
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


double get_current_pos(motor_config_t *motor) {
    uint8_t fromTMC[3]; //allocate buffer to store response from TMC
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Read429Bytes(TMC429_IDX_XACTUAL(motor->MotionIC_motorNum), (uint8_t *)fromTMC); //Read actual position from the TMC
    int32_t steps = (fromTMC[0] << 16) | (fromTMC[1] << 8) | fromTMC[2]; //Combine the 3 bytes from the TMC into a single 24 bit integer
    double pos_rad = _steps_to_rads(motor->rad_to_steps, steps); //Convert the position from steps to radians
    return pos_rad;
}

void StopAllMotion(void)
{
    move_to_pos(&motorConfigs[1], get_current_pos(&motorConfigs[1]));
    move_to_pos(&motorConfigs[2], get_current_pos(&motorConfigs[2]));
}

void Motor2_RunPositive(void)
{
    move_to_pos(&motorConfigs[1], get_current_pos(&motorConfigs[1]) + 0.1);
}

void Motor2_RunNegative(void)
{
    move_to_pos(&motorConfigs[1], get_current_pos(&motorConfigs[1]) - 0.1);
}

void Motor2_Stop(void)
{
    move_to_pos(&motorConfigs[1], get_current_pos(&motorConfigs[1]));
}

void Motor3_RunPositive(void)
{
    move_to_pos(&motorConfigs[2], get_current_pos(&motorConfigs[2]) + 0.1);
}

void Motor3_RunNegative(void)
{
    move_to_pos(&motorConfigs[2], get_current_pos(&motorConfigs[2]) - 0.1);
}

void Motor3_Stop(void)
{
    move_to_pos(&motorConfigs[2], get_current_pos(&motorConfigs[2]));
}

uint8_t PlanMoveFromColor(uint8_t color_id, uint8_t *target_bin)
{
    switch (color_id) {
        case 1: *target_bin = 1; return 1; // yellow
        case 2: *target_bin = 2; return 1; // red
        case 3: *target_bin = 3; return 1; // blue
        case 4: *target_bin = 4; return 1; // orange
        default: return 0; // unknown color
    }
}