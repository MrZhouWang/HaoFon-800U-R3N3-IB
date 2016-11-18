#define LED_GLOBAL
#include "include.h"

INT8U LedStatus = LED_NONE;   //灯状态
INT16U  LedDelayCnt = 0;
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : LED上电完成灯闪烁
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void led_blink(void)
{
	LED_R_ON ;
	delay_ms(1000);	
	LED_R_OFF ;	


	LED_G_ON ;	
	delay_ms(1000);
	LED_G_OFF ;
		
	LED_B_ON;	
	delay_ms(1000);	
	LED_G_OFF ;
	
	
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : ADC基准电压采集错误LED灯闪烁 机制
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void ADC_BASE_ERR_BLINK(void)
{
			  
	      
				if(ADC_BaseLedState == ON)
				{
					 ADC_BASE_ERR_ON();
					 ADC_BaseLedState = OFF;
					 
					 ADC_BaseErrNum = ADC_BaseErrNumIintON;    //赋值LED闪烁时间 在定时器里进行减值
				}
				else if(ADC_BaseLedState == OFF)
				{
					 ADC_BASE_ERR_OFF();
					 ADC_BaseLedState = ON;
					 
					 ADC_BaseErrNum = ADC_BaseErrNumIintOFF;   //赋值LED闪烁时间 在定时器里进行减值
				}
				//ADC_BaseErrNu=0 ;
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : ADC基准电压采集错误LED灯闪烁
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void ADC_BASE_ERR_ON(void)
{
	  LED_R_ON ;
	  LED_B_OFF;
	  LED_G_OFF;
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : ADC基准电压采集错误LED灯闪烁
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void ADC_BASE_ERR_OFF(void)
{
	  LED_R_OFF ;
	  LED_B_OFF;
	  LED_G_OFF;
}

//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : IO初始化
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void PORT_Init (void)
{	
	P0MDIN  &= ~0x02;        	// P0.1,为模拟输入 	0=模拟输入，1=数字输入
	P0MDIN  |= 0x10;          // P0.4 DIGITAL INPUT
	P0MDOUT |= 0x0C;	        // P0.2,3,为推挽输出方式	
	P0MDOUT &= ~(0xC1);	      // P0.0,6,7为Open Drain输出方式	
  XBR0  	|= 0x4F;  	      // 跳过使用的IO口P0.0 1 2 3 6 7 	
	XBR1    |= 0x03;          // 把UART TX映射到P0.4 RX映射到P0.5
	XBR2    |= 0X40;	        // 使能交叉开关，使能弱上拉

  

  led_light_ctrl(LED_NONE); // 关掉LED
	   
	pEMILEDCTL = OFF;         // 发射管关断
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : led控制
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void led_light_ctrl(INT8U leds)
{
	switch (leds)
	{
		case LED_RED:  		LED_G_OFF; LED_B_OFF;  LED_R_ON;  break;		
		case LED_GREEN:		LED_R_OFF; LED_B_OFF;  LED_G_ON;  break;
		case LED_BLUE:		LED_R_OFF; LED_G_OFF;  LED_B_ON;  break;
		
		case LED_RG:  		LED_B_OFF; LED_R_ON;   LED_G_ON;  break;		
		case LED_RB:		  LED_G_OFF; LED_R_ON;   LED_B_ON;  break;
		case LED_GB:		  LED_R_OFF; LED_G_ON;   LED_B_ON;  break;
		case LED_RGB:		  LED_R_ON;  LED_G_ON;   LED_B_ON;  break;			
		default:			    LED_R_OFF; LED_G_OFF; LED_B_OFF;  break;
	}
}
/*******************************************************************************
	 					指示灯闪烁控制模式
	input:   
	    led_flash_period:      闪烁时间间隔
	    led_flash_NO:          闪烁灯序号
	    led_flash_mode:        闪烁模式
	output:  none					
*******************************************************************************/
void CMD_led_flash(void)
{
/***********************LED指示灯控制*********************/	   
	INT8U	led_sts,led_mode;
	if( (TRUE == Key.b.Short))//&&(task_unfinish_num == OFF)&&(digital_led_loading == OFF))  
	{ 	 	
		
		led_mode = LED_FLASH_MODE_ALWAYS;
		if(para.str.KeyPressLed >= MAX_KEYPRESSLED)    // 有按键按下，则根据F5参数亮灯
		{
		  led_sts = led_flash_NO+1;
		  if(led_sts >= MAX_KEYPRESSLED)
		  { led_sts = LED_RED; }
		}
		else
		{ led_sts = para.str.KeyPressLed; }    
	}							
	else 
	{  	                                             // 无按键按下
		 if( (digital_led_loading == ON) )
			 { led_sts = LED_NONE;    }		               // 进度条等待时灭显示
		 else if(ClearFlg == ON)                       //初始化清除的时候led_flash_NO=LED_NONE; 
		 	 { MulticastStatus=LED_NONE; led_flash_NO=led_sts = LED_NONE; ClearFlg = OFF;}	 
		 else{ led_sts = led_flash_NO;   led_mode = led_flash_mode; }	     
	}
	
/******************设置灯色 *******************************************/
	if(led_sts == LED_NONE)
	{ led_light_ctrl(LED_NONE);	 }	                      // 全灭
	else
	{	
		if(led_mode == LED_FLASH_MODE_ALWAYS)	              // 常亮显示
		{ led_light_ctrl(led_sts); }			                  // 仅点亮对应颜色
		else  if(sys.led_flash_count == 0)		              // 闪烁显示，时间到
		{		                                        
			if(led_flash_status == 0)
			{	                                                //灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				led_light_ctrl(led_sts);				                // 仅点亮对应颜色
				led_flash_status = 1;					                  // 切换到灯亮状态
				sys.led_flash_count = GetLedOnTime();	          // 取灯亮时间
			}
			else
			{	                                                //灯亮计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				led_light_ctrl(LED_NONE);			                 	// 全灭
				led_flash_status = 0;					                  // 切换到灯灭状态
				sys.led_flash_count = GetLedOffTime();	        // 取灯灭时间
			}
		}
	}	
}
/*******************************************************************************
	获取灯亮时间，单位ms	 100ms为单位
	Input :		none
	Output :	返回亮的时间倍数
*******************************************************************************/

INT16U GetLedOnTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS	:						                   //常亮模式，不会调用此函数
		break;			
	case LED_FLASH_MODE_1V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V2:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V5:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V10:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_2V1:
		unRet = led_flash_period*2;
		break;
	case LED_FLASH_MODE_5V1:
		unRet = led_flash_period*5;
		break;
	case LED_FLASH_MODE_10V1:
		unRet = led_flash_period*10;
		break;
	default:
		break;
	}
	return unRet;
}
/*******************************************************************************
	获取灯灭时间，单位ms	 100ms为单位
	Input :		none
	Output :	返回灭的时间倍数
*******************************************************************************/

INT16U GetLedOffTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS:					                            //常亮模式，不会调用此函数
		break;				
	case LED_FLASH_MODE_1V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_2V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_5V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_10V1:
		unRet = led_flash_period;
		break;
	case LED_FLASH_MODE_1V2:
		unRet = led_flash_period*2;
		break;
	case LED_FLASH_MODE_1V5:
		unRet = led_flash_period*5;
		break;
	case LED_FLASH_MODE_1V10:
		unRet = led_flash_period*10;
		break;
	default:
		break;
	}
	return unRet;
}

