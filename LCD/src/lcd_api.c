/******************** (C) COPYRIGHT 2010  *************************************
* 文件名称: lcd_api.c
* 程序作者: chf
* 程序版本: V1.0
* 编制日期: 2017.02.25
* 功能描述: LCD 接口函数
*******************************************************************************/

#define	LCD_API_H

#include "stm32f10x.h"
#include  <os.h>
#include "app_cfg.h"
#include "lcd_dev.h"
#include "lcd_api.h"
#include "BQ2589x_dev.h"
#include <stdio.h>
#include <string.h>

BQ2589x_DEV_ext	unsigned int  BQ2589x_ReadBatVoltage( void );
BQ2589x_DEV_ext	unsigned int  BQ2589x_ReadSysVoltage( void );
BQ2589x_DEV_ext	unsigned int  BQ2589x_ReadTSVoltage( void );
BQ2589x_DEV_ext	unsigned int  BQ2589x_ReadVBUSVoltage( void );
BQ2589x_DEV_ext	unsigned int  BQ2589x_ReadChargeCurrent( void );

/*******************************************************************************
* Function Name  : LCD_Display
* Description    : LCD_Display
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Display(void)
{
	OS_ERR err;
	unsigned int BatVoltage,SysVoltage,TSVoltage,VBUSVoltage,ChargeCurrent;
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

/*******************************************************************************
* Function Name  : Interface_task
* Description    : Interface task
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_task(void *p_arg)
{	
	OS_ERR err;
	//外设初始化
	LCD_Init();
	//
    for(;;)
    {
		OSTimeDly( OS_TICKS_PER_SEC, OS_OPT_TIME_TIMEOUT, &err );
		//
		MCU_LED_TOG();
		
		LCD_Display();
    }
}


