
#define AP_KEY_GLOBALS	
#include "extern_symbol.h" 
							 

/*******************************************************************************
	 					防止按键拉动
	input:   none     
	output:  none					
*******************************************************************************/
#define CNT_DELAY   5
#if 0
void Delay(void)
{
    UINT8 i;
    
    for (i=0; i<CNT_DELAY; i++)
    {
        Delay1Ms();
    }
}
#endif

/*******************************************************************************
	 					带显示的delay
	input:   none     
	output:  none					
*******************************************************************************/
#if 0
void Delay_Display(UINT16 count)
{
    UINT8 i;
    
    for (i=0; i<count; i++)
    {
        Delay1Ms();
    }
}
#endif

/*******************************************************************************
	 					按键处理程序
	input:   none     
	output:  none					
*******************************************************************************/
//#if 0
void Process_Keyboard(void)
{    

	UINT16 updata_num = 0;

//	if(SystemLocked == ON)
//	{  
//		keyStatus = CMD_KEY_NULL;
//		return;
//	}

	// 主机广播设定地址
	if(beSettingAddr == 1)
	{ 
		//任意键排都记下地址
//		address[0] = toSettedAddr[0]; address[1] = toSettedAddr[1];	address[2] = toSettedAddr[2];
		para.str.address = para_bk.str.address;	// 保存设定的地址
		SaveParaToBlock();

		led_flash_NO 	= LED_BLUE;				//接收前亮第一组灯，接收后亮第二组灯
		beSettingAddr   = 2;
	}
	//只有在未完成任务不空，并且非显示进度条时，才响应OK键。
	else if((task_unfinish_num > 0)&&(digital_led_loading == OFF))
	{   //刷新数据
		//一个都没完成情况下
		if(task_finished_num == 0)
		{
			//完成的块中刷入数据
			task_finished_num = 1;
			task_finished_NO[0]   = task_unfinish_NO[0];
			task_ed_goods_numH[0] =	task_un_goods_numH[0];
			task_ed_goods_numL[0] = task_un_goods_numL[0];
			//原先有2个任务队列，则未完成块中作处理
			if(task_unfinish_num == 2)
			{
				//当未完成块的数据=2时，数据前移                                            
				task_unfinish_NO[0]    = task_unfinish_NO[1];
				task_un_goods_numH[0]  = task_un_goods_numH[1];
				task_un_goods_numL[0]  = task_un_goods_numL[1];
			
				task_curOri_goods_numH = task_un_goods_numH[0];
				task_curOri_goods_numL = task_un_goods_numL[0];

			}
			task_unfinish_num--;
		}
		//一个已完成情况下
		else if(task_finished_num == 1)
		{
			//完成的块中刷入数据
			task_finished_num = 2;
			task_finished_NO[1]    = task_unfinish_NO[0];
			task_ed_goods_numH[1]  = task_un_goods_numH[0];
			task_ed_goods_numL[1]  = task_un_goods_numL[0];
			//完成的数据做历史存储以供回显使用
			if(task_unfinish_num == 2)
			{
				//当未完成块的数据=2时，数据前移                                            
				task_unfinish_NO[0]     = task_unfinish_NO[1];
				task_un_goods_numH[0]   = task_un_goods_numH[1];
				task_un_goods_numL[0]   = task_un_goods_numL[1];
				
				task_curOri_goods_numH  = task_un_goods_numH[0];
				task_curOri_goods_numL  = task_un_goods_numL[0];
			}
			task_unfinish_num--;
		}
		//本次作业一全部完成任务，灯灭，数据清零
		if(task_unupdate_num == 0 && task_unfinish_num ==0)	 	
		{
//			Display_Clear();
			TaskStats    = TASK_FINISH; 	   // 任务完成
			led_flash_NO = para.str.FinishedLED;//FinishedLED;
			led_flash_mode = LED_FLASH_MODE_ALWAYS;		  // 常亮模式
		}
		//本次作业还有待拾取任务，进行拉条显示
		else											
		{	//任务缓冲区空或者主机还有剩余任务,显示-------
			if(task_unfinish_num > 0)					
			{
				digital_led_loading = ON;
				sys.digital_led_load_cnt= (para.str.WtTime+1)*100;//(WtTime+1)*100;//LOADING短时间//DIGITAL_LED_REFRESH_DELAY;//LOADING短时间
//				digital_led_loading_cnt = 0;
				//先保存当前哪组灯亮，在设置拉条时没有灯亮
//				led_flash_NO_BK = led_flash_NO;
//				led_flash_NO    = LED_NONE;			
			}
			else if(task_unupdate_num > 0)				//本次作业还有任务，但是任务还没下发到本设备中，显示------
			{
				digital_led_loading = ON;
				sys.beWaitNewData = ON;					//等待新数据下发
				sys.digital_led_load_cnt = DIGITAL_LED_REFRESH_DELAY_LONG;//LOADING长时间
//				digital_led_loading_cnt = 0;
				//先保存当前哪组灯亮，在设置拉条时没有灯亮
//				led_flash_NO_BK = led_flash_NO;
//				led_flash_NO    = LED_NONE;			
			}
		}
	}

	keyStatus = CMD_KEY_NULL;
}
//#endif



