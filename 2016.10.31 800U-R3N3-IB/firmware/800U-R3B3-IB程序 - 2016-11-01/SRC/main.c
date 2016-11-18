/********************************************************************************
工作状态下 

      使能     按键	    灯        输出
			
			 0		   按下		  黄    	   有
			 0		   松开		  灭	       无
			 1		   松开		  红	       无
			 1		   按下		  绿         有	   

使能：1 使能输入   0 无使能输入


基准采样在定时器初上电几个周期内完成，与平时采样环境保持一致

2016.09.18  感应离开延时100ms灯色切换
2016.10.27  开关状态对比值采用绝对值
2016.10.27  基准采样在上电的时候取100次平均值 标准基准值在1.35V左右 如果基准值不在这个范围内则红灯常亮在报警状态
2016.10.28  集准采样发生异常设备会进行相应的异常指示
********************************************************************************/

#define MAIN_GLOBAL
#include "include.h"			

bit f_prossout= FALSE;
bit valid_ok = FALSE;
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 系统频率设置
//// 输入参数 : 
//// 返回参数 : 
//// 说明     : 
//////////////////////////////////////////////////////////////////////////////////
void OSCILLATOR_Init (void)
{   	
	INT8U	i;
	OSCICN   = 0x06;			          //频率为24.5Mhz	/2 =12.25
	for(i = 500; i>0; i--);
	while((OSCICN & 0x10) == 0) ;		// 判断是否正常振荡
}
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 主函数
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
//////////////////////////////////////////////////////////////////////////////////
void main(void)
{	
	
	PCA0MD &= ~0x40; 		         //看门狗定时器被禁止
	RSTSRC |= 0x02;					     //使能VDD电压监视
	
	EA = 0;                      //两个操作码来禁止中断
	EA = 0;
	
	PORT_Init ();					  		 // Initialize crossbar and GPIO
	delay_ms(100);               
		
  OSCILLATOR_Init();			     // Initialize the system clock		
	Timer0_Init();						   // Timer0 自动重装模式2
	Timer2_Init();               // Timer2 Initialization
	UART485_Init();              // 串口初始化
	ADC_Init ();					   		 // 设置ADC



	Base_ADC();                  //基准采样
	LoadParaFromFlash();	       // 读取灵敏度值
	led_blink();
  sys_data_reset();            //系统参数复位
  
 	
	

	
	
	Display_Clear();
	CurDispMode = DISPMODE_NONE;	
	M485_CTRL_OFF;						                   // 关闭发送DE端口，打开接收/RE485端口
	led_flash_period = 1000;						         // 闪烁周期控制
	led_flash_mode 	 = LED_FLASH_MODE_ALWAYS ;	 // 长亮模式
	TaskStats 		 = TASK_NONE;		               // 非工作状态
	ClearFlg	   = OFF;
	Key.b.Short    = FALSE;				               // 短按释放
	Key.b.LongOnce = FALSE;				               // 长按一次释放	
	Key.b.Long 	   = FALSE;				               // 长按释放
	f_KeyEnable    = TRUE;                       // 使能按键
	SystemLocked   = OFF;				                 // 系统解锁
	CurDispMode    = DISPMODE_NONE;  	           // 当前任务状态设为非运行状态
	LED_MultiCast_Mode =OFF;                     // 组播模式关
	para.str.FinishedLED=LED_NONE;               // F1初始化为灭
	
	
	
	Interrupt_Init ();			     // 开中断
	
  
	while(1)
	{
		PCA0CPH2 =   1;                              //喂狗
		// 1个信号灯与3 组led指示灯闪烁控制
		if(AD_Base_OK != OK)                       //采集基准电压失败 指示 
		{
			 if(ADC_BaseErrNum == 0)
	        ADC_BASE_ERR_BLINK();		
		}
		Key_Scan();                                //按键扫描处理程序
		CMD_led_flash();                           //灯闪烁周期以及灯色控制
       
		
                		
		                                           //非配置状态
		if (uart.rFlag == ON)
		{  
     // Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 	
			Cmd_Process();                           //处理主机下发命令
			uart.rFlag   = OFF;
			uart.rIndex  = 0;
		}
		                                           //显示LOADING 过程 '-' '--' '---' '----' '-----' '------'
		if((digital_led_loading == ON))
		{
			if(sys.digital_led_load_cnt == 0)
			{
				digital_led_loading = OFF;
				sys.beWaitNewData = OFF;                 
			}
		}
		PCA0CPH2 = 1;                              //喂狗			
	} 
}


