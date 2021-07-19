#include "stm32f10x.h"
#include "PID.h"
#include "pwm.h"
#include "Motor.h"
#include "Encoder.h"
#include "Timer.h"
#include "usart.h"
#include "Speed.h"
#include "sys.h"
#include "delay.h"

//int i=0;

 int main(void)
 {	 
	 delay_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 Motor_Init(7199,0); 
	 
	 while(1)
	 {
	
	 }
}
 

/*void TIM5_IRQHandler(void)                            //TIM5�ж�
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);   //���TIMx���жϴ�����λ
		i++;
		if(i==4)
			i=0;
	}
}*/

