
#define AP_UART_GLOBALS	
#include "extern_symbol.h" 

//volatile OS_UART xdata uart;

/***************************************************
		T1定时器初始化
  											7     6     5     4     3     2     1     0     Reset Value
sfr SCON = 0x98; //Serial Control         SM0/FE SM1   SM2   REN   TB8   RB8    TI    RI    0000,0000
SM0 SM1:
 0   0 	       同步移位寄存器		fosc/12
 0   1		   10位异步收发         由定时器控制
 1   0         11位异步收发  		fosc/32或fosc/64
 1   1         11位异步收发         由定时器控制
P = PSW.0      P=1奇   P=0偶
STC15W408AS 没有T1定时器
***************************************************/

void InitT1(void)
{
	SCON = 0xd0;			      	//串行口工作模式3选择
	PCON  &= 0x7f;                  //SOMD =0，不加倍
	//串口1切换到P3.0和P3.1
	AUXR1 = 0x40;        		//设置串口1在P3.6和 P3.7中

	AUXR &= ~(1<<4);	//Timer stop
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR &= ~(1<<3);	//Timer2 set As Timer
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (INT8U)((65536-(11059200/4/38400))>>8);    //设定定时初值
	TL2 = (INT8U)(65536-(11059200/4/38400));		  //设定定时初值
	IE2  &= ~(1<<2);	//禁止中断
//	AUXR &= ~(1<<3);	//定时	Timer2 set As Timer
	AUXR |=  (1<<4);	//Timer run enable
	ES   = 1;				        //Enabled serial port interrrup 
}

/*******************************************************************************
	 						串口初始化
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
	
	//UART 收发指示灯IO配置
//	p_led_txd = 1;
//	p_led_rxd = 1;	
}


/*******************************************************************************
	 						串口初始化1
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
	SCON = 0x50;			      	//串行口工作模式1选择
	PCON  &= 0x7f;                  //SOMD =0，不加倍
	//串口1切换到P3.0和P3.1
	AUXR1 &= 0x3F;					//bit7=0，bit6=0
	 	
	AUXR &= ~(1<<4);	//Timer stop
	AUXR |= 0x01;		//S1 BRT Use Timer2;
	AUXR &= ~(1<<3);	//Timer2 set As Timer
	AUXR |=  (1<<2);	//Timer2 set as 1T mode
	TH2 = (INT8U)((65536-(11059200/4/38400))>>8);    //设定定时初值
	TL2 = (INT8U)(65536-(11059200/4/38400));		  //设定定时初值
	IE2  &= ~(1<<2);	//禁止中断
	AUXR &= ~(1<<3);	//定时	Timer2 set As Timer
	AUXR |=  (1<<4);	//Timer run enable
}
#endif
/*******************************************************************************
	 					 串口中断函数
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
        串口发送程序
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
        串口发送程序
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
