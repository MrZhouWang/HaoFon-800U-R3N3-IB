#define KEY_GLOBAL
#include "include.h"
UINT8 idata keyStatus = OFF;		//����״̬

//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ȡ����ֵ
//// ������� : 
//// ���ز��� : ���ؼ�ֵ
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
INT8U Scan_Val()
{		
		INT8U	keyval = CMD_KEY_NULL;	 
	  if((f_prossout == TRUE)&&(AD_Base_OK == OK))                      //���������� �޶����ڻ�׼������������Ƚ�
	  {	
			 f_prossout = FALSE;
			 
			 
			 if((sampDat>RevBase)&&(sampDat-RevBase)>Dval)   //�ж��Ƿ�Ϊ���ش���
				 out++;
			 else                 //����ڵ�ʱ����Ч ���¼�ʱ
				 out=0;	
	  }		
		if(out>Dnum)            //�а���
		{
			 out=Dnum+1;
			 keyval = CMD_KEY_OK;      
		}
		else
			 keyval = CMD_KEY_NULL;
		
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
	//f_KeyEnable = FALSE;		//ɨ������ʹ�ܱ�־
	/*------------------------------------------------------------------*/	
	keyval = Scan_Val();				
	/*------------------------------------------------------------------*/



	if( CMD_KEY_NULL == keyval )
	{ 	                            //�ް�������
		if( TRUE == Key.b.Short )
		{ 	                          // �ո��ɿ�
		   if(beSettingAddr != 0)
		   {  }		                    //����״̬				   	 
		}					
		Key.b.Short    = FALSE;			  //�̰��ͷ�
		Key.b.LongOnce = FALSE;			  //����һ���ͷ�	
		Key.b.Long 	   = FALSE;			  //�����ͷ�					
	}
	else if( CMD_KEY_OK == keyval ) //��һ�ΰ���
	{		

		if( (FALSE == Key.b.Short) )
		{ Key.b.Short = TRUE;	keyStatus = keyval; Process_Keyboard(); }	  // ִ�а�������
        
		Key.b.Short = TRUE;					  //�̰���Ч
		Key.OnCnt	= 0x00;					    //����ʱ���ʼ��
	}
  Key.Backup = keyval; 
}
/*******************************************************************************
	 					�����������
	input:   none     
	output:  none					
*******************************************************************************/
void Process_Keyboard(void)
{    
	UINT16 updata_num = 0;

	
	if(beSettingAddr == 1)                      // �����㲥�趨��ַ
	{ 
		para.str.address = para_bk.str.address;	  // �����趨�ĵ�ַ
		SaveParaToBlock();

		led_flash_NO 	= LED_BLUE;				          //����ǰ����һ��ƣ����պ����ڶ����
		beSettingAddr   = 2;
	}
	
	else if((task_unfinish_num > 0)&&(digital_led_loading == OFF)) //ֻ����δ������񲻿գ����ҷ���ʾ������ʱ������ӦOK����
	{   		
		if(task_finished_num == 0)                         //һ����û��������
		{
			
			task_finished_num = 1;
			task_finished_NO[0]   = task_unfinish_NO[0];     //��ɵĿ���ˢ������
			task_ed_goods_numH[0] =	task_un_goods_numH[0];
			task_ed_goods_numL[0] = task_un_goods_numL[0];
			
			if(task_unfinish_num == 2)                       //ԭ����2��������У���δ��ɿ���������
			{
				                                          
				task_unfinish_NO[0]    = task_unfinish_NO[1];  //��δ��ɿ������=2ʱ������ǰ��  
				task_un_goods_numH[0]  = task_un_goods_numH[1];
				task_un_goods_numL[0]  = task_un_goods_numL[1];
			
				task_curOri_goods_numH = task_un_goods_numH[0];
				task_curOri_goods_numL = task_un_goods_numL[0];

			}
			task_unfinish_num--;
		}
		
		else if(task_finished_num == 1)                    //һ������������
		{
			
			task_finished_num = 2;                           //��ɵĿ���ˢ������
			task_finished_NO[1]    = task_unfinish_NO[0];
			task_ed_goods_numH[1]  = task_un_goods_numH[0];
			task_ed_goods_numL[1]  = task_un_goods_numL[0];
			
			if(task_unfinish_num == 2)                       //��ɵ���������ʷ�洢�Թ�����ʹ��
			{
				                                           
				task_unfinish_NO[0]     = task_unfinish_NO[1]; //��δ��ɿ������=2ʱ������ǰ�� 
				task_un_goods_numH[0]   = task_un_goods_numH[1];
				task_un_goods_numL[0]   = task_un_goods_numL[1];
				
				task_curOri_goods_numH  = task_un_goods_numH[0];
				task_curOri_goods_numL  = task_un_goods_numL[0];
			}
			task_unfinish_num--;
		}
		
		if(task_unupdate_num == 0 && task_unfinish_num ==0)	//������ҵһȫ��������񣬵�����������	
		{
			TaskStats    = TASK_FINISH; 	                    // �������
			led_flash_NO = para.str.FinishedLED;              //FinishedLED;
			led_flash_mode = LED_FLASH_MODE_ALWAYS;		        // ����ģʽ
		}		  
		else											                          //������ҵ���д�ʰȡ���񣬽���������ʾ
		{	
			if(task_unfinish_num > 0)					                //���񻺳����ջ�����������ʣ������,��ʾ-------
			{
				digital_led_loading = ON;
				sys.digital_led_load_cnt= (para.str.WtTime+1)*100;//(WtTime+1)*100;//LOADING��ʱ��//DIGITAL_LED_REFRESH_DELAY;//LOADING��ʱ��
			}
			else if(task_unupdate_num > 0)				            //������ҵ�������񣬵�������û�·������豸�У���ʾ------
			{
				digital_led_loading = ON;
				sys.beWaitNewData = ON;					                //�ȴ��������·�
				sys.digital_led_load_cnt = DIGITAL_LED_REFRESH_DELAY_LONG;//LOADING��ʱ��	
			}
		}
	}

	keyStatus = CMD_KEY_NULL;
}

