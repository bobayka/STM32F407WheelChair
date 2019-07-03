

#ifndef _I2C_Clear_Busy
#define _I2C_Clear_Busy
#include "stm32f4xx_hal.h"
#include "myError.h"

myError I2C_ClearBusyFlagErratum(I2C_HandleTypeDef* handle, uint32_t timeout);

#endif

