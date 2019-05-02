#include "Wheelchair.h"

void stopWheelchair(DAC_HandleTypeDef* hdac){
	HAL_DAC_SetValue(hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2085/1.07);
	HAL_DAC_SetValue(hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2200/1.07);
}

void Wheelchair( uint16_t* ADC_data, struct FinishAngle const* finangl, uint8_t angleMax, uint8_t angleMin){
  
//    int8_t signZ, signX;    
		float final_X_Angle = finangl->rotate.x;
		float final_Z_Angle = finangl->rotate.z;
	
//   signZ = (int8_t) copysign(1, final_Z_Angle);
//   signX = (int8_t) copysign(1, final_X_Angle);
//    else if (gz>20 && final_Z_Angle>0){
//      tickAct=HAL_GetTick();
//    }    
	// for parabola Wheelchair(ADC_data,&finangl, 15, 5, 3.6, 3.4);
//      ADC_data[1]= 2085 + (int)(coefZ*(final_Z_Angle*final_Z_Angle*signZ + final_Z_Angle) );
//      ADC_data[0]= 2200 + (int)(coefX*(final_X_Angle*final_X_Angle*signX + final_X_Angle) );
		
		ADC_data[1]= 2085;
		ADC_data[0]= 2200;
		if (finangl->rotate.z > 0){
			if (fabsf(final_Z_Angle)> angleMin)
				ADC_data[1]= 2085 + (int)( 82 * (fabsf(final_Z_Angle) - angleMin));
			if (fabsf(final_Z_Angle)>angleMax)
				ADC_data[1]= 2085 + (int)( 82 * (angleMax - angleMin));
			}
		else{
			if (fabsf(final_Z_Angle)> angleMin)
			ADC_data[1]= 2085 - (int)( 71 * (fabsf(final_Z_Angle) - angleMin));
			if (fabsf(final_Z_Angle)>angleMax)
				ADC_data[1]= 2085 - (int)( 71 * (angleMax - angleMin));
		}
		if (finangl->rotate.x > 0){
      if (fabsf(final_X_Angle)>angleMin)
        ADC_data[0]= 2200 + (int)( 99 * (fabsf(final_X_Angle) - angleMin));
      if (fabsf(final_X_Angle)>angleMax)
        ADC_data[0]= 2200 + (int)( 99 * (angleMax - angleMin));
		}
    else {
		  if (fabsf(final_X_Angle)>angleMin)
        ADC_data[0]= 2200 - (int)( 62 * (fabsf(final_X_Angle) - angleMin));
      if (fabsf(final_X_Angle)>angleMax)
        ADC_data[0]= 2200 - (int)( 62 * (angleMax - angleMin));
		}
   
}
