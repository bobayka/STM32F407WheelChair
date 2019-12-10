#ifndef __UartTransmitAG_H
#define __UartTransmitAG_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "myError.h"

 
struct MPU6050_ACCResult;
struct MPU6050_GYROResult;

 
myError getCharBufferForTransmit(struct MPU6050_ACCResult * accelData, struct MPU6050_GYROResult*  gyroData, uint8_t* buffer, int size);


#endif

