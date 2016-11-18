
#define AP_KEY_GLOBALS	
#include "extern_symbol.h" 
							 

/*******************************************************************************
	 					��ֹ��������
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
	 					����ʾ��delay
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
	 					�����������
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

	// �����㲥�趨��ַ
	if(beSettingAddr == 1)
	{ 
		//������Ŷ����µ�ַ
//		address[0] = toSettedAddr[0]; address[1] = toSettedAddr[1];	address[2] = toSettedAddr[2];
		para.str.address = para_bk.str.address;	// �����趨�ĵ�ַ
		SaveParaToBlock();

		led_flash_NO 	= LED_BLUE;				//����ǰ����һ��ƣ����պ����ڶ����
		beSettingAddr   = 2;
	}
	//ֻ����δ������񲻿գ����ҷ���ʾ������ʱ������ӦOK����
	else if((task_unfinish_num > 0)&&(digital_led_loading == OFF))
	{   //ˢ������
		//һ����û��������
		if(task_finished_num == 0)
		{
			//��ɵĿ���ˢ������
			task_finished_num = 1;
			task_finished_NO[0]   = task_unfinish_NO[0];
			task_ed_goods_numH[0] =	task_un_goods_numH[0];
			task_ed_goods_numL[0] = task_un_goods_numL[0];
			//ԭ����2��������У���δ��ɿ���������
			if(task_unfinish_num == 2)
			{
				//��δ��ɿ������=2ʱ������ǰ��                                            
				task_unfinish_NO[0]    = task_unfinish_NO[1];
				task_un_goods_numH[0]  = task_un_goods_numH[1];
				task_un_goods_numL[0]  = task_un_goods_numL[1];
			
				task_curOri_goods_numH = task_un_goods_numH[0];
				task_curOri_goods_numL = task_un_goods_numL[0];

			}
			task_unfinish_num--;
		}
		//һ������������
		else if(task_finished_num == 1)
		{
			//��ɵĿ���ˢ������
			task_finished_num = 2;
			task_finished_NO[1]    = task_unfinish_NO[0];
			task_ed_goods_numH[1]  = task_un_goods_numH[0];
			task_ed_goods_numL[1]  = task_un_goods_numL[0];
			//��ɵ���������ʷ�洢�Թ�����ʹ��
			if(task_unfinish_num == 2)
			{
				//��δ��ɿ������=2ʱ������ǰ��                                            
				task_unfinish_NO[0]     = task_unfinish_NO[1];
				task_un_goods_numH[0]   = task_un_goods_numH[1];
				task_un_goods_numL[0]   = task_un_goods_numL[1];
				
				task_curOri_goods_numH  = task_un_goods_numH[0];
				task_curOri_goods_numL  = task_un_goods_numL[0];
			}
			task_unfinish_num--;
		}
		//������ҵһȫ��������񣬵�����������
		if(task_unupdate_num == 0 && task_unfinish_num ==0)	 	
		{
//			Display_Clear();
			TaskStats    = TASK_FINISH; 	   // �������
			led_flash_NO = para.str.FinishedLED;//FinishedLED;
			led_flash_mode = LED_FLASH_MODE_ALWAYS;		  // ����ģʽ
		}
		//������ҵ���д�ʰȡ���񣬽���������ʾ
		else											
		{	//���񻺳����ջ�����������ʣ������,��ʾ-------
			if(task_unfinish_num > 0)					
			{
				digital_led_loading = ON;
				sys.digital_led_load_cnt= (para.str.WtTime+1)*100;//(WtTime+1)*100;//LOADING��ʱ��//DIGITAL_LED_REFRESH_DELAY;//LOADING��ʱ��
//				digital_led_loading_cnt = 0;
				//�ȱ��浱ǰ�������������������ʱû�е���
//				led_flash_NO_BK = led_flash_NO;
//				led_flash_NO    = LED_NONE;			
			}
			else if(task_unupdate_num > 0)				//������ҵ�������񣬵�������û�·������豸�У���ʾ------
			{
				digital_led_loading = ON;
				sys.beWaitNewData = ON;					//�ȴ��������·�
				sys.digital_led_load_cnt = DIGITAL_LED_REFRESH_DELAY_LONG;//LOADING��ʱ��
//				digital_led_loading_cnt = 0;
				//�ȱ��浱ǰ�������������������ʱû�е���
//				led_flash_NO_BK = led_flash_NO;
//				led_flash_NO    = LED_NONE;			
			}
		}
	}

	keyStatus = CMD_KEY_NULL;
}
//#endif



/************************************************************************
* ��ȡ����ֵ
* ���ؼ�ֵ
************************************************************************/
INT8U	Scan_Val(void)
{
	INT8U	keyval = CMD_KEY_NULL;
	//-------------------------------------------
	//�����1           ��ʱ1us   
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
** ���������� Key scan and Key Function module
** ʱ�䣺2009-9-15 16:46
************************************************************************/
void Key_Scan(void)
{		
	INT8U	keyval;

	/*------------------------------------------------------------------*/
	// ϵͳ��������յ����ݣ��尴�����±�־����ɨ�谴��
	if(SystemLocked == ON)  { Key.b.Short = FALSE; return; }
	/*------------------------------------------------------------------*/
	//ɨ���־ʹ��ȷ��,��ʱ��32msʹ��һ��
	if(f_KeyEnable == FALSE){return;}
	f_KeyEnable = FALSE;		//ɨ������ʹ�ܱ�־
	/*------------------------------------------------------------------*/	
	keyval = Scan_Val();				
	/*------------------------------------------------------------------*/

    #if ( MACHINETYPE == P3N3_IB_800U )                         //������������ 20140413
        if( keyval != Key.Backup ) { Key.Backup = keyval; }		//�����ֵ  //ȡ��ע��20140413
    #endif

//	else 
	if( CMD_KEY_NULL == keyval )
	{ 	//�ް�������
		if( TRUE == Key.b.Short )
		{ 	// �ո��ɿ�
		   if(beSettingAddr != 0)
		   {  }		//����״̬		
//		   else if(TASK_NONE == TaskStats)
//		   	   { led_flash_NO = LED_NONE; }								// �ǹ���״̬���
//		   else	if(TASK_FINISH == TaskStats)
//		   	   { TaskStats = TASK_NONE; led_flash_NO = FinishedLED; }	// ����ȫ�������F1���õĵ�
//		   else
//		   {
//
//		   }		   	 
		}					
		Key.b.Short    = FALSE;			//�̰��ͷ�
		Key.b.LongOnce = FALSE;			//����һ���ͷ�	
		Key.b.Long 	   = FALSE;			//�����ͷ�					
	}
	else if( CMD_KEY_OK == keyval )
	{	//��һ�ΰ���	

		if( (FALSE == Key.b.Short) )
		{ Key.b.Short = TRUE;	keyStatus = keyval; Process_Keyboard(); }	  // ִ�а�������
        
		Key.b.Short = TRUE;					//�̰���Ч
		Key.OnCnt	= 0x00;					//����ʱ���ʼ��
	}
   Key.Backup = keyval; 
}
/*******************************************************************************
								END					
*******************************************************************************/

