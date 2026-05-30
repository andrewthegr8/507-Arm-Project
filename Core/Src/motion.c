#include "motion.h"
#include "TMC429_Register.h"
#include "math.h"
#include "TMC429.h"
#include <stdint.h>

int microstepStep; //Micro step configuration for all motors



double compute_motor_params(int microstep, int gearboxRatio, int stepsPerRev)
//Call at ruuntime to compute the conversion factor for a motor once.
{
   return (double)gearboxRatio * (double)stepsPerRev * (double)microstep / (2 * M_PI);
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
    int32_t steps = _rads_to_steps(motor->conversion_factor, target_pos_rad); //compute number of steps we need to move
    //Need a signed 24-bit integer for motion planning regster
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Write429Datagram(TMC429_IDX_XTARGET(motor->MotionIC_motorNum), (steps >> 16) & 0xFF, (steps >> 8) & 0xFF, steps & 0xFF); //Write the target position to the TMC429
}

double get_current_pos(motor_config_t *motor) {
    uint8_t fromTMC[3]; //allocate buffer to store response from TMC
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Read429Bytes(TMC429_IDX_XACTUAL(motor->MotionIC_motorNum), (uint8_t *)fromTMC); //Read actual position from the TMC
    int32_t steps = (fromTMC[0] << 16) | (fromTMC[1] << 8) | fromTMC[2]; //Combine the 3 bytes from the TMC into a single 24 bit integer
    double pos_rad = _steps_to_rads(motor->conversion_factor, steps); //Convert the position from steps to radians
    return pos_rad;
}

