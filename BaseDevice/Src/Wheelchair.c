#include "Wheelchair.h"
float StopKoef  = 0;

void stopWheelchair(DAC_HandleTypeDef* hdac){
	HAL_DAC_SetValue(hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2085/1.07);
	HAL_DAC_SetValue(hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 2200/1.07);
}

void Wheelchair( uint16_t* ADC_data, struct FinishAngle const* finangl, uint8_t angleMax, uint8_t angleMin){
		float final_X_Angle = finangl->rotate.x;
		float final_Z_Angle = finangl->rotate.z;
		uint8_t subMaxMin = angleMax - angleMin;
		float abs_fin_z_angle = fabsf(final_Z_Angle);
		float abs_fin_x_angle = fabsf(final_X_Angle);

		float subZ;
		if (abs_fin_z_angle > angleMax)
			subZ = subMaxMin;
		else if (abs_fin_z_angle > angleMin)
			subZ = abs_fin_z_angle - angleMin;
		else
			subZ = 0;
		
		
		float subX;
		if (abs_fin_x_angle > angleMax)
			subX = subMaxMin;
		else if (abs_fin_x_angle > angleMin)
			subX = abs_fin_x_angle - angleMin;
    else
			subX = 0;
		
		if (finangl->rotate.z > 0){
			ADC_data[1]= 2085 + (int)( 82 * subZ) * StopKoef ;
		}
		else{
			ADC_data[1]= 2085 - (int)( 71 * subZ);
		}
		if (finangl->rotate.x > 0){

			ADC_data[0]= 2200 + (int)( 99 * subX);
		}
        else {
            ADC_data[0]= 2200 - (int)( 62 * subX);
        }
   
}
