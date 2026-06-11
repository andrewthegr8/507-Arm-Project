#include "fsm.h"
#include "color_sensor.h"
#include "maneuvers.h"
#include "main.h"
#include "joystick.h"
#include "motion.h"

extern tcs34725_handle_t sensor_handle;
extern motor_config_t motorConfigs[4];
extern trajectory_t red_trajectory;
extern trajectory_t green_trajectory;
extern trajectory_t yellow_trajectory;
extern trajectory_t blue_trajectory;

static FSM_State current_state;
static ColorResult_t detected_color;

void FSM_Init(void)
{
    current_state = STATE_INIT;
    detected_color = COLOR_UNKNOWN;
}

void FSM_Update(void)
{
    switch (current_state)
    {
        case STATE_INIT:
            current_state = STATE_JOYSTICK;
            break;

        case STATE_JOYSTICK:
            Joystick_Read();

            switch (Joystick_ReadX()) {
                case 1: move_to_pos(Motor1, get_current_pos(Motor1) - 0.1); break;
                case 2: move_to_pos(Motor1, get_current_pos(Motor1) + 0.1); break;
                default: break;
            }

            switch (Joystick_ReadY()) {
                case 1: move_to_pos(Motor2, get_current_pos(Motor2) - 0.1); break;
                case 2: move_to_pos(Motor2, get_current_pos(Motor2) + 0.1); break;
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
            switch (detected_color) {
                case COLOR_RED:    execute_trajectory(motorConfigs, &red_trajectory);    break;
                case COLOR_YELLOW: execute_trajectory(motorConfigs, &yellow_trajectory); break;
                case COLOR_GREEN:  execute_trajectory(motorConfigs, &green_trajectory);  break;
                case COLOR_BLUE:   execute_trajectory(motorConfigs, &blue_trajectory);   break;
                default: break;
            }
            current_state = STATE_INIT;
            break;

        default:
            current_state = STATE_INIT;
            break;
    }
}