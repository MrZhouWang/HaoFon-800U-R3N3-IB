/*=========================================================
// 程序更改说明
//  2015年2月12日更改  版本v1.0
//	 1.在800U-T3N3_V1.2的基础上修改，将触控按键改为普通按键
     2.其它功能不变
//	 MCU:STC15W408AS   8K Flash  512RAM  EEPROM 5K 	  UART 分组可当三组用 SOP16封装，14个IO口，
         内带晶振和上电复位电路

// 功能说明
F1参数 保存的是任务完成后灯亮的颜色，共有0~7，8种颜色，默认无
F2参数 进度条的基准时间
F3参数 按键按下的灯颜色，共有0~8，9种，0~7对应各自的颜色，8为不变

工作状态下
按下按键，灯根据F3的设置，亮对应的灯
松开时，任务全部完成亮F1灯颜色，未完成，先灭，再延时进度时间，再亮通讯下发的灯色
非工作状态下
按下按键，灯根据F3的设置，亮对应的灯
松开时，灭
设置状态下
正在设置地址亮绿灯
设置完成亮蓝灯
清除命令，则灭灯

// 2014年11月10日更改
1. F3 变量名更改为KeyPressLed，=8时则亮灯颜色为上个状态+1，超过7则变1
2. 开机只亮红绿蓝
3. 工作完成后，灯状态不清除，除非来清除命令
4. FT PTL 软件任务完成时会发清除命令，勾F1功能则不发清除命令
5. 按键灯亮颜色优先于清除命令的灯颜色

// 2014年12月19日更改 V1.1
1. 参数存储和命名和P6一致
2. 通讯重发判断同P6一致
3. 触摸按键灵敏度还是32比较合适
4. 按键按下的值默认为蓝色
5. 锁定则不扫描按键，原来扫描按键但不执行

// 2014年12月24日更改 V1.2
1. 广播可设定灵敏度，0xA0,灵敏度为32~48，不在此范围内则恢复到默认值32

//2015年4月7日更改为800U-P3N3-T3N3 IB_V2.0_20150407
1.在800U-T3N3_V1.2基础上，增加物理按键功能，并通过条件编译选择触控功能或者按压功能
2.触控最灵敏级别由原来的32改为10
3.删除部分注释掉的功能重复的函数的代码

=========================================================*/

#define AP_CPU_GLOBALS	
#include "extern_symbol.h" 

/*******************************************************************************
	 					系统参数复位
*******************************************************************************/
void sys_data_reset(void)
{
    UINT8 i;

//	ucFocus        	= 0;
	LastSoftGetOkCount = 0;
    task_unfinish_num =0;     
    task_finished_num =0;

    for(i=0; i< MAX_TASK_NUM; i++)
    {
        task_unfinish_NO[i] =0;    
        task_un_goods_numH[i] =0;
        task_un_goods_numL[i] =0; 
        
        ///已完成任务块

        task_finished_NO[i] =0;    
        task_ed_goods_numH[i] =0;
        task_ed_goods_numL[i] =0; 
    }
	sys.digital_led_load_cnt = 0;
	digital_led_loading = OFF;	     
	sys.beWaitNewData = OFF;
	f_repeat	      = FALSE;		 // 清重复接收到任务标志
	beSettingAddr = 0;
}

/*******************************************************************************
	 					1毫秒延迟函数
*******************************************************************************/
void Delay1Ms() 
{
    UINT16 i = 852;
    while (i != 0) {i--;}
}

/*******************************************************************************
	 					毫秒延迟函数
    input   usDelay     延迟时间值
	output  none
*******************************************************************************/

void DelayMs(UINT16 usDelay)
{
    for(; usDelay>0; usDelay--)
    {
        Delay1Ms();
    }   
}

/*******************************************************************************
	 					  端口初始化
*******************************************************************************/
void IO_Init(void)
{
	//开漏输出	PxM1和PxM0对应配成11
	P3M1 = 0x0C;				   // SDA和SCL口配成开漏，3.3V
	P3M0 = 0x0C;	

	P1M1 = 0x1C;				   // BLUE和RED和GREEN口配成开漏，5V
	P1M0 = 0x1C;
	//推挽输出 PxM1和PxM0对应配成01
	P1M1 &= (~0x03);			  // 485CTL配成推挽，5V
	P1M0 |= 0x03;		


	//IO状态初始化
	led_light_ctrl(LED_NONE);

//	sys.flag = OFF;
//	sys.count= 2;

//	address[0] = 0xff;
//	address[1] = 0xff;
//	address[2] = 0xff;
//	baudrate   = BD_38400;
}

