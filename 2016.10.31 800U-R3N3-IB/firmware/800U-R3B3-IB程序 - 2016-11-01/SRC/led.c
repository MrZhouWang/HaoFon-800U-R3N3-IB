#define LED_GLOBAL
#include "include.h"

INT8U LedStatus = LED_NONE;   //��״̬
INT16U  LedDelayCnt = 0;
//////////////////////////////////////////////////////////////////////////////////
//// �������� : LED�ϵ���ɵ���˸
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
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
//// �������� : ADC��׼��ѹ�ɼ�����LED����˸ ����
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void ADC_BASE_ERR_BLINK(void)
{
			  
	      
				if(ADC_BaseLedState == ON)
				{
					 ADC_BASE_ERR_ON();
					 ADC_BaseLedState = OFF;
					 
					 ADC_BaseErrNum = ADC_BaseErrNumIintON;    //��ֵLED��˸ʱ�� �ڶ�ʱ������м�ֵ
				}
				else if(ADC_BaseLedState == OFF)
				{
					 ADC_BASE_ERR_OFF();
					 ADC_BaseLedState = ON;
					 
					 ADC_BaseErrNum = ADC_BaseErrNumIintOFF;   //��ֵLED��˸ʱ�� �ڶ�ʱ������м�ֵ
				}
				//ADC_BaseErrNu=0 ;
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ADC��׼��ѹ�ɼ�����LED����˸
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void ADC_BASE_ERR_ON(void)
{
	  LED_R_ON ;
	  LED_B_OFF;
	  LED_G_OFF;
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ADC��׼��ѹ�ɼ�����LED����˸
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void ADC_BASE_ERR_OFF(void)
{
	  LED_R_OFF ;
	  LED_B_OFF;
	  LED_G_OFF;
}

//////////////////////////////////////////////////////////////////////////////////
//// �������� : IO��ʼ��
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void PORT_Init (void)
{	
	P0MDIN  &= ~0x02;        	// P0.1,Ϊģ������ 	0=ģ�����룬1=��������
	P0MDIN  |= 0x10;          // P0.4 DIGITAL INPUT
	P0MDOUT |= 0x0C;	        // P0.2,3,Ϊ���������ʽ	
	P0MDOUT &= ~(0xC1);	      // P0.0,6,7ΪOpen Drain�����ʽ	
  XBR0  	|= 0x4F;  	      // ����ʹ�õ�IO��P0.0 1 2 3 6 7 	
	XBR1    |= 0x03;          // ��UART TXӳ�䵽P0.4 RXӳ�䵽P0.5
	XBR2    |= 0X40;	        // ʹ�ܽ��濪�أ�ʹ��������

  

  led_light_ctrl(LED_NONE); // �ص�LED
	   
	pEMILEDCTL = OFF;         // ����ܹض�
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : led����
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
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
	 					ָʾ����˸����ģʽ
	input:   
	    led_flash_period:      ��˸ʱ����
	    led_flash_NO:          ��˸�����
	    led_flash_mode:        ��˸ģʽ
	output:  none					
*******************************************************************************/
void CMD_led_flash(void)
{
/***********************LEDָʾ�ƿ���*********************/	   
	INT8U	led_sts,led_mode;
	if( (TRUE == Key.b.Short))//&&(task_unfinish_num == OFF)&&(digital_led_loading == OFF))  
	{ 	 	
		
		led_mode = LED_FLASH_MODE_ALWAYS;
		if(para.str.KeyPressLed >= MAX_KEYPRESSLED)    // �а������£������F5��������
		{
		  led_sts = led_flash_NO+1;
		  if(led_sts >= MAX_KEYPRESSLED)
		  { led_sts = LED_RED; }
		}
		else
		{ led_sts = para.str.KeyPressLed; }    
	}							
	else 
	{  	                                             // �ް�������
		 if( (digital_led_loading == ON) )
			 { led_sts = LED_NONE;    }		               // �������ȴ�ʱ����ʾ
		 else if(ClearFlg == ON)                       //��ʼ�������ʱ��led_flash_NO=LED_NONE; 
		 	 { MulticastStatus=LED_NONE; led_flash_NO=led_sts = LED_NONE; ClearFlg = OFF;}	 
		 else{ led_sts = led_flash_NO;   led_mode = led_flash_mode; }	     
	}
	
/******************���õ�ɫ *******************************************/
	if(led_sts == LED_NONE)
	{ led_light_ctrl(LED_NONE);	 }	                      // ȫ��
	else
	{	
		if(led_mode == LED_FLASH_MODE_ALWAYS)	              // ������ʾ
		{ led_light_ctrl(led_sts); }			                  // ��������Ӧ��ɫ
		else  if(sys.led_flash_count == 0)		              // ��˸��ʾ��ʱ�䵽
		{		                                        
			if(led_flash_status == 0)
			{	                                                //��������������л�������״̬���������øı�������ڲ���
				led_light_ctrl(led_sts);				                // ��������Ӧ��ɫ
				led_flash_status = 1;					                  // �л�������״̬
				sys.led_flash_count = GetLedOnTime();	          // ȡ����ʱ��
			}
			else
			{	                                                //���������������л�������״̬���������øı�������ڲ���
				led_light_ctrl(LED_NONE);			                 	// ȫ��
				led_flash_status = 0;					                  // �л�������״̬
				sys.led_flash_count = GetLedOffTime();	        // ȡ����ʱ��
			}
		}
	}	
}
/*******************************************************************************
	��ȡ����ʱ�䣬��λms	 100msΪ��λ
	Input :		none
	Output :	��������ʱ�䱶��
*******************************************************************************/

INT16U GetLedOnTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS	:						                   //����ģʽ��������ô˺���
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
	��ȡ����ʱ�䣬��λms	 100msΪ��λ
	Input :		none
	Output :	�������ʱ�䱶��
*******************************************************************************/

INT16U GetLedOffTime()
{
	UINT16 unRet = led_flash_period;
	switch(led_flash_mode)
	{
	case LED_FLASH_MODE_ALWAYS:					                            //����ģʽ��������ô˺���
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

