#include "OLED.h"
#include "IIC.h"

u8 OLED_GRAM[8][128];
u16 DMA1_MEM_LEN;
u8 g_OLED_GRAM_State;	//待写入显存标志位
u8 g_OLED_DMA_BusyFlag;	//DMA忙碌位

unsigned char *reverse(unsigned char *s)
{
    unsigned char temp;
    unsigned char *p = s;    //p??s???
    unsigned char *q = s;    //q??s???
    while(*q)
        ++q;
    q--;
    
    //??????,??p?q??
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}

unsigned char *my_itoa(long n)
{
    int i = 0,isNegative = 0;
    static unsigned char s[50];      //???static??,???????
    if((isNegative = n) < 0) //?????,?????
    {
        n = -n;
    }
    do      //?????????,?????,??????
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //?????,????
    {
        s[i++] = '-';
    }
    s[i] = '\0';    //??????????
    return reverse(s);
}
unsigned char *my_strcat(u8 * str1, u8 * str2)
{
	u8* pt = str1;
	while(*str1 != '\0') str1++;
	while(*str2 != '\0') *str1++ = *str2++;
	*str1 = '\0';
	return pt;

}

void WriteDat(unsigned char I2C_Data)//???
{
	I2C_WriteByte(0x40, I2C_Data);
}

void OLED_WR_Byte (u8 dat,u8 cmd)
{
	I2C_WriteByte(cmd, dat);
}

void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围返回
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_GRAM[pos][x]|=temp;
	else OLED_GRAM[pos][x]&=~temp;	    
}

void WriteCmd(unsigned char I2C_Command)
{
	I2C_WriteByte(0x00, I2C_Command);
}

void OLED_RamClear(void)
{
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[i][n]=0X00; 
}

void OLED_SetPos(unsigned char x, unsigned char y) //???????
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Init(void)
{
	I2C_GPIO_Config();
	I2C_Mode_Config();
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(OLED_Memory_Addressing_Mode);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //???? 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
	
	OLED_RamClear();//Clear OLED ram
	
	#ifdef OLED_DMA_Trans
		DMA_OLED_Config(DMA1_Channel6,(u32)&OLED_HardWare_IIC->DR,(u32)OLED_GRAM,1025);//DMA1??4,???I2C1,????OLED_GRAM,??128*8 = 1024.
		I2C_DMACmd(OLED_HardWare_IIC, ENABLE);//??I2C1 ? DMA??
	#endif
	
	#ifdef OLED_TIM_Refreash
		TIM_Int_Init(OLED_UseTIM_Period*2-1,36000-1);//72MHz
	#endif
}
 
