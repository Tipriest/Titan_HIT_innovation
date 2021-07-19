#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x_adc.h"

#define N 10

extern u32 ADC_Value[N][2];

void myADC_Init(void);
void DMA_Configuration(void);
//u16 Get_Adc(u8 ch);
//u16 Get_Adc_Average(u8 ch, u8 times);

#endif

