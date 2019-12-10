#ifndef __myError_H
#define __myError_H

#include "stm32f4xx_hal.h"
#include "myMsg.h"


typedef struct  {
	HAL_StatusTypeDef error;
	struct myMsg msg;
}myError;

myError NewError(HAL_StatusTypeDef error, char* msg);
myError Wrap (myError err, char* msg);
myError I2CErrorHandling(I2C_HandleTypeDef* hi2c,HAL_StatusTypeDef err);
#endif

