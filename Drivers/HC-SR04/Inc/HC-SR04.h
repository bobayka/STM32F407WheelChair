#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "stm32f4xx_hal.h"

struct GPIOSettings{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	GPIO_PinState State;
};

struct TIMParam{
	uint32_t Channel;
	int CounterIn;
	int counterOut;
};
struct RangingModule{
	struct GPIOSettings GPIOsettings;
	struct TIMParam TIMparam;
	float Distance;//cm
};

void distanceMeasurement(TIM_HandleTypeDef* htim, struct RangingModule* module);

#endif
