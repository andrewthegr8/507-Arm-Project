#ifndef FSM_H
#define FSM_H

typedef enum {
    STATE_INIT = 0,
    STATE_JOYSTICK,
    STATE_GETCOLOR,
    STATE_MOVEBLOCK,
} FSM_State;

/**
 * @brief Initialize the finite state machine for the arm controller.
 */
void FSM_Init(void);

/**
 * @brief Periodic update tick for the FSM. Call from main loop or scheduler.
 */
void FSM_Update(void);

#endif