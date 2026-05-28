#include "motion.h"
#include "math.h"
#include "TMC429.h"

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

void move_to_pos(motor_config_t *motor, double target_pos_rad) {
    int32_t steps = _rads_to_steps(motor->conversion_factor, target_pos_rad); //compute number of steps we need to move
    //Need a signed 24-bit integer for motion planning regster
    SelectMotionIC(motor->motionIC); //Select the correct TMC429 chip
    Write429Datagram(TMC429_IDX_XTARGET(motor->MotionIC_motorNum), (steps >> 16) & 0xFF, (steps >> 8) & 0xFF, steps & 0xFF); //Write the target position to the TMC429
}

