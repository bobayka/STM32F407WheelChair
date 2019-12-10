#include "HC-SR04.h"

void distanceMeasurement(TIM_HandleTypeDef* htim, struct RangingModule* module){
	GPIO_PinState state = HAL_GPIO_ReadPin(module ->GPIOsettings.GPIOx, module -> GPIOsettings.GPIO_Pin);
	if ( module->GPIOsettings.State != state){
		module->GPIOsettings.State = state;
		int compare = __HAL_TIM_GetCompare(htim, module -> TIMparam.Channel);
		if (module->GPIOsettings.State == GPIO_PIN_SET){
			module->TIMparam.CounterIn =  compare;
		}else{
			module->Distance =  (float)(compare - module->TIMparam.CounterIn) / 58;// Distance in cm
		}
	}
}

