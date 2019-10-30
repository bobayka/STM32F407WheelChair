#ifndef _SWITCHINGCONTROL_H
#define _SWITCHINGCONTROL_H

#include "stm32f4xx_hal.h"

void checkStopStartCondition(int16_t sensLEFT, struct Finish const *  finish);

#endif
