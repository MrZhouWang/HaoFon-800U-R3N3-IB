
#define AP_UART_GLOBALS	
#include "extern_symbol.h" 

//volatile OS_UART xdata uart;

/***************************************************
		T1��ʱ����ʼ��
  											7     6     5     4     3     2     1     0     Reset Value
sfr SCON = 0x98; //Serial Control         SM0/FE SM1   SM2   REN   TB8   RB8    TI    RI    0000,0000
SM0 SM1:
 0   0 	       ͬ����λ�Ĵ���		fosc/12
 0   1		   10λ�첽�շ�         �ɶ�ʱ������
 1   0         11λ�첽�շ�  		fosc/32��fosc/64
 1   1         11λ�첽�շ�         �ɶ�ʱ������
P = PSW.0      P=1��   P=0ż
STC15W408AS û��T1��ʱ��
***************************************************/

void InitT1(void)
{
	SCON = 0xd0;			      	//���пڹ���ģʽ3ѡ��
	PCON  &= 0x7f;                  //SOMD =0�����ӱ�
	//����1�л���P3.0��P3.1
	AUXR1 = 0x40;        		//���ô���1��P3.6�� P3.7��

	AUXR &= ~(1<<4);	//Timer stop
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR &= ~(1<<3);	//Timer2 set As Timer
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (INT8U)((65536-(11059200/4/38400))>>8);    //�趨��ʱ��ֵ
	TL2 = (INT8U)(65536-(11059200/4/38400));		  //�趨��ʱ��ֵ
	IE2  &= ~(1<<2);	//��ֹ�ж�
//	AUXR &= ~(1<<3);	//��ʱ	Timer2 set As Timer
	AUXR |=  (1<<4);	//Timer run enable
	ES   = 1;				        //Enabled serial port interrrup 
}

/*******************************************************************************
	 						���ڳ�ʼ��
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE					
*******************************************************************************/
void UART_Init(void)
{
	InitT1();
	uart.rIndex = 0;
	uart.rFlag  = OFF;
	uart.rTimer = 0;
	
	//UART �շ�ָʾ��IO����
//	p_led_txd = 1;
//	p_led_rxd = 1;	
}


/*******************************************************************************
	 						���ڳ�ʼ��1
	input :     none
	default:
	    data bits:  8
	    start bits: 1
	    stop bits:  1
	    parity:     NONE	
						
*******************************************************************************/
#if 0
void UART1_Init(void)
{	
	SCON = 0x50;			      	//���пڹ���ģʽ1ѡ��
	PCON  &= 0x7f;                  //SOMD =0�����ӱ�
	//����1�л���P3.0��P3.1
	AUXR1 &= 0x3F;					//bit7=0��bit6=0
	 	
	AUXR &= ~(1<<4);	//Timer stop
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR &= ~(1<<3);	//Timer2 set As Timer
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (INT8U)((65536-(11059200/4/38400))>>8);    //�趨��ʱ��ֵ
	TL2 = (INT8U)(65536-(11059200/4/38400));		  //�趨��ʱ��ֵ
	IE2  &= ~(1<<2);	//��ֹ�ж�
	AUXR &= ~(1<<3);	//��ʱ	Timer2 set As Timer
	AUXR |=  (1<<4);	//Timer run enable
}
#endif
/*******************************************************************************
	 					 �����жϺ���
	input:   none     
	output:  none					
*******************************************************************************/
#define RCV_OVER_TIMER	2

void SCON_int (void) interrupt 4
{

	if (RI)
	{
		RI = 0;

		if ( (uart.rIndex < UART_BUF_LEN)&&(uart.rFlag == OFF) )
		{
			uart.rBuffer[uart.rIndex++] = SBUF;	
			uart.rTimer = RCV_OVER_TIMER;
		}
		
	}
//	p_Test = 1;
}
/***********************************************
        ���ڷ��ͳ���
input :     *Ram    
            len     
output :    NONE
***********************************************/
#if 0
void SendSerialByte1(UINT8 *Ram, UINT16 len)
{ 

    for(;len>0;len--)
    {

		TI = 0;
        SBUF = *Ram;           
        while(!TI){;}             
        TI = 0;
        Ram++;
    }
}
#endif
/***********************************************
        ���ڷ��ͳ���
input :     *Ram    
            len     
output :    NONE
***********************************************/
void SendSerialByte(UINT8 *Ram, UINT16 len)
{ 
//	p_led_txd = 0;
	p_m485_ctrl = 1;
    for(;len>0;len--)
    {
		ACC = *Ram;
		if(P == 0)
		{
			TB8 = 0;
		}
		else
		{
			TB8 =1;
		}

        SBUF = *Ram;           
        while(!TI){;}             
        TI = 0;
        Ram++;
    }
	p_m485_ctrl = 0;
//	p_led_txd = 1;

}
/*******************************************************************************
								END					
*******************************************************************************/
