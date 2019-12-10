#include "UartTransmitAG.h"
#include "MPU6050.h"


#define DIM 3
myError getCharBufferForTransmit( struct MPU6050_ACCResult * accelData, struct MPU6050_GYROResult*  gyroData, uint8_t* buffer, int size){
	int16_t inputBuffer[2* DIM]={accelData->Accel_X, accelData->Accel_Y, accelData->Accel_Z, 
											 gyroData->Gyro_X, gyroData->Gyro_Y, gyroData->Gyro_Z};
	
	buffer[0] = ':';
	int ret = 1;
	for (int i = 0; i < 2 * DIM; i++) {
		buffer[ret++] = (uint8_t)(inputBuffer[i] >> 8);
		buffer[ret++] = (uint8_t)(inputBuffer[i]);
		buffer[ret++] = ',';
	}
	buffer[ret++] = '\r';
	return NewError(HAL_OK, "");
}
