#include "switchingControl.h"

#include "main.h"
#include "Wheelchair.h"
#include "Reorientation.h"

#define IN 1
#define OUT 0

extern int8_t switchMode;


void checkSwitchingCondition(int16_t sensLEFT, struct Finish const *  finish){

	static uint32_t tickBut=0;
	static uint64_t tickRunDevice = 0;
	static uint8_t state = OUT;
	if (finish->gyroData.y < -sensLEFT && HAL_GetTick() - tickBut >= 250){
		tickBut = HAL_GetTick();
		tickRunDevice = HAL_GetTick();
		if(switchMode == RemoteON){
			state = IN;
		}else{
			switchMode = ~switchMode;
		}
	}
	if((HAL_GetTick() - tickRunDevice) >= 3000 && state == IN){
		state = OUT;
		setReorientationParam(&finish->accelData);
		switchMode = ~switchMode;
	}

}
