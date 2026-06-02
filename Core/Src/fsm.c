#include "fsm.h"
#include "main.h"
#include "joystick.h"
#include "servo.h"
#include "stepper.h"
#include "color_sensor.h"
#include "motion.h"

static RobotContext robot;
static tcs34725_handle_t sensor_handle;

void FSM_SetState(RobotState new_state)
{
    robot.prev_state = robot.state;
    robot.state = new_state;
    robot.state_entry = 1;
    robot.state_start_ms = HAL_GetTick();
}

RobotState FSM_GetState(void)
{
    return robot.state;
}

void FSM_Init(void)
{
    robot.state = S0_IDLE;
    robot.prev_state = S0_IDLE;
    robot.state_entry = 1;
    robot.state_start_ms = HAL_GetTick();

    robot.color_id = 0;
    robot.target_bin = 0;
    robot.grip_ok = 0;
    robot.move_plan_ready = 0;

    memset(&sensor_handle, 0, sizeof(tcs34725_handle_t));
    sensor_handle.iic_init    = my_i2c_init;
    sensor_handle.iic_deinit  = my_i2c_deinit;
    sensor_handle.iic_read    = my_i2c_read;
    sensor_handle.iic_write   = my_i2c_write;
    sensor_handle.delay_ms    = my_delay_ms;
    sensor_handle.debug_print = my_debug_print;
    tcs34725_init(&sensor_handle);
    COLOR_SENSOR_Init(&sensor_handle);
}

void FSM_Update(void)
{
    switch (robot.state) {

    case S0_IDLE:
        if (robot.state_entry) {
            robot.state_entry = 0;
            Claw_Open();
            StopAllMotion();
            robot.grip_ok = 0;
            robot.move_plan_ready = 0;
        }

        if (Joystick_IsMoved()) {
            FSM_SetState(S1_MANUAL_APPROACH);
        }
        break;

    case S1_MANUAL_APPROACH:
        if (robot.state_entry) {
            robot.state_entry = 0;
        }

        ManualControl_UpdateFromJoystick();

        if (Joystick_ButtonPressed()) {
            StopAllMotion();
            FSM_SetState(S2_CLOSE_GRIPPER);
        }

        if (ManualLimitReached()) {
            StopAllMotion();
            FSM_SetState(S0_IDLE);
        }
        break;

    case S2_CLOSE_GRIPPER:
        if (robot.state_entry) {
            robot.state_entry = 0;
            Claw_Close();
        }

        if ((HAL_GetTick() - robot.state_start_ms) > 500U) {
            robot.grip_ok = 1;
            FSM_SetState(S3_READ_COLOR);
        }
        break;

    case S3_READ_COLOR:
        if (robot.state_entry) {
            robot.state_entry = 0;
        }

        robot.color_id = (uint8_t)COLOR_SENSOR_Read(&sensor_handle);
        if (robot.color_id != COLOR_UNKNOWN) {
            FSM_SetState(S4_PLOT_MOVE);
        } else {
            Claw_Open();
            FSM_SetState(S7_RETURN_HOME);
        }
        break;

    case S4_PLOT_MOVE:
        if (robot.state_entry) {
            robot.state_entry = 0;
            robot.move_plan_ready = PlanMoveFromColor(robot.color_id, &robot.target_bin);
        }

        if (robot.move_plan_ready) {
            FSM_SetState(S5_MOVE_BLOCK);
        } else {
            Claw_Open();
            FSM_SetState(S7_RETURN_HOME);
        }
        break;

    case S5_MOVE_BLOCK:
        if (robot.state_entry) {
            robot.state_entry = 0;
            Motion_StartMoveToBin(robot.target_bin);
        }

        if (Motion_IsDone()) {
            FSM_SetState(S6_RELEASE_BLOCK);
        }

        if ((HAL_GetTick() - robot.state_start_ms) > 5000U) {
            StopAllMotion();
            Claw_Open();
            FSM_SetState(S7_RETURN_HOME);
        }
        break;

    case S6_RELEASE_BLOCK:
        if (robot.state_entry) {
            robot.state_entry = 0;
            Claw_Open();
        }

        if ((HAL_GetTick() - robot.state_start_ms) > 500U) {
            FSM_SetState(S7_RETURN_HOME);
        }
        break;

    case S7_RETURN_HOME:
        if (robot.state_entry) {
            robot.state_entry = 0;
            Motion_StartMoveHome();
        }

        if (Motion_IsDone()) {
            FSM_SetState(S0_IDLE);
        }

        if ((HAL_GetTick() - robot.state_start_ms) > 5000U) {
            StopAllMotion();
            FSM_SetState(S0_IDLE);
        }
        break;

    default:
        StopAllMotion();
        FSM_SetState(S0_IDLE);
        break;
    }
}