#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h" 

#define ENCODER_TIM_PERIOD (u16)(65535)   //���ɴ���65535 ��ΪF103�Ķ�ʱ����16λ�ġ�

void Encoder_Init_TIM1(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);

s16 getTIMx_DetaCnt(TIM_TypeDef * TIMx);
void Get_Motor_Speed(int *Speed_Motor1,int *Speed_Motor2, 
										int *Speed_Motor3,int *Speed_Motor4);

#endif
