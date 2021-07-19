#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"

extern int   motor1, motor2, motor3, motor4;                      //电机PWM变量

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C    
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x40010C0C    
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x40010C0C    
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x40010C0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40010C0C    


#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 

 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PDout(n)    BIT_ADDR(GPIOD_ODR_Addr,n)  //输入 
#define PEout(n)    BIT_ADDR(GPIOE_ODR_Addr,n)  //输入 
#define PGout(n)    BIT_ADDR(GPIOG_ODR_Addr,n)  //输入 


#define PWM1   TIM8->CCR4    //PA7
#define PWM2   TIM8->CCR2    //PA7
#define PWM3   TIM8->CCR1    //PA7
#define PWM4   TIM8->CCR3    //PA7

#define AIN2   PGout(15)
#define AIN1   PGout(13)
#define BIN1   PEout(1)
#define BIN2   PEout(6)
#define CIN2   PBout(9)
#define CIN1   PGout(11)
#define DIN1   PDout(7)
#define DIN2   PDout(5)


void Set_Pwm(int moto1,int moto2,int moto3,int moto4);
int myabs(int a);


#endif

