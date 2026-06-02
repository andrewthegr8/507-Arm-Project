#ifndef FSM_H
#define FSM_H

#include <stdint.h>

typedef enum {
    S0_IDLE = 0,
    S1_MANUAL_APPROACH,
    S2_CLOSE_GRIPPER,
    S3_READ_COLOR,
    S4_PLOT_MOVE,
    S5_MOVE_BLOCK,
    S6_RELEASE_BLOCK,
    S7_RETURN_HOME
} RobotState;

typedef struct {
    RobotState state;
    RobotState prev_state;

    uint8_t state_entry;
    uint32_t state_start_ms;

    uint8_t color_id;
    uint8_t target_bin;

    uint8_t grip_ok;
    uint8_t move_plan_ready;
} RobotContext;

void FSM_Init(void);
void FSM_Update(void);
void FSM_SetState(RobotState new_state);
RobotState FSM_GetState(void);

#endif