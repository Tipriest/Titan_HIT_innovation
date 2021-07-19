#include "IIC.h"

void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	OLED_I2C_GPIO_APBxClock_FUN(OLED_I2C_GPIO_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  OLED_I2C_SCL_PIN | OLED_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(OLED_I2C_SCL_PORT, &GPIO_InitStructure);
}

void I2C_Mode_Config(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	OLED_I2C_APBxClock_FUN(OLED_I2C_CLK,ENABLE);
	I2C_DeInit(OLED_I2C);
	
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2Cx_OWN_ADDRESS;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

	I2C_Init(OLED_I2C, &I2C_InitStructure);
	I2C_Cmd(OLED_I2C, ENABLE);
}


