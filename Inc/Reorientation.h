#include "stm32f4xx_hal.h"
#include "main.h"
#ifndef _Reorientation_H
#define _Reorientation_H

void Reorientation_by_quaternion (struct Accel* AccData);
void setReorientationParam(struct Accel const* AccData);
#endif
