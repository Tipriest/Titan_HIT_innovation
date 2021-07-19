#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"


//#define OLED_TIM_Refreash	//???????????????
#define OLED_UseTIM_Clock	RCC_APB1Periph_TIM2
#define OLED_UseTIM			TIM2
#define OLED_UseTIM_IRQn	TIM2_IRQn
#define OLED_UseTIM_IRQHander		TIM2_IRQHandler
#define OLED_UseTIM_Period			30  				//??:ms

void TIM_Int_Init(u16 arr,u16 psc);


#endif