//IIC ssd1306  
void OLED_Refresh_OneTime(void)
{
	
	//#ifdef OLED_HardWareI2C//??IIC
		#ifdef OLED_DMA_Trans//DMA??
			if(g_OLED_DMA_BusyFlag == 0)
			{
				while(I2C_GetFlagStatus(OLED_HardWare_IIC, I2C_FLAG_BUSY));
			
				I2C_GenerateSTART(OLED_HardWare_IIC, ENABLE);//??I2C1
				while(!I2C_CheckEvent(OLED_HardWare_IIC, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,???*/

				I2C_Send7bitAddress(OLED_HardWare_IIC, OLED_ADDRESS, I2C_Direction_Transmitter);//???? -- ??0x78
				while(!I2C_CheckEvent(OLED_HardWare_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
				  /* Test on I2C2 EV1 and clear it */
				//while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED)); 
			
				I2C_SendData(OLED_HardWare_IIC, OLED_DATA);//?????
				while (!I2C_CheckEvent(OLED_HardWare_IIC, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
							  /* Test on I2C2 EV1 and clear it */
				//while(!I2C_CheckEvent(I2C1, I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED));

				//DelayMs(10);
				MYDMA_Enable(DMA1_Channel6);
				g_OLED_DMA_BusyFlag = 1;
			}

		#else
			u8 i = 0 ,j =0;
			while(I2C_GetFlagStatus(OLED_HardWare_IIC, I2C_FLAG_BUSY));
			
			I2C_GenerateSTART(OLED_HardWare_IIC, ENABLE);//??I2C1
			while(!I2C_CheckEvent(OLED_HardWare_IIC, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,???*/

			I2C_Send7bitAddress(OLED_HardWare_IIC, OLED_ADDRESS, I2C_Direction_Transmitter);//???? -- ??0x78
			while(!I2C_CheckEvent(OLED_HardWare_IIC, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

			I2C_SendData(OLED_HardWare_IIC, OLED_DATA);//?????
			while (!I2C_CheckEvent(OLED_HardWare_IIC, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
			
			for(i = 0; i < 8;  i++)
			{
				for(j = 0;j<128;j++)
				{
					I2C_SendData(OLED_HardWare_IIC, OLED_GRAM[i][j]);//????
					while (!I2C_CheckEvent(OLED_HardWare_IIC, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
				}
				
			}
			
			I2C_GenerateSTOP(OLED_HardWare_IIC, ENABLE);//??I2C1??
			
			g_OLED_GRAM_State = 0;	//?????????
		#endif
	//#elif OLED_SimulateI2C//??IIC
		//??IIC?,???????
	
	//#endif
}

void DMA_OLED_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//??DMA??
	
    DMA_DeInit(DMA_CHx);   //?DMA???1?????????
	DMA1_MEM_LEN = cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA?????
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA?????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //??????,??????????  ???????
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA???DMA?????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //?????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //?????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //?????8?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //?????8?
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //???????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA?? x?????? 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA??x????????????
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //??DMA_InitStruct?????????DMA??????????
	  
	//?????NVIC??
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;  //DMA??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //?????0?
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //????1?
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ?????
	NVIC_Init(&NVIC_InitStructure);  //???NVIC???
	
	/* Enable DMA Channelx complete transfer interrupt */
	DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);
	
}

//????DMA??
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //?? DMA ?????? 	
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMA???DMA?????
 	DMA_Cmd(DMA_CHx, ENABLE);  //?? DMA ?????? 
}	 


void DMA1_Channel6_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC6))
	{
		/* Clear the DMA Channel3 transfer error interrupt pending bit */
		DMA_ClearFlag(DMA1_FLAG_TC6);
				
		I2C_GenerateSTOP(OLED_HardWare_IIC, ENABLE);//??I2C1??
		
		g_OLED_GRAM_State = 0;	//?????????
		g_OLED_DMA_BusyFlag = 0;//??????
	}
}

void OLED_I2C1_DMA_Init(void)	
{
	DMA_OLED_Config(DMA1_Channel6,(u32)&OLED_HardWare_IIC->DR,(u32)OLED_GRAM,1025);//DMA1??4,???I2C1,????OLED_GRAM,??128*8 = 1024.
	I2C_DMACmd(OLED_HardWare_IIC, ENABLE);//??I2C1 ? DMA??
	
}

void OLED_ShowINT(u8 x, u8 y, int num, u8 size, u8 mode)
{
	unsigned char *ch = my_itoa(num);
	OLED_ShowString(x, y, ch, size, mode);
}

void OLED_ShowFLOAT(u8 x, u8 y, float num, u8 pointNum,u8 size, u8 mode)
{
	unsigned char ch1[50],ch2[50];
	unsigned char *ptemp;
	unsigned i=0,j=0;
	long t1,t2;
	float ftemp;
	t1 = num/1;
	ftemp = num - t1;
	for(i = 0; i < pointNum;i++)
	{
		ftemp *= 10;
	}
	t2 = (long)ftemp;
	
	ptemp = my_itoa(t1);
	for(i = 0; i < 50;i++)	ch1[i] = *ptemp++;
	ptemp = my_itoa(t2);
	for(i = 0; i < 50;i++)	ch2[i] = *ptemp++;
	
	while(ch1[j] != '\0')
	{
		j++;
	}
	ch1[j] = '.';
	ptemp = my_strcat(ch1, ch2);
	OLED_ShowString(x, y, ptemp, size, mode);
}

void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	//OLED_Refresh_Gram();//????
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//???????????????????
	chr=chr-' ';//???????	
	if(size == 8)csize = 5;	
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//??1206??
		else if(size==16)temp=asc2_1608[chr][t];	//??1608??
		else if(size==24)temp=asc2_2412[chr][t];	//??2412??
		else return;								//?????
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{
	//u8 csize=(size/8+((size%8)?1:0))*(size/2);
	if(size != 8)	
	{
		while(*p!='\0')
		{       
			if(x>OLED_PIXEL_X-(size/2)+1){x=0;y+=size;}
			if(y>OLED_PIXEL_Y-size+1){y=x=0;}
			OLED_ShowChar(x,y,*p,size,mode);  
			x+=size/2;
			p++;
		} 
	}	
	else 
	{
		while(*p!='\0')
		{       
			if(x>OLED_PIXEL_X-(size/2)+1){x=0;y+=size;}
			if(y>OLED_PIXEL_Y-size+1){y=x=0;}
			OLED_ShowChar(x,y,*p,size,mode);  
			x+=5;
			p++;
		} 
	}	
}     

void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(OLED_I2C, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(OLED_I2C, ENABLE);
	while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(OLED_I2C, OLED_ADDRESS, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(OLED_I2C, addr);
	while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(OLED_I2C, data);
	while (!I2C_CheckEvent(OLED_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(OLED_I2C, ENABLE);
}

void OLED_OFF(void)
{
	WriteCmd(0X8D);  //?????
	WriteCmd(0X10);  //?????
	WriteCmd(0XAE);  //OLED??
}


