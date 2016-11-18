/********************************************************************************
����״̬�� 

      ʹ��     ����	    ��        ���
			
			 0		   ����		  ��    	   ��
			 0		   �ɿ�		  ��	       ��
			 1		   �ɿ�		  ��	       ��
			 1		   ����		  ��         ��	   

ʹ�ܣ�1 ʹ������   0 ��ʹ������


��׼�����ڶ�ʱ�����ϵ缸����������ɣ���ƽʱ������������һ��

2016.09.18  ��Ӧ�뿪��ʱ100ms��ɫ�л�
2016.10.27  ����״̬�Ա�ֵ���þ���ֵ
2016.10.27  ��׼�������ϵ��ʱ��ȡ100��ƽ��ֵ ��׼��׼ֵ��1.35V���� �����׼ֵ���������Χ�����Ƴ����ڱ���״̬
2016.10.28  ��׼���������쳣�豸�������Ӧ���쳣ָʾ
********************************************************************************/

#define MAIN_GLOBAL
#include "include.h"			

bit f_prossout= FALSE;
bit valid_ok = FALSE;
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ϵͳƵ������
//// ������� : 
//// ���ز��� : 
//// ˵��     : 
//////////////////////////////////////////////////////////////////////////////////
void OSCILLATOR_Init (void)
{   	
	INT8U	i;
	OSCICN   = 0x06;			          //Ƶ��Ϊ24.5Mhz	/2 =12.25
	for(i = 500; i>0; i--);
	while((OSCICN & 0x10) == 0) ;		// �ж��Ƿ�������
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ������
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void main(void)
{	
	
	PCA0MD &= ~0x40; 		         //���Ź���ʱ������ֹ
	RSTSRC |= 0x02;					     //ʹ��VDD��ѹ����
	
	EA = 0;                      //��������������ֹ�ж�
	EA = 0;
	
	PORT_Init ();					  		 // Initialize crossbar and GPIO
	delay_ms(100);               
		
  OSCILLATOR_Init();			     // Initialize the system clock		
	Timer0_Init();						   // Timer0 �Զ���װģʽ2
	Timer2_Init();               // Timer2 Initialization
	UART485_Init();              // ���ڳ�ʼ��
	ADC_Init ();					   		 // ����ADC



	Base_ADC();                  //��׼����
	LoadParaFromFlash();	       // ��ȡ������ֵ
	led_blink();
  sys_data_reset();            //ϵͳ������λ
  
 	
	

	
	
	Display_Clear();
	CurDispMode = DISPMODE_NONE;	
	M485_CTRL_OFF;						                   // �رշ���DE�˿ڣ��򿪽���/RE485�˿�
	led_flash_period = 1000;						         // ��˸���ڿ���
	led_flash_mode 	 = LED_FLASH_MODE_ALWAYS ;	 // ����ģʽ
	TaskStats 		 = TASK_NONE;		               // �ǹ���״̬
	ClearFlg	   = OFF;
	Key.b.Short    = FALSE;				               // �̰��ͷ�
	Key.b.LongOnce = FALSE;				               // ����һ���ͷ�	
	Key.b.Long 	   = FALSE;				               // �����ͷ�
	f_KeyEnable    = TRUE;                       // ʹ�ܰ���
	SystemLocked   = OFF;				                 // ϵͳ����
	CurDispMode    = DISPMODE_NONE;  	           // ��ǰ����״̬��Ϊ������״̬
	LED_MultiCast_Mode =OFF;                     // �鲥ģʽ��
	para.str.FinishedLED=LED_NONE;               // F1��ʼ��Ϊ��
	
	
	
	Interrupt_Init ();			     // ���ж�
	
  
	while(1)
	{
		PCA0CPH2 =   1;                              //ι��
		// 1���źŵ���3 ��ledָʾ����˸����
		if(AD_Base_OK != OK)                       //�ɼ���׼��ѹʧ�� ָʾ 
		{
			 if(ADC_BaseErrNum == 0)
	        ADC_BASE_ERR_BLINK();		
		}
		Key_Scan();                                //����ɨ�账�����
		CMD_led_flash();                           //����˸�����Լ���ɫ����
       
		
                		
		                                           //������״̬
		if (uart.rFlag == ON)
		{  
     // Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 	
			Cmd_Process();                           //���������·�����
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
		                                           //��ʾLOADING ���� '-' '--' '---' '----' '-----' '------'
		if((digital_led_loading == ON))
		{
			if(sys.digital_led_load_cnt == 0)
			{
				digital_led_loading = OFF;
				sys.beWaitNewData = OFF;                 
			}
		}
		PCA0CPH2 = 1;                              //ι��			
	} 
}


