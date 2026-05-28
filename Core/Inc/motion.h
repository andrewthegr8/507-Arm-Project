#ifndef MOTION_H
#define MOTION_H

#include <stdint.h>

void Motion_Init(void);
void Motion_StartMoveToBin(uint8_t bin);
void Motion_StartMoveHome(void);
uint8_t Motion_IsDone(void);
void StopAllMotion(void);

#endif