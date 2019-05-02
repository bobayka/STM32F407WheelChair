#include "RestartI2C1.h"


void RestartI2C1(I2C_HandleTypeDef *hi2c){
	__HAL_RCC_I2C1_CLK_ENABLE();
	HAL_Delay(100);
	__HAL_RCC_I2C1_FORCE_RESET();
	HAL_Delay(100);
	__HAL_RCC_I2C1_RELEASE_RESET();
	HAL_Delay(100);
}