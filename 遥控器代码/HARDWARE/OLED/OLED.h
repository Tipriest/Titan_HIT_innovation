#ifndef __OLED_H
#define __OLED_H
#include "stm32f10x.h"
#include "IIC.h"
#include "oledfont.h"

extern u8 OLED_GRAM[8][128];
extern u8 g_OLED_GRAM_State;	//待写入显存标志位
extern u8 g_OLED_DMA_BusyFlag;	//DMA忙碌位
#define OLED_Memory_Addressing_Mode  0x00
#define OLED_ADDRESS	0x78
#define OLED_CMD 0x00
#define OLED_DATA 0x40

//#define OLED_DMA_Trans 			//???????DMA??  ??????OLED_HardWareI2C
#define OLED_HardWare_IIC	OLED_I2C
//#define OLED_SimulateI2C	//?????????I2C??

#define OLED_LED_LIGHTUP 1			//???????
#define OLED_LED_EXTINGUISH 0		//???????

#define OLED_DISPLAYCHAR_NORMAL 1	//????
#define OLED_DISPLAYCHAR_REVERSE 0	//????

#define OLED_PIXEL_X 128			//???? X_MAX
#define OLED_PIXEL_Y 64				//???? Y_MAX


#define OLED_OK()		g_OLED_GRAM_State = 1	//??? ?????????

enum enum_OLED_Direction  
{
	UP = 0,
	DOWN = 1,
	LEFT = 2,
	RIGHT = 3
};

void I2C_WriteByte(uint8_t addr,uint8_t data);
void DMA_OLED_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);
void DMA1_Channel6_IRQHandler(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void WriteCmd(unsigned char I2C_Command);
void OLED_RamClear(void);
void OLED_Init(void);
void OLED_I2C1_DMA_Init(void);
void OLED_Refresh_OneTime(void);

void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  ;	
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode);
void OLED_DrawLine(int x1,int y1,int x2,int y2,int color);
void OLED_Show16X16CN_AND_8X16ASC(unsigned int x, unsigned int y, unsigned char *s , u8 mode);
void OLED_ShowINT(u8 x, u8 y, int num, u8 size, u8 mode);
void OLED_ShowFLOAT(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode);
void OLED_OFF(void);

#endif

