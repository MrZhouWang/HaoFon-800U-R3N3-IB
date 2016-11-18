
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
//  �������ʾ����������:
//  1)  sys.flag = ON:   ==> �����������ʾ
//  2)  ucShift:             ==> ��ʾ��Xλ�����
//  3)  ed[ucShift].show==> �Ƿ���ʾ��Xλ�����   
///////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
	                    ����ǰ��ʾled����            
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
	 					���������ʾ
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
	��ȡ����ʱ�䣬��λms	 100msΪ��λ
	Input :		none
	Output :	��������ʱ�䱶��
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
	��ȡ����ʱ�䣬��λms	 100msΪ��λ
	Input :		none
	Output :	�������ʱ�䱶��
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
	 					ָʾ����˸����ģʽ
	input:   
	    led_flash_period:      ��˸ʱ����
	    led_flash_NO:          ��˸�����
	    led_flash_mode:        ��˸ģʽ
	output:  none					
*******************************************************************************/
void CMD_led_flash(void)
{
    //LEDָʾ�ƿ���	   
	INT8U	led_sts,led_mode;

	if( (TRUE == Key.b.Short) )//&&(KeyPressLed < MAX_KEYPRESSLED) )
	{ 	// �а������£������F5��������
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
	{  	// �ް�������
		 if( (digital_led_loading == ON) )
			 { led_sts = LED_NONE;    }		   // �������ȴ�ʱ����ʾ
		 else if(ClearFlg == ON) 
		 	 { ClearFlg = OFF; led_sts = led_flash_NO = LED_NONE; }
		 else{ led_sts = led_flash_NO;   led_mode = led_flash_mode; }	
	}


	// �ж��Ƿ����״̬
	if(led_sts == LED_NONE)
	{ led_light_ctrl(LED_NONE);	 }	// ȫ��
	else
	{	// ��·״̬
		if(led_mode == LED_FLASH_MODE_ALWAYS)	// ������ʾ
		{ led_light_ctrl(led_sts); }			// ��������Ӧ��ɫ
		else  if(sys.led_flash_count == 0)		// ��˸��ʾ��ʱ�䵽
		{
			//��ǰ��Ϊ��״̬
			if(led_flash_status == 0)
			{	//��������������л�������״̬���������øı�������ڲ���
				led_light_ctrl(led_sts);				// ��������Ӧ��ɫ
				led_flash_status = 1;					// �л�������״̬
				sys.led_flash_count = GetLedOnTime();	// ȡ����ʱ��
			}
			//��ǰΪ����
			else
			{	//���������������л�������״̬���������øı�������ڲ���
				led_light_ctrl(LED_NONE);				// ȫ��
				led_flash_status = 0;					// �л�������״̬
				sys.led_flash_count = GetLedOffTime();	// ȡ����ʱ��
			}
		}
	}	
}

/*******************************************************************************
								END					
*******************************************************************************/