/***************************************************
		T0定时器初始化
***************************************************/
#define TH0_INIT        0xf8
#define TL0_INIT        0xcc

void InitT0(void)
{
    //定时1ms//默认为12T模式=>T0_CLK=SYS_CLK/12=11 059 200 /12 =921600
    TMOD = (TMOD & 0xf0) | 0x01;    //mode 1: 16 bit counter
    TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

    ET0  = 1;                       //allow interrupt
    TR0  = 1;                       //open timer0
}

/*******************************************************************************
	定时器T0中断服务程序(ms中断)
	Input :		中断向量
	Output :	sys.flag 置为有效	2ms
*******************************************************************************/

void timer0_int (void) interrupt 1
{ 
//    p_Test = ~p_Test;
	  TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

//    sys.flag  = ON;
    //串口缓冲区有数据
    if (uart.rIndex != 0)	 			
    {
        if (uart.rTimer-- == 0) 
        {
			uart.rFlag  = ON;    
        }
    }

	// 按键扫描计数时间
	if(Key.SysCnt++ >= TIMER_COUNT_32ms )
	{ f_KeyEnable = TRUE; Key.SysCnt = 0; }

    //递减计算器，自减1
    if (sys.digital_led_load_cnt != 0)
        sys.digital_led_load_cnt--;    
    
    //递减计算器，自减1
//    if (sys.count != 0)
//        sys.count--;

    //递减计算器，自减1
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;

 }

/*******************************************************************************
	 					开机自检测程序
    input:   none     
	output:  none
*******************************************************************************/

void Start_Init(void)
{
	sys_data_reset();

	led_light_ctrl(LED_RED);
 	DelayMs(500);
	led_light_ctrl(LED_GREEN);
 	DelayMs(500);
	led_light_ctrl(LED_BLUE);
 	DelayMs(500);
    
//	led_light_ctrl(LED_RG);
// 	DelayMs(500);
//	led_light_ctrl(LED_RB);
// 	DelayMs(500);
//	led_light_ctrl(LED_GB);
// 	DelayMs(500);
	led_light_ctrl(LED_NONE);
	CurDispMode = DISPMODE_NONE;
}

/*******************************************************************************
	 						main program				
*******************************************************************************/   
int main(void) 
{   
	EA = 0;
	IO_Init();

	DelayMs(300);    		// 等待电源稳定
	
	InitT0();
	
	UART_Init();		  	//

	Start_Init();			// 灯亮状态
	
 	LoadParaInEeprom();	    // 读取灵敏度值
    
    #if	( MACHINETYPE == T3N3_IB_800U)
       CBM7008_init();			// 必须开机后200ms再设置灵敏度	
    #endif
    
	EA = 1;					//使能中断

	Display_Clear();

// 	uart.wBuffer[0] = 0x55;
// 	SendSerialByte(&para.Byte[0],MAX_CFGPARA);
// 	SendSerialByte((UINT8 *)uart.wBuffer,1);
// 	SendSerialByte(&lmd[0], 8);
// 	uart.wBuffer[0] = 0xAA;
// 	SendSerialByte((UINT8 *)uart.wBuffer,1);

	p_m485_ctrl = 0;							// 关闭发送DE端口，打开接收/RE485端口
	led_flash_period = 300;						//闪烁周期控制
	led_flash_mode 	 = LED_FLASH_MODE_ALWAYS ;	//长亮模式
	TaskStats 		 = TASK_NONE;		// 非工作状态
	ClearFlg	   = OFF;
	Key.b.Short    = FALSE;				// 短按释放
	Key.b.LongOnce = FALSE;				// 长按一次释放	
	Key.b.Long 	   = FALSE;				// 长按释放
	SystemLocked   = OFF;				// 系统解锁
	CurDispMode    = DISPMODE_NONE;  	// 当前任务状态设为非运行状态
    
	while(1)
	{       
		 CMD_led_flash(); 
		 Key_Scan();          		

		 if (uart.rFlag == ON)
		 {        	
			 Cmd_Process();
			 uart.rFlag   = OFF;
			 uart.rIndex  = 0;
		 }

		 if((digital_led_loading == ON))
		 {
			 if(sys.digital_led_load_cnt == 0)
			 {
				 digital_led_loading = OFF;
				 sys.beWaitNewData = OFF;                 
			 }
		 }

	}		
}

/*******************************************************************************
								END					
*******************************************************************************/
