#include "include.h"
//unsigned int test_count=0;
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ��0��ʼ��
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void Timer0_Init(void)			 // 30us
{
	TR0 = 1;          //Timer0����,��TCON=0x10;
	ET0 = 1;          //T0����ж�����
 	TH0 = (255-8);		//10US 
 	TMOD = 2;			    //��ʱ��0 8bit�Զ���װ��
 	CKCON |= 0x01;		//4��Ƶ��  12.5/4
	EA =0;
	EA =0;
	//PT0 = 1;         //Timer0 Set High Interrupt Priority Level
}

//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ��1��ʼ��
//// ������� : 
//// ���ز��� : 
//// ˵��     : ��ΪUART��ʼ�ջ�׼ ������ģʽ�� 8 wei �Զ���װ
//////////////////////////////////////////////////////////////////////////////////
void Timer1_Init(void)			 
{
	TR1 = 1;          //Timer1����,��TCON=0x40;
	ET1 = 0;          //T1����жϽ�ֹ
	TMOD |=0x20;			//��ʱ��1 8bit�Զ���װ��
	
	CKCON &= ~0x10;		//Timer1ʱ��ѡ��ΪSystemʱ��
 	TH1 = 0XD8;
 	
 	
}

//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ��0�ж���ں���
//// ������� : 
//// ���ز��� : 
//// ˵��     : 1ms�ɼ�һ�� ��������20�˴��ڻ�׼��ѹ����ʾ�д���
//////////////////////////////////////////////////////////////////////////////////
void Timer0(void) interrupt 1 				 //10us
{
	count++;					                   //��10us����				

  if(LedDelayCnt>0)                    //�����ضϼ�ʱ
		 LedDelayCnt--;
	

		 if(ADC_BaseErrNum>0)                 
	   {
		    ADC_BaseErrNum--;
	   }

	
/********************���䴦��****************************************************/	

	if(count>FSLEDONTIME)                 //��������
	{	
			
			pEMILEDCTL = ON;			            // ����ܴ�
      count=0;
      valid_ok = TRUE;
	}
	else
	{
		 if(valid_ok == TRUE)
	   {
			// LED_B_ON;
			 sampDat = ADC_Samp(); 	         // �ɼ��߶�����	
      // LED_B_OFF;		 
		   f_prossout = TRUE;               //����һ�����
			 pEMILEDCTL = OFF;			           // ����ܹر�
			 
      
    
	   }
		 pEMILEDCTL = OFF;			             // ����ܹر�
	   valid_ok = FALSE;
	}

	}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ��2��ʼ��
//// ������� : 
//// ���ز��� : 
//// ˵��     : in 16-bit reload mode   2ms
//////////////////////////////////////////////////////////////////////////////////
void Timer2_Init(void)			 
{
   TR2 = 1;         //Timer2 Enable
	 T2XCLK = 0;      //Timer2 external clock selection is the system clock divided by 12
	 TMR2RLL = 0X58;  //Timer2 Reload Register Low Byte
   TMR2RLH = 0xF8;  //Timer2 Reload Register High Byte  
	
	 TMR2L = TMR2RLL; //Timer2 Low Byte
	 TMR2H = TMR2RLH; //Timer2 High Byte
	 ET2 = 1;         //Enable Timer2 Interrupt
	 PT2 = 1;         //Timer2 Set High Interrupt Priority Level
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ��2�ж���ں���
//// ������� : 
//// ���ز��� : 
//// ˵��     : 2ms
//////////////////////////////////////////////////////////////////////////////////
void Timer2(void) interrupt 5				 //2ms
{


	
	    //�ݼ����������Լ�1
    if (sys.digital_led_load_cnt != 0)
        sys.digital_led_load_cnt--;    
    
    //�ݼ����������Լ�1
//    if (sys.count != 0)
//        sys.count--;

    //�ݼ����������Լ�1
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;
	
	
	  if (uart.rIndex != 0)	 		     //����1-485������������	
    {
			  
        if (uart.rTimer-- == 0) 
        {
			     uart.rFlag  = ON;
		    }
       TR0 = 0;
    }
		else 
			 TR0 = 1;
		
		
		
		TF2H = 0; //Timer2 software clear
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ʱ��0����
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void Interrupt_Init (void)
{
	EA = 1; //�ж������ܿ���λ
}