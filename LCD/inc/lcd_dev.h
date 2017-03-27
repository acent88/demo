/******************** (C) COPYRIGHT 2011  *************************************
* 文件名称: lcd_dev.h
* 程序作者: chf
* 程序版本: V1.0
* 编制日期: 2017.02.25
* 功能描述: LCD 低层驱动函数
*******************************************************************************/

#ifndef __LCD_DEV_H__
#define __LCD_DEV_H__

#ifdef  LCD_DEV_H
#define LCD_DEV_ext 
#else 
#define LCD_DEV_ext  extern 
#endif

//引脚宏定义

//OLED模式设置：4线串行模式						  
//-----------------OLED端口定义----------------  	
#define LCD_SPI_PORT		GPIOA
#define LCD_SPI_CS_PIN		GPIO_Pin_4
#define LCD_SPI_SCK_PIN		GPIO_Pin_5
#define LCD_SPI_MISO_PIN	GPIO_Pin_6
#define LCD_SPI_MOSI_PIN	GPIO_Pin_7
//CS
#define LCD_REST_SELECT() 	GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_CS_PIN)		//片选
#define LCD_REST_DESELECT() GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_CS_PIN)
//RS data or cmd
#define LCD_RS_DATA() 		GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_MISO_PIN)		//数据 or 命令选择
#define LCD_RS_CMD() 		GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_MISO_PIN)
//SCK
#define LCD_SCK_Clr() 		GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_SCK_PIN)    //时钟线
#define LCD_SCK_Set() 		GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_SCK_PIN)
//mosi
#define LCD_MOSI_Clr() 		GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_MOSI_PIN)	//数据线
#define LCD_MOSI_Set() 		GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_MOSI_PIN)

typedef enum 
{
    LCD_WRITE_CMD = 0,
    LCD_WRITE_DATA
}LCD_WRITE_TYPE;

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		
//
#define LCD_X_MAX_SIZE  128
#define LCD_Y_MAZ_SIZE  8

#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58 

LCD_DEV_ext	unsigned char LCD_GRAM[LCD_X_MAX_SIZE][LCD_Y_MAZ_SIZE];



//funtion
LCD_DEV_ext	void LCD_Init(void);
LCD_DEV_ext	void LCD_Refresh_Gram(void);
LCD_DEV_ext	void LCD_ShowString(unsigned char x,unsigned char y,char *str);
LCD_DEV_ext	void LCD_Display_Start(void);

#endif
