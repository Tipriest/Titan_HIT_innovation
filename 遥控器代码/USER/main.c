#include "stm32f10x.h"
#include "ADC.h"
#include "delay.h"
#include "usart.h"
#include "OLED.h"
#include "IIC.h"
#include "TIMER.h"

float voltage[2];

 int main(void)
 {	
	 u32 sum;
	 u32 i=0, j=0;
	 u16 times=0;
	 u8 t=0;
	 uint8_t Buf[86]="####G307458772P1039291851S000H5010IA-0.12345N-0.12345E00000000D-035R######U30222B####";
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	 uart_init(9600);
   myADC_Init();
	 DMA_Configuration();
	 OLED_Init();
	 //OLED_I2C1_DMA_Init();
	 TIM_Int_Init(4999, 7199);
	 Buf[0] = 0XED;  Buf[1] = 0x3B; Buf[2] = 0x01; Buf[3] = 0xB1; Buf[67] = 0x00; Buf[68] = 0xAB;
   Buf[69] = 0xDD; Buf[70] = 0xDD;Buf[71] = 0x41;Buf[72] = 0x30;Buf[80] = 0x54;Buf[81] = 0x30;
	 Buf[82] = 0x50; Buf[83] = 0xDE;
	 while(1)
	 {
		 
		times++;
		if(times%10==0)
		{	
       for(t=0;t<84;t++)
		   {
					 USART_SendData(USART2,Buf[t]);                                 //向串口2发送数据
					 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);         //等待发送结束
				   delay_ms(10);
		   }
			 
	  }
			

		 
		 for(j=0; j<2; j++)
		 {
			 sum=0;
			 for(i=0; i<N; i++)
			 {
				 sum+=ADC_Value[i][j];
			 }
			 voltage[j]=(float)sum/(N*4096)*3.3;
		 }
		 OLED_ShowString(0,15,(u8*)"Now Speed:",16,OLED_DISPLAYCHAR_NORMAL);
	   OLED_ShowFLOAT(0,32, voltage[0], 8, 16, OLED_DISPLAYCHAR_NORMAL);
			OLED_ShowFLOAT(0,49, voltage[1], 8, 16, OLED_DISPLAYCHAR_NORMAL);
	 
	 }	
		 
 }
 
 void OLED_UseTIM_IRQHander(void)   //TIM3??
	{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //??TIM2????????
			{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //??TIMx?????? 
				//g_tempNum ++;
				printf("\nx_voltage is %f, y_voltage is %f\n\r", voltage[0], voltage[1]);
				OLED_Refresh_OneTime();
			}
			
	}
 
