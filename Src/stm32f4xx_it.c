/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */
#include <string.h> /* memset */
#include "stdio.h"
#include "Reorientation.h"  
//#include "Magnitometer_Calibration.h"
#include "AccelAngle.h"
#include "KalmanFilter.h"
#include "complimentaryfilter.h"
#include "Heading.h"
//#include "MouseControl.h"
#include "Wheelchair.h"
#include "I2C_ClearBusyFlagErratum.h"
#include "RestartI2C1.h"
#include "switchingControl.h"
#include "HC-SR04.h"
#include "MPU6050.h"


#define CURSOR_STEP 1

uint8_t HID_Buffer[4];

extern MPU6050_GYROResult    IntGyroData;
extern MPU6050_ACCResult     IntAccData;

struct RangingModule frontModule1 = {{GPIOA, GPIO_PIN_6, GPIO_PIN_RESET}, {TIM_CHANNEL_1, 0, 0}, 0.0};
struct RangingModule frontModule2 = {{GPIOA, GPIO_PIN_7, GPIO_PIN_RESET}, {TIM_CHANNEL_2, 0, 0}, 0.0};
KalmanFilter HC_SR04_Kf_Module1;
	
struct Kfilterforaxis Kalman = {0};// change on static
struct FinishAngle finangl;
struct Finish finish;
struct AccelAngle accelAngle;
float magnet_Y_Angle;
uint16_t ADC_data[2];
extern int8_t switchMode;

extern DAC_HandleTypeDef hdac;
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

float convertAccData(int16_t acc);
float convertGyroData(int16_t gyro);
float convertTempData(int16_t temp);

enum { 
	OFF,
	ON
};
typedef struct{
uint8_t distStopFlag; 
uint8_t errorStopFlag;
} StopFlags;

StopFlags stopFlags = {OFF, OFF};
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles ADC1, ADC2 and ADC3 global interrupts.
*/
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc3);
  /* USER CODE BEGIN ADC_IRQn 1 */

	if(!switchMode){
		ADC_data[0] = HAL_ADC_GetValue(&hadc1);//����� ���������� �����??
		ADC_data[1] = HAL_ADC_GetValue(&hadc3);

//		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,ADC_data[1]/1.07);
//		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,ADC_data[0]/1.07);
	}
  /* USER CODE END ADC_IRQn 1 */
}

/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	stopFlags.errorStopFlag = OFF;
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
	GetRawGyro(&IntGyroData);// Get three projections of the gyroscope
	int status = GetRawAcc(&IntAccData);
	if (status != HAL_OK){
		stopFlags.errorStopFlag = ON;
		myError err = I2CErrorHandling(&hi2c1, status);
		err = Wrap(err, "I2CErrorHandling: "); 
		HAL_UART_Transmit(&huart1, (uint8_t*)err.msg.msg, err.msg.size, HAL_MAX_DELAY);
	}else{
		// learn reset interrupt

		finish.gyroData.x = convertGyroData(IntGyroData.Gyro_X) - 0.197288513f;// It's raw data without calibration factors
		finish.gyroData.y = convertGyroData(IntGyroData.Gyro_Y) - 0.118911743f;// You can get these coefficients in previous versions of the program
		finish.gyroData.z = -(convertGyroData(IntGyroData.Gyro_Z) - 0.557388306f);
		//-------------------------------------------------------------------------        
		finish.accelData.x = convertAccData(IntAccData.Accel_X) - 0.0123291016f;// It's raw data without calibration factors
		finish.accelData.y = convertAccData(IntAccData.Accel_Y) - 0.0107421875f;// You can get these coefficients in previous versions of the program 
		finish.accelData.z = convertAccData(IntAccData.Accel_Z) - 0.0264587402f;        

		checkSwitchingCondition(140, &finish);

		Reorientation_by_quaternion (&finish.accelData);
						
		//--------------------------------------------------------------------------- 
		float t;				
		t = convertTempData(IntAccData.Temperature);// The value of temperature
		//------------------------------GetAccelAngles-------------------------------------
		accelAngle.rotate.x = get_X_Rotation(&finish.accelData);// Angles of the accel
		accelAngle.rotate.y = get_Y_Rotation(&finish.accelData);
		accelAngle.rotate.z = get_Z_Rotation(&finish.accelData);
		//------------------------------------------------------------------------
	}
	if(switchMode == RemoteOFF){
		Wheelchair(ADC_data,&finangl, 13, 3);// add stopflag to this function
	}
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,ADC_data[1]/1.07);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_2,DAC_ALIGN_12B_R,ADC_data[0]/1.07);
	//MouseControl(HID_Buffer, &finangl, &finish.gyroData, 15, 0.04, 0.03 , 40, 120);// Function of mouse control
        
  /* USER CODE END TIM2_IRQn 1 */
}

