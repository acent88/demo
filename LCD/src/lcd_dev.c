/******************** (C) COPYRIGHT 2010  *************************************
* 文件名称: lcd_dev.c
* 程序作者: chf
* 程序版本: V1.0
* 编制日期: 2017.02.25
* 功能描述: lcd 低层驱动函数
*******************************************************************************/

#define	LCD_DEV_H

#include "stm32f10x.h"
#include  <os.h>
#include "app_cfg.h"
#include "lcd_dev.h"
#include "oledfont.h"
#include <stdio.h>
#include <string.h>


/*******************************************************************************
* Function Name  : LCD_Create_Event
* Description    : 创建LCD任务中需要的事件
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Create_Event(void)
{
    
}

/*******************************************************************************
* Function Name  : LCD_GPIO_Configuration
* Description    : LCD GPIO 初始化函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = LCD_SPI_CS_PIN | LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN | LCD_SPI_MISO_PIN; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
    GPIO_Init(LCD_SPI_PORT, &GPIO_InitStructure); 
	//
    LCD_REST_SELECT();
}

/*******************************************************************************
* Function Name  : LCD_Write_Byte
* Description    : SPI write a byte
* Input          : Byte,type
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Write_Byte(unsigned char Byte,LCD_WRITE_TYPE type)
{
	unsigned char i;		
	//
	if( LCD_WRITE_CMD == type)LCD_RS_CMD();
	else LCD_RS_DATA();		  
	//
	for(i=0;i<8;i++)
	{			  
		//
		LCD_SCK_Clr();
		//
		if(Byte&0x80) LCD_MOSI_Set();
		else LCD_MOSI_Clr();
		//
		LCD_SCK_Set();
		//
		Byte<<=1;   
	}			  
	//
	LCD_RS_CMD();
}

/*******************************************************************************
* Function Name  : Power_Variable_Init
* Description    : 初始化LCD任务所用到的全局变量
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Variable_Init(void)
{
	

}

/*******************************************************************************
* Function Name  : LCD_Refresh_Gram
* Description    : 更新显存到LCD		
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Refresh_Gram(void)
{
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		LCD_Write_Byte (0xb0+i,LCD_WRITE_CMD);    //设置页地址（0~7）
		LCD_Write_Byte (0x00,LCD_WRITE_CMD);      //设置显示位置—列低地址
		LCD_Write_Byte (0x10,LCD_WRITE_CMD);      //设置显示位置—列高地址   
		for(n=0;n<LCD_X_MAX_SIZE;n++)LCD_Write_Byte(LCD_GRAM[n][i],LCD_WRITE_DATA); 
	}  
}
	  	  
/*******************************************************************************
* Function Name  : LCD_Display_On
* Description    : 开启OLED显示
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display_On(void)
{
	LCD_Write_Byte(0X8D,LCD_WRITE_CMD);  //SET DCDC命令
	LCD_Write_Byte(0X14,LCD_WRITE_CMD);  //DCDC ON
	LCD_Write_Byte(0XAF,LCD_WRITE_CMD);  //DISPLAY ON
}
   
/*******************************************************************************
* Function Name  : LCD_Display_Off
* Description    : 关闭OLED显示     
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display_Off(void)
{
	LCD_Write_Byte(0X8D,LCD_WRITE_CMD);  //SET DCDC命令
	LCD_Write_Byte(0X10,LCD_WRITE_CMD);  //DCDC OFF
	LCD_Write_Byte(0XAE,LCD_WRITE_CMD);  //DISPLAY OFF
}	

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	  
void LCD_Clear(void)  
{  
	unsigned char i,n;  
	for(i=0;i<LCD_Y_MAZ_SIZE;i++)
	{
		for(n=0;n<LCD_X_MAX_SIZE;n++)
		{
			LCD_GRAM[n][i]=0X00;  
		}
	}
	LCD_Refresh_Gram();//更新显示
}

/*******************************************************************************
* Function Name  : LCD_Disp_Init
* Description    : LCD显示初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Disp_Init(void)
{			  
	LCD_Write_Byte(0xAE,LCD_WRITE_CMD); //关闭显示
	LCD_Write_Byte(0xD5,LCD_WRITE_CMD); //设置时钟分频因子,震荡频率
	LCD_Write_Byte(0x50,LCD_WRITE_CMD);   //[3:0],分频因子;[7:4],震荡频率
	LCD_Write_Byte(0xA8,LCD_WRITE_CMD); //设置驱动路数
	LCD_Write_Byte(0X3F,LCD_WRITE_CMD); //默认0X3F(1/64) 
	LCD_Write_Byte(0xD3,LCD_WRITE_CMD); //设置显示偏移
	LCD_Write_Byte(0X00,LCD_WRITE_CMD); //默认为0

	LCD_Write_Byte(0x40,LCD_WRITE_CMD); //设置显示开始行 [5:0],行数.
													    
	LCD_Write_Byte(0x8D,LCD_WRITE_CMD); //电荷泵设置
	LCD_Write_Byte(0x14,LCD_WRITE_CMD); //bit2，开启/关闭
	LCD_Write_Byte(0x20,LCD_WRITE_CMD); //设置内存地址模式
	LCD_Write_Byte(0x02,LCD_WRITE_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	LCD_Write_Byte(0xA1,LCD_WRITE_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	LCD_Write_Byte(0xC0,LCD_WRITE_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	LCD_Write_Byte(0xDA,LCD_WRITE_CMD); //设置COM硬件引脚配置
	LCD_Write_Byte(0x12,LCD_WRITE_CMD); //[5:4]配置
		 
	LCD_Write_Byte(0x81,LCD_WRITE_CMD); //对比度设置
	LCD_Write_Byte(0xEF,LCD_WRITE_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	LCD_Write_Byte(0xD9,LCD_WRITE_CMD); //设置预充电周期
	LCD_Write_Byte(0xf1,LCD_WRITE_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	LCD_Write_Byte(0xDB,LCD_WRITE_CMD); //设置VCOMH 电压倍率
	LCD_Write_Byte(0x30,LCD_WRITE_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	LCD_Write_Byte(0xA4,LCD_WRITE_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	LCD_Write_Byte(0xA6,LCD_WRITE_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	LCD_Write_Byte(0xAF,LCD_WRITE_CMD); //开启显示	 
	LCD_Clear();

}

/*******************************************************************************
* Function Name  : LCD_Init
* Description    : LCD 初始化.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Init(void)
{
	OS_ERR err;
	//
	LCD_Create_Event();
	//
	LCD_GPIO_Configuration();
	//
	LCD_Variable_Init();
	//
	OSTimeDly( OS_TICKS_PER_SEC/10, OS_OPT_TIME_TIMEOUT, &err );
	//
	LCD_REST_DESELECT();
	//
	LCD_Disp_Init();
}


/*******************************************************************************
* Function Name  : LCD_DrawPoint
* Description    : LCD_DrawPoint
* Input          : x:0~127 y:0~63 t:1 填充 0,清空		
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
	unsigned char pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)LCD_GRAM[x][pos]|=temp;
	else LCD_GRAM[x][pos]&=~temp;
}

/*******************************************************************************
* Function Name  : LCD_Fill
* Description    : LCD_Fill
* Input          : x1,y1,x2,y2 填充区域的对角坐标 确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63 dot:0,清空;1,填充	  	 	
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot)
{
	unsigned char x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)LCD_DrawPoint(x,y,dot);
	}													    
	LCD_Refresh_Gram();//更新显示
}

/*******************************************************************************
* Function Name  : LCD_ShowChar
* Description    : 在指定位置显示一个字符,包括部分字符
* Input          : x:0~127	y:0~63 	mode:0,反白显示;1,正常显示 size:选择字体 16/12 		
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size,unsigned char mode)
{
	unsigned char temp,t,t1;
	unsigned char y0=y;
	chr=chr-' ';//得到偏移后的值				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //调用1206字体
		else temp=oled_asc2_1608[chr][t];		 //调用1608字体 	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_DrawPoint(x,y,mode);
			else LCD_DrawPoint(x,y,!mode);
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
  
/*******************************************************************************
* Function Name  : LCD_ShowString
* Description    : 显示字符串
* Input          : x,y:起点坐标  *p:字符串起始地址
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_ShowString(unsigned char x,unsigned char y,char *str)
{
    while(*str!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
		
        if(y>MAX_CHAR_POSY){y=x=0;LCD_Clear();}
		
        LCD_ShowChar(x,y,*str,16,1);	 
        x+=8;
        str++;
    }  
}

/*******************************************************************************
* Function Name  : LCD_Display_Start
* Description    : 开机时显示的启动条
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display_Start(void)
{
	OS_ERR err;
	//
	LCD_ShowString(0,0, "shang di is SB");  
	//
	LCD_ShowString(16,48,"START...");  
	//
	LCD_Refresh_Gram();
	//
	OSTimeDly( OS_TICKS_PER_SEC, OS_OPT_TIME_TIMEOUT, &err );
	//
	//LCD_Clear();
}

