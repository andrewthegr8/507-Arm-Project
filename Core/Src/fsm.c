#include "fsm.h"
#include "color_sensor.h"
#include "maneuvers.h"
#include "main.h"
#include "joystick.h"
#include "motion.h"

#define M1MIN -M_PI/2 //Minimum position for motor 1 in radians.
#define M1MAX (M_PI/2-0) //Maximum position for motor 1 in radians.
#define M2MIN 0.1 //Minimum position for motor 2 in radians.
#define M2MAX M_PI/2 //Maximum position for motor 2 in radians.

extern tcs34725_handle_t sensor_handle;
extern motor_config_t motorConfigs[4];
extern trajectory_t red_trajectory;
extern trajectory_t green_trajectory;
extern trajectory_t yellow_trajectory;
extern trajectory_t blue_trajectory;
extern trajectory_t initial_pose;

static FSM_State current_state;
static ColorResult_t detected_color;

double newpos; //Variable to store new position for joystick control

/**
 * @brief Initialize FSM state and detected color value.
 */
void FSM_Init(void)
{
    current_state = STATE_INIT;
    detected_color = COLOR_UNKNOWN;
}

/**
 * @brief Main FSM update function. Handles state transitions and actions.
 *
 * This should be called regularly from the main loop.
 */
void FSM_Update(void)
{
    switch (current_state)
    {
        case STATE_INIT:
            execute_trajectory(motorConfigs, &initial_pose);
            current_state = STATE_JOYSTICK;
            break;

        case STATE_JOYSTICK:
            Joystick_Read();

            switch (Joystick_ReadX()) {
                case 1: 
                    newpos = get_current_pos(Motor1) - 0.1;
                    if (newpos < M1MIN) {
                        newpos = M1MIN;
                    }   
                    move_to_pos(Motor1, newpos);
                    break;
                case 2: 
                newpos = get_current_pos(Motor1) + 0.1;
                    if (newpos > M1MAX) {
                        newpos = M1MAX;
                    }   
                    move_to_pos(Motor1, newpos);
                    break;
                default: break;
            }

            switch (Joystick_ReadY()) {
                case 1:
                newpos = get_current_pos(Motor2) - 0.1;
                    if (newpos < M2MIN) {
                        newpos = M2MIN;
                    }   
                    move_to_pos(Motor2, newpos);
                    break;
                case 2: 
                newpos = get_current_pos(Motor2) + 0.1;
                    if (newpos > M2MAX) {
                        newpos = M2MAX;
                    }   
                    move_to_pos(Motor2, newpos);
                    break;
                default: break;
            }

            if (Joystick_ReadButton())
                current_state = STATE_GETCOLOR;
            break;

        case STATE_GETCOLOR:
            detected_color = COLOR_SENSOR_Read(&sensor_handle);
            if (detected_color != COLOR_UNKNOWN)
                current_state = STATE_MOVEBLOCK;
            break;

        case STATE_MOVEBLOCK:
            double pos1 = get_current_pos(Motor1);
            double pos2 = get_current_pos(Motor2);
            switch (detected_color) {
                case COLOR_RED:    
                red_trajectory.waypoints[0].joint1 = pos1;
                red_trajectory.waypoints[1].joint1 = pos1;
                red_trajectory.waypoints[0].joint2 = pos2;
                execute_trajectory(motorConfigs, &red_trajectory);
                break;
                case COLOR_YELLOW:
                yellow_trajectory.waypoints[0].joint1 = pos1;
                yellow_trajectory.waypoints[1].joint1 = pos1;
                yellow_trajectory.waypoints[0].joint2 = pos2;
                execute_trajectory(motorConfigs, &yellow_trajectory);
                break;
                case COLOR_GREEN:
                green_trajectory.waypoints[0].joint1 = pos1;
                green_trajectory.waypoints[1].joint1 = pos1;
                green_trajectory.waypoints[0].joint2 = pos2;
                execute_trajectory(motorConfigs, &green_trajectory);
                break;
                case COLOR_BLUE:
                blue_trajectory.waypoints[0].joint1 = pos1;
                blue_trajectory.waypoints[1].joint1 = pos1;
                blue_trajectory.waypoints[0].joint2 = pos2;
                execute_trajectory(motorConfigs, &blue_trajectory);
                break;
                default: break;
            }
            current_state = STATE_INIT;
            break;

        default:
            current_state = STATE_INIT;
            break;
    }
}