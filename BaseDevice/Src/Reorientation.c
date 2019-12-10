#include "Reorientation.h"
#include "main.h"

float  q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

void Reorientation_by_quaternion (struct Accel* AccData){
  float ax = AccData->x;
  float ay = AccData->y;
  float az = AccData->z;
  
  float x = 0,y = 0,z = 0;

  //if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == GPIO_PIN_RESET){
//    *ax=(*ax)/Amplif;
//  *ay=(*ay)/Amplif;
//  *az=(*az)/Amplif;
//  q0=cos(a/2);
//  q1=sin(a/2)*(*az)/magnitude;
////  q2=sin(a/2)*(*ay);
//  q2=0;
//  q3=-sin(a/2)*(*ax)/magnitude;
  
  x = ax * (1 - 2 * (q2q2+q3q3)) + ay * 2 * (q1q2-q0q3) + az * 2 * (q0q2+q1q3);
  y = ax * 2 * (q1q2+q0q3) + ay * (1 - 2 * (q1q1+q3q3)) + az * 2 * (q2q3-q0q1);
  z = ax * 2 * (q1q3+q0q2) + ay * 2 * (q0q1+q2q3) + az * (1 - 2 * (q1q1-q2q2));
  
  AccData->x = x;
  AccData->y = y;
  AccData->z = z;
}

float ag, q0g, q1g, q2g, q3g;
void setReorientationParam(struct Accel const* AccData){
	float ax = AccData->x;
  float ay = AccData->y;
  float az = AccData->z;
	float Amplif = sqrt(ax * ax + ay * ay + az * az);
	float magnitude = sqrt(ax * ax + az * az);
   
	float a = acos(- ay / Amplif);
	float q0 = cos(a/2);
	float q1 = sin(a/2) * az / magnitude;
	float q2 = 0;
	float q3 = -sin(a/2) * ax / magnitude;
	ag = a; 
	q0g = q0;
	q1g = q1;
	q2g = q2;
	q3g = q3;
	q0q1 = q0*q1;
	q0q2 = q0*q2;
	q0q3 = q0*q3;
	q1q1 = q1*q1;
	q1q2 = q1*q2;
	q1q3 = q1*q3;
	q2q2 = q2*q2;
	q2q3 = q2*q3;
	q3q3 = q3*q3;
}













