/******************** (C) COPYRIGHT 2010  *************************************
* �ļ�����: lcd_dev.c
* ��������: chf
* ����汾: V1.0
* ��������: 2017.02.25
* ��������: lcd �Ͳ���������
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
* Description    : ����LCD��������Ҫ���¼�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Create_Event(void)
{
    
}

/*******************************************************************************
* Function Name  : LCD_GPIO_Configuration
* Description    : LCD GPIO ��ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = LCD_SPI_CS_PIN | LCD_SPI_SCK_PIN | LCD_SPI_MOSI_PIN | LCD_SPI_MISO_PIN; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
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
* Description    : ��ʼ��LCD�������õ���ȫ�ֱ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Variable_Init(void)
{
	

}

/*******************************************************************************
* Function Name  : LCD_Refresh_Gram
* Description    : �����Դ浽LCD		
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Refresh_Gram(void)
{
	unsigned char i,n;		    
	for(i=0;i<8;i++)  
	{  
		LCD_Write_Byte (0xb0+i,LCD_WRITE_CMD);    //����ҳ��ַ��0~7��
		LCD_Write_Byte (0x00,LCD_WRITE_CMD);      //������ʾλ�á��е͵�ַ
		LCD_Write_Byte (0x10,LCD_WRITE_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<LCD_X_MAX_SIZE;n++)LCD_Write_Byte(LCD_GRAM[n][i],LCD_WRITE_DATA); 
	}  
}
	  	  
/*******************************************************************************
* Function Name  : LCD_Display_On
* Description    : ����OLED��ʾ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display_On(void)
{
	LCD_Write_Byte(0X8D,LCD_WRITE_CMD);  //SET DCDC����
	LCD_Write_Byte(0X14,LCD_WRITE_CMD);  //DCDC ON
	LCD_Write_Byte(0XAF,LCD_WRITE_CMD);  //DISPLAY ON
}
   
/*******************************************************************************
* Function Name  : LCD_Display_Off
* Description    : �ر�OLED��ʾ     
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display_Off(void)
{
	LCD_Write_Byte(0X8D,LCD_WRITE_CMD);  //SET DCDC����
	LCD_Write_Byte(0X10,LCD_WRITE_CMD);  //DCDC OFF
	LCD_Write_Byte(0XAE,LCD_WRITE_CMD);  //DISPLAY OFF
}	

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	 
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
	LCD_Refresh_Gram();//������ʾ
}

/*******************************************************************************
* Function Name  : LCD_Disp_Init
* Description    : LCD��ʾ��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Disp_Init(void)
{			  
	LCD_Write_Byte(0xAE,LCD_WRITE_CMD); //�ر���ʾ
	LCD_Write_Byte(0xD5,LCD_WRITE_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	LCD_Write_Byte(0x50,LCD_WRITE_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	LCD_Write_Byte(0xA8,LCD_WRITE_CMD); //��������·��
	LCD_Write_Byte(0X3F,LCD_WRITE_CMD); //Ĭ��0X3F(1/64) 
	LCD_Write_Byte(0xD3,LCD_WRITE_CMD); //������ʾƫ��
	LCD_Write_Byte(0X00,LCD_WRITE_CMD); //Ĭ��Ϊ0

	LCD_Write_Byte(0x40,LCD_WRITE_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	LCD_Write_Byte(0x8D,LCD_WRITE_CMD); //��ɱ�����
	LCD_Write_Byte(0x14,LCD_WRITE_CMD); //bit2������/�ر�
	LCD_Write_Byte(0x20,LCD_WRITE_CMD); //�����ڴ��ַģʽ
	LCD_Write_Byte(0x02,LCD_WRITE_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	LCD_Write_Byte(0xA1,LCD_WRITE_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	LCD_Write_Byte(0xC0,LCD_WRITE_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	LCD_Write_Byte(0xDA,LCD_WRITE_CMD); //����COMӲ����������
	LCD_Write_Byte(0x12,LCD_WRITE_CMD); //[5:4]����
		 
	LCD_Write_Byte(0x81,LCD_WRITE_CMD); //�Աȶ�����
	LCD_Write_Byte(0xEF,LCD_WRITE_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	LCD_Write_Byte(0xD9,LCD_WRITE_CMD); //����Ԥ�������
	LCD_Write_Byte(0xf1,LCD_WRITE_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	LCD_Write_Byte(0xDB,LCD_WRITE_CMD); //����VCOMH ��ѹ����
	LCD_Write_Byte(0x30,LCD_WRITE_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	LCD_Write_Byte(0xA4,LCD_WRITE_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	LCD_Write_Byte(0xA6,LCD_WRITE_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	LCD_Write_Byte(0xAF,LCD_WRITE_CMD); //������ʾ	 
	LCD_Clear();

}

/*******************************************************************************
* Function Name  : LCD_Init
* Description    : LCD ��ʼ��.
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
* Input          : x:0~127 y:0~63 t:1 ��� 0,���		
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
	unsigned char pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)LCD_GRAM[x][pos]|=temp;
	else LCD_GRAM[x][pos]&=~temp;
}

/*******************************************************************************
* Function Name  : LCD_Fill
* Description    : LCD_Fill
* Input          : x1,y1,x2,y2 �������ĶԽ����� ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63 dot:0,���;1,���	  	 	
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
	LCD_Refresh_Gram();//������ʾ
}

/*******************************************************************************
* Function Name  : LCD_ShowChar
* Description    : ��ָ��λ����ʾһ���ַ�,���������ַ�
* Input          : x:0~127	y:0~63 	mode:0,������ʾ;1,������ʾ size:ѡ������ 16/12 		
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size,unsigned char mode)
{
	unsigned char temp,t,t1;
	unsigned char y0=y;
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //����1206����
		else temp=oled_asc2_1608[chr][t];		 //����1608���� 	                          
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
* Description    : ��ʾ�ַ���
* Input          : x,y:�������  *p:�ַ�����ʼ��ַ
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
* Description    : ����ʱ��ʾ��������
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

