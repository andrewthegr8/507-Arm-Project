#ifndef FSM_H
#define FSM_H

typedef enum {
    STATE_INIT = 0,
    STATE_JOYSTICK,
    STATE_GETCOLOR,
    STATE_MOVEBLOCK,
} FSM_State;

void FSM_Init(void);
void FSM_Update(void);

#endif