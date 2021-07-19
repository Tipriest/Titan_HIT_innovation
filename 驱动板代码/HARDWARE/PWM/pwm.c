#include "pwm.h"

void Motor_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //ʹ��PD�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //ʹ��PE�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE); //ʹ��PG�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //�����趨������ʼ��GPIOB 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;	//�˿�����
	GPIO_Init(GPIOD, &GPIO_InitStructure);				  //�����趨������ʼ��GPIOB 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_6;	//�˿�����
	GPIO_Init(GPIOE, &GPIO_InitStructure);				  //�����趨������ʼ��GPIOB 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;	//�˿�����
	GPIO_Init(GPIOG, &GPIO_InitStructure);				  //�����趨������ʼ��GPIOB 
	PWM_Init(arr,psc);
}

void PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //ʹ��GPIO����ʱ��ʹ��

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;                //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;              //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;           //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);        //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;      //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM8, &TIM_OCInitStructure); 
  TIM_OC2Init(TIM8, &TIM_OCInitStructure); 	//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM8, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);               //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_CtrlPWMOutputs(TIM8,ENABLE);	                  //MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);     //CH1Ԥװ��ʹ��	 
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);   
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);   
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);     //CH4Ԥװ��ʹ��	 

	TIM_ARRPreloadConfig(TIM8, ENABLE);                   //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM8, ENABLE);                                //ʹ��TIM1
}

