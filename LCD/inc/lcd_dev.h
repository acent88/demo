/******************** (C) COPYRIGHT 2011  *************************************
* �ļ�����: lcd_dev.h
* ��������: chf
* ����汾: V1.0
* ��������: 2017.02.25
* ��������: LCD �Ͳ���������
*******************************************************************************/

#ifndef __LCD_DEV_H__
#define __LCD_DEV_H__

#ifdef  LCD_DEV_H
#define LCD_DEV_ext 
#else 
#define LCD_DEV_ext  extern 
#endif

//���ź궨��

//OLEDģʽ���ã�4�ߴ���ģʽ						  
//-----------------OLED�˿ڶ���----------------  	
#define LCD_SPI_PORT		GPIOA
#define LCD_SPI_CS_PIN		GPIO_Pin_4
#define LCD_SPI_SCK_PIN		GPIO_Pin_5
#define LCD_SPI_MISO_PIN	GPIO_Pin_6
#define LCD_SPI_MOSI_PIN	GPIO_Pin_7
//CS
#define LCD_REST_SELECT() 	GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_CS_PIN)		//Ƭѡ
#define LCD_REST_DESELECT() GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_CS_PIN)
//RS data or cmd
#define LCD_RS_DATA() 		GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_MISO_PIN)		//���� or ����ѡ��
#define LCD_RS_CMD() 		GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_MISO_PIN)
//SCK
#define LCD_SCK_Clr() 		GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_SCK_PIN)    //ʱ����
#define LCD_SCK_Set() 		GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_SCK_PIN)
//mosi
#define LCD_MOSI_Clr() 		GPIO_ResetBits(LCD_SPI_PORT,LCD_SPI_MOSI_PIN)	//������
#define LCD_MOSI_Set() 		GPIO_SetBits(LCD_SPI_PORT,LCD_SPI_MOSI_PIN)

typedef enum 
{
    LCD_WRITE_CMD = 0,
    LCD_WRITE_DATA
}LCD_WRITE_TYPE;

//OLED���Դ�
//��Ÿ�ʽ����.
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
