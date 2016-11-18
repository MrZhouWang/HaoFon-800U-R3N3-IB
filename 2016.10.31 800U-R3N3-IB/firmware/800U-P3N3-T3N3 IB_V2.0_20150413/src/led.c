
#define AP_LED_GLOBALS	
#include "extern_symbol.h" 


///////////////////////////////////////////////////////////////////////////////
//   -----         -----        -----         -----         -----       -----
//  |       |       |       |       |       |       |       |       |       |       |       |
//  |       |       |       |       |       |       |       |       |       |       |       |
//   -----         -----        -----         -----         -----         ----- 
//  |       |       |       |       |       |       |       |       |       |       |       |
//  |       |       |       |       |       |       |       |       |       |       |       |
//   -----         -----        -----         -----         -----      ----- 
//      
//      1                 2               3               4                 5               6
//  数码管显示程序处理流程:
//  1)  sys.flag = ON:   ==> 允许数码管显示
//  2)  ucShift:             ==> 显示第X位数码管
//  3)  ed[ucShift].show==> 是否显示第X位数码管   
///////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
	                    给当前显示led供电            
    Input :	 UINT8 ucValue   //LED_RED, LED_GREEN, LED_BLUE 
    Output:  None	
*******************************************************************************/
void led_light_ctrl(UINT8 ucValue)
{
	switch (ucValue)
	{
		case LED_RED:  		LED_G_OFF; LED_B_OFF;  LED_R_ON;  break;		
		case LED_GREEN:		LED_R_OFF; LED_B_OFF;  LED_G_ON;  break;
		case LED_BLUE:		LED_R_OFF; LED_G_OFF;  LED_B_ON;  break;
		
		case LED_RG:  		LED_B_OFF; LED_R_ON;   LED_G_ON;  break;		
		case LED_RB:		LED_G_OFF; LED_R_ON;   LED_B_ON;  break;
		case LED_GB:		LED_R_OFF; LED_G_ON;   LED_B_ON;  break;
		case LED_RGB:		LED_R_ON;  LED_G_ON;   LED_B_ON;  break;			
		default:			LED_R_OFF; LED_G_OFF; LED_B_OFF;break;
	}
}
/*******************************************************************************
	 					清除所有显示
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Clear(void)
{
   	if( (TRUE == Key.b.Short) )
	{
	   	ClearFlg = ON;
	}
   	else
	{
    	ClearFlg = OFF;
		led_flash_NO = LED_NONE;
		led_light_ctrl(LED_NONE);
	}
}



/*******************************************************************************
	获取灯亮时间，单位ms	 100ms为单位
	Input :		none
	Output :	返回亮的时间倍数
*******************************************************************************/

UINT16 GetLedOnTime()
{
	UINT16 unRet ;
	switch(led_flash_mode)
	{
		case LED_FLASH_MODE_2V1:	unRet = led_flash_period*2;	break;
		case LED_FLASH_MODE_5V1:	unRet = led_flash_period*5;	break;
		case LED_FLASH_MODE_10V1:	unRet = led_flash_period*10;break;
		default:					unRet = led_flash_period;	break;
	}
	return(unRet);
}
/*******************************************************************************
	获取灯灭时间，单位ms	 100ms为单位
	Input :		none
	Output :	返回灭的时间倍数
*******************************************************************************/

UINT16 GetLedOffTime()
{
	UINT16 unRet ;
	switch(led_flash_mode)
	{
		case LED_FLASH_MODE_1V2:	unRet = led_flash_period*2;	break;
		case LED_FLASH_MODE_1V5:	unRet = led_flash_period*5;	break;
		case LED_FLASH_MODE_1V10:	unRet = led_flash_period*10;break;
		default:					unRet = led_flash_period;	break;
	}
	return(unRet);
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
    //LED指示灯控制	   
	INT8U	led_sts,led_mode;

	if( (TRUE == Key.b.Short) )//&&(KeyPressLed < MAX_KEYPRESSLED) )
	{ 	// 有按键按下，则根据F5参数亮灯
		led_mode = LED_FLASH_MODE_ALWAYS;

		if(para.str.KeyPressLed >= MAX_KEYPRESSLED) 
		{
		  led_sts = led_flash_NO+1;
		  if(led_sts >= MAX_KEYPRESSLED)
		  { led_sts = LED_RED; }
		}
		else
		{ led_sts = para.str.KeyPressLed; }//KeyPressLed;  }      
	}							
	else
	{  	// 无按键按下
		 if( (digital_led_loading == ON) )
			 { led_sts = LED_NONE;    }		   // 进度条等待时灭显示
		 else if(ClearFlg == ON) 
		 	 { ClearFlg = OFF; led_sts = led_flash_NO = LED_NONE; }
		 else{ led_sts = led_flash_NO;   led_mode = led_flash_mode; }	
	}


	// 判断是否点亮状态
	if(led_sts == LED_NONE)
	{ led_light_ctrl(LED_NONE);	 }	// 全灭
	else
	{	// 电路状态
		if(led_mode == LED_FLASH_MODE_ALWAYS)	// 常亮显示
		{ led_light_ctrl(led_sts); }			// 仅点亮对应颜色
		else  if(sys.led_flash_count == 0)		// 闪烁显示，时间到
		{
			//当前灯为灭状态
			if(led_flash_status == 0)
			{	//灯灭计数器到，切换到灯亮状态，并且设置改变灯亮周期参数
				led_light_ctrl(led_sts);				// 仅点亮对应颜色
				led_flash_status = 1;					// 切换到灯亮状态
				sys.led_flash_count = GetLedOnTime();	// 取灯亮时间
			}
			//当前为亮灯
			else
			{	//灯亮计数器到，切换到灯灭状态，并且设置改变灯灭周期参数
				led_light_ctrl(LED_NONE);				// 全灭
				led_flash_status = 0;					// 切换到灯灭状态
				sys.led_flash_count = GetLedOffTime();	// 取灯灭时间
			}
		}
	}	
}

/*******************************************************************************
								END					
*******************************************************************************/

