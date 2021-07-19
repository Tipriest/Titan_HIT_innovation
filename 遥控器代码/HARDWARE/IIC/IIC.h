#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x.h"

//I2C Parameter Configure
#define OLED_I2C I2C1
#define OLED_I2C_APBxClock_FUN RCC_APB1PeriphClockCmd
#define OLED_I2C_CLK RCC_APB1Periph_I2C1
#define OLED_I2C_GPIO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define OLED_I2C_GPIO_CLK RCC_APB2Periph_GPIOB
#define OLED_I2C_SCL_PORT GPIOB
#define OLED_I2C_SDA_PORT GPIOB
#define OLED_I2C_SCL_PIN GPIO_Pin_6
#define OLED_I2C_SDA_PIN GPIO_Pin_7

//I2C Mode Configure
#define I2C_Speed 400000
#define I2Cx_OWN_ADDRESS 0x0A
#define I2C_PageSize 8

void I2C_GPIO_Config(void);
void I2C_Mode_Config(void);

#endif