/**
* @brief This function handles TIM3 global interrupt.
*/
float distance1 , distance2;
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	static int stopStateCnt = 0;
	static int startStateCnt = 0;

	
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	distanceMeasurement(&htim3, &frontModule1);
	distanceMeasurement(&htim3, &frontModule2);
	distance1 = frontModule1.Distance;
	distance2 = frontModule2.Distance;
	KalmanFilterSimple1D(frontModule1.Distance, &HC_SR04_Kf_Module1, 30);
	
	if(HC_SR04_Kf_Module1.state < 200){
		if (stopStateCnt > 3 ){
			stopFlags.distStopFlag = ON;
		}
		stopStateCnt++;
		startStateCnt = 0;
	}else{
		if (startStateCnt > 5){
			stopFlags.distStopFlag = OFF;
		}
		startStateCnt++;
		stopStateCnt = 0;
	}
  /* USER CODE END TIM3_IRQn 1 */
}

/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */

  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

	//---------------------KalmanFilter-----------------------------------------------
	KalmanFilterSimple1D(accelAngle.rotate.x,&Kalman.Kf_X, 10);//filtered value 
	KalmanFilterSimple1D(accelAngle.rotate.z,&Kalman.Kf_Z, 10);

	//---------------------AnglesAfterAllFilter---------------------------------------
	complimentaryfilter(&finangl.rotate.x, finish.gyroData.x, &Kalman.Kf_X.state, 0.99f, 0.005);// final angels
	complimentaryfilter(&finangl.rotate.z, finish.gyroData.z, &Kalman.Kf_Z.state, 0.99f, 0.005);// final angels

  /* USER CODE END TIM4_IRQn 1 */
}

/**
* @brief This function handles TIM5 global interrupt.
*/
void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM5_IRQn 0 */
	static int counter = 0;
  /* USER CODE END TIM5_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM5_IRQn 1 */
	counter += (stopFlags.distStopFlag == ON||stopFlags.errorStopFlag)? 1: -1;
	counter = (counter > 50)? 50 : counter;
	counter = (counter < 0)? 0 : counter;
	StopKoef = 1 - counter * 0.02;
  /* USER CODE END TIM5_IRQn 1 */
}

/**
* @brief This function handles USB On The Go FS global interrupt.
*/
void OTG_FS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_FS_IRQn 0 */

  /* USER CODE END OTG_FS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
  /* USER CODE BEGIN OTG_FS_IRQn 1 */

  /* USER CODE END OTG_FS_IRQn 1 */
}

/* USER CODE BEGIN 1 */

float convertAccData(int16_t acc) {
	float f_acc = ((float) (acc * 2.0f) / 32768.0f);
	return f_acc;
}

float convertGyroData(int16_t gyro) {
//	float f_gyro = (float) (gyro * 250.0f) / 32768.0f;
  	float f_gyro = (float) (gyro / 131.0f);
	return f_gyro;
}

float convertTempData(int16_t temp)
{
  float f_temp = (temp)/340.0+17;///333.87+21
  return f_temp;
}

float convertMagnetData(uint16_t magn){
//  float f_magn=((((float)magn) - 128.0f)/(256.0f) + 1.0f) * 4912.0f / 32760.0f;
//    float f_magn=(float)magn * 4912.0f / 32760.0f;
      if(magn>=0x8000)
        	return -((65535 - magn) + 1);
      else
        return magn;
}

float getLength(float ax, float ay, float az) {  
	return ax*ax + ay*ay + az*az;
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
