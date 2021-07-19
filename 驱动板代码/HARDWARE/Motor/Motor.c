#include "Motor.h"

int motor1;
int motor2;
int motor3;
int motor4;

void Set_Pwm(int moto1,int moto2,int moto3,int moto4)
{
	if(moto1>0)     AIN2=0, AIN1=1;
	else 	            AIN2=1,	AIN1=0;
	PWM1=myabs(moto1);
	if(moto2>0)     BIN2=1, BIN1=0;
	else 	            BIN2=0,	BIN1=1;
	PWM2=myabs(moto1);
	if(moto3>0)     CIN2=0, CIN1=1;
	else 	            CIN2=1,	CIN1=0;
	PWM3=myabs(moto1);
	if(moto4>0)     DIN2=1, DIN1=0;
	else 	            DIN2=0,	DIN1=1;
	PWM4=myabs(moto1);
}

int myabs(int a)
{
	int temp;
	if(a<0)  
	  temp=-a;  
	else 
	  temp=a;
	return temp;
}
