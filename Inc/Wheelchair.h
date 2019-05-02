#include "stdint.h"
#include "main.h"
#include "stm32f4xx_hal.h"

#ifndef __WHEELCHAIR_H__
#define __WHEELCHAIR_H__
void Wheelchair( uint16_t* ADC_data, struct FinishAngle const* finangl, uint8_t angleMax, uint8_t angleMin);
void stopWheelchair(DAC_HandleTypeDef* hdac);

#endif
