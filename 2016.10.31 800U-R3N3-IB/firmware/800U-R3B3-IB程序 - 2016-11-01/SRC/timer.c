#include "include.h"
//unsigned int test_count=0;
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 定时器0初始化
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void Timer0_Init(void)			 // 30us
{
	TR0 = 1;          //Timer0启动,即TCON=0x10;
	ET0 = 1;          //T0溢出中断允许
 	TH0 = (255-8);		//10US 
 	TMOD = 2;			    //定时器0 8bit自动重装载
 	CKCON |= 0x01;		//4分频率  12.5/4
	EA =0;
	EA =0;
	//PT0 = 1;         //Timer0 Set High Interrupt Priority Level
}

//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 定时器1初始化
//// 输入参数 : 
//// 返回参数 : 
//// 说明     : 作为UART的始终基准 工作在模式二 8 wei 自动重装
//////////////////////////////////////////////////////////////////////////////////
void Timer1_Init(void)			 
{
	TR1 = 1;          //Timer1启动,即TCON=0x40;
	ET1 = 0;          //T1溢出中断禁止
	TMOD |=0x20;			//定时器1 8bit自动重装载
	
	CKCON &= ~0x10;		//Timer1时钟选择为System时钟
 	TH1 = 0XD8;
 	
 	
}

//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 定时器0中断入口函数
//// 输入参数 : 
//// 返回参数 : 
//// 说明     : 1ms采集一次 连续采样20此大于基准电压，表示有触发
//////////////////////////////////////////////////////////////////////////////////
void Timer0(void) interrupt 1 				 //10us
{
	count++;					                   //对10us计数				

  if(LedDelayCnt>0)                    //按键关断计时
		 LedDelayCnt--;
	

		 if(ADC_BaseErrNum>0)                 
	   {
		    ADC_BaseErrNum--;
	   }

	
/********************发射处理****************************************************/	

	if(count>FSLEDONTIME)                 //持续发射
	{	
			
			pEMILEDCTL = ON;			            // 发射管打开
      count=0;
      valid_ok = TRUE;
	}
	else
	{
		 if(valid_ok == TRUE)
	   {
			// LED_B_ON;
			 sampDat = ADC_Samp(); 	         // 采集高端数据	
      // LED_B_OFF;		 
		   f_prossout = TRUE;               //发射一次完成
			 pEMILEDCTL = OFF;			           // 发射管关闭
			 
      
    
	   }
		 pEMILEDCTL = OFF;			             // 发射管关闭
	   valid_ok = FALSE;
	}

	}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 定时器2初始化
//// 输入参数 : 
//// 返回参数 : 
//// 说明     : in 16-bit reload mode   2ms
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
//// 功能描述 : 定时器2中断入口函数
//// 输入参数 : 
//// 返回参数 : 
//// 说明     : 2ms
//////////////////////////////////////////////////////////////////////////////////
void Timer2(void) interrupt 5				 //2ms
{


	
	    //递减计算器，自减1
    if (sys.digital_led_load_cnt != 0)
        sys.digital_led_load_cnt--;    
    
    //递减计算器，自减1
//    if (sys.count != 0)
//        sys.count--;

    //递减计算器，自减1
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;
	
	
	  if (uart.rIndex != 0)	 		     //串口1-485缓冲区有数据	
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
//// 功能描述 : 定时器0开启
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void Interrupt_Init (void)
{
	EA = 1; //中断允许总控制位
}