/************************************************************************
* 获取按键值
* 返回键值
************************************************************************/
INT8U	Scan_Val(void)
{
	INT8U	keyval = CMD_KEY_NULL;
	//-------------------------------------------
	//检测列1           延时1us   
    #if	( MACHINETYPE == T3N3_IB_800U)
        keyval = Get_CBM7008_KeySts();
	if((keyval & 0x02) == 0x02)
	{ keyval = CMD_KEY_OK; }
        
    #else
	    KEY_OK = 1;
		_nop_();_nop_();_nop_();_nop_();
        keyval = KEY_OK; 
        if(keyval == 1)
            { keyval = CMD_KEY_NULL; }
        else{ keyval = CMD_KEY_OK; }

     #endif  

	//-------------------------------------------		
	return( keyval );
}
/************************************************************************
** 功能描述： Key scan and Key Function module
** 时间：2009-9-15 16:46
************************************************************************/
void Key_Scan(void)
{		
	INT8U	keyval;

	/*------------------------------------------------------------------*/
	// 系统锁定或接收到数据，清按键按下标志，不扫描按键
	if(SystemLocked == ON)  { Key.b.Short = FALSE; return; }
	/*------------------------------------------------------------------*/
	//扫描标志使能确认,定时器32ms使能一次
	if(f_KeyEnable == FALSE){return;}
	f_KeyEnable = FALSE;		//扫描则清使能标志
	/*------------------------------------------------------------------*/	
	keyval = Scan_Val();				
	/*------------------------------------------------------------------*/

    #if ( MACHINETYPE == P3N3_IB_800U )                         //增加条件编译 20140413
        if( keyval != Key.Backup ) { Key.Backup = keyval; }		//保存键值  //取消注释20140413
    #endif

//	else 
	if( CMD_KEY_NULL == keyval )
	{ 	//无按键按下
		if( TRUE == Key.b.Short )
		{ 	// 刚刚松开
		   if(beSettingAddr != 0)
		   {  }		//亮灯状态		
//		   else if(TASK_NONE == TaskStats)
//		   	   { led_flash_NO = LED_NONE; }								// 非工作状态灭灯
//		   else	if(TASK_FINISH == TaskStats)
//		   	   { TaskStats = TASK_NONE; led_flash_NO = FinishedLED; }	// 任务全部完成亮F1设置的灯
//		   else
//		   {
//
//		   }		   	 
		}					
		Key.b.Short    = FALSE;			//短按释放
		Key.b.LongOnce = FALSE;			//长按一次释放	
		Key.b.Long 	   = FALSE;			//长按释放					
	}
	else if( CMD_KEY_OK == keyval )
	{	//第一次按下	

		if( (FALSE == Key.b.Short) )
		{ Key.b.Short = TRUE;	keyStatus = keyval; Process_Keyboard(); }	  // 执行按键功能
        
		Key.b.Short = TRUE;					//短按有效
		Key.OnCnt	= 0x00;					//按下时间初始化
	}
   Key.Backup = keyval; 
}
/*******************************************************************************
								END					
*******************************************************************************/

