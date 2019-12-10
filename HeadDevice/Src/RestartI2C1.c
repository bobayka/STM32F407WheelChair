#include "RestartI2C1.h"

 HAL_StatusTypeDef RestartI2C1(I2C_HandleTypeDef *hi2c){
		HAL_I2C_DeInit(hi2c); 
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();
	__HAL_RCC_I2C1_CLK_DISABLE();
	return HAL_I2C_Init(hi2c);
}
