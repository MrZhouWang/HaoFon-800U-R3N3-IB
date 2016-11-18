#define PROCESS_GLOBAL
#include "include.h"
/*******************************************************************************
	 					����MODBUS��վ�·�������
ÿ�����14���ֽڣ�
BYTE_ADD,BYTE_HEAD,BYTE1,---> BYTE10 , CRC_H , CRC_L

BYTE_ADD 	: �ն˵�ַ
BYTE_HEAD	:  �´�������
BYTE1		:  �´�ָ���
BYTE2		:  LEDָʾ����ʾ��ʽ
BYTE3		:  �����·���"���ӱ�ǩ"����
BYTE4		:  �����еĴ˵�ַʣ��"���ӱ�ǩ"����
BYTE5-BYTE7	:   ��һ��"���ӱ�ǩ"����
BYTE8-BYTE10:   �ڶ���"���ӱ�ǩ"����	
BYTE11		: CRC_H
BYTE12		: CRC_L

�´������:
00-	  ����豸��������ʾ��Ϣ����ǩ��������;
01-	  ���ӱ�ǩ����Ʒ������а�ģʽ����ʱ3~10λ������Ч�������쳤��2�ֽ�
02-    ָʾ����ʾ��ʽ�������ƣ���ʱ3~10λ������Ч�������쳤��2�ֽ�
03-	  �·���ʾ�����Լ�ָʾ�Ƶ���Ϣ����ʰȡ���ϻ����̵���ҵ��
         �����쳤��7�ֽڻ���10�ֽ�
*******************************************************************************/
void Cmd_Process(void)
{
	//���ݰ����Ȳ���,����ModbusӦ��
	
	if (uart.rIndex < ONE_CMD_MIN_LENGTH)
		return;   		
    //�ȿ���ַ��CRCУ�顢���ݳ��ȵ��ж�
	  //�ٸ��ݵڶ����ֽڵķ�Χ�ж��ǲ�����Э��������Ǿ�Э�����
	  //��ַ��CRC����У��,�´�ָ��洢,MODBUSӦ��,CRC_Check�����а�����ַ�趨����
	  DiscodeProtocalV1();
    //	SendSerialByte((UINT8 *)uart.rBuffer, uart.rIndex);
}
/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void DiscodeProtocalV1(void)
{
	UINT8 addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];
	UINT16 usCRC, retCRC;
	UINT8 i = 0;
	//0�Ź㲥�����ַ������ظ�
	if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
	{
//		p_led_rxd = 0;
			
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
		//retCRC = usCRC;
		if (retCRC == usCRC) 						//crcУ��ɹ� 
		{
			if(uart.rBuffer[3] == 0X00)					//��ַ����
			{
				ResetAddr();
			}
			else if(uart.rBuffer[3] == 0X01)				//���յ�ַ������ʾ������ȴ�����ȷ��
			{
				if(beSettingAddr != 0X02)
				{				
					beSettingAddr = 1;
					para_bk.str.address = uart.rBuffer[5];	// ���趨��ַ���뱸����
					//���ø�����Ϣ����ʾ��ַ�趨��ʾ
					led_flash_mode = LED_FLASH_MODE_ALWAYS;	//����
					led_flash_NO = LED_GREEN;				//����ǰ����һ��ƣ����պ����ڶ����
					ClearFlg	 = OFF;
				}
			}
			//0X10��	�㲥��ʾ
			//00 06 ?? 10 ## ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X10)				//�㲥��ʾ����,���## ## == 0X00 0X00������ʾ��ַ
			{
				INT16U	usValue;
				usValue = ((UINT16)(uart.rBuffer[4])) *256 + (UINT16)(uart.rBuffer[5]);
				
			    //��ʾ��ַ
				if(usValue == 0X00)                 
				{
//					led_flash_NO_BK = LED_GREEN;					
				}
				else if(usValue == F1)
				{
					led_flash_NO = para.str.FinishedLED;  //FinishedLED;
					ClearFlg	 = OFF;
				}
				else if(usValue == F2)
				{
//					led_flash_NO_BK = WtTime;
				}
				else if(usValue == F3)
				{
//					led_flash_NO = KeyPressLed;
//					ClearFlg	 = OFF;
				}
				else if(usValue == F4)
				{
//					led_flash_NO_BK = LED_RB;
				}
				else if(usValue == F5)
				{
//					led_flash_NO_BK = LED_RGB;
					led_flash_NO = para.str.KeyPressLed;//KeyPressLed;
					ClearFlg	 = OFF;				
				}

				led_flash_mode = LED_FLASH_MODE_ALWAYS;
			}
			//00 06 ?? 11 ?? ?? CRCH CRCL
			else if(uart.rBuffer[3] == 0X11)				//�㲥���
			{
				Display_Clear();
				sys_data_reset();
			}
			//0X20��	�㲥�洢��
			//00 06 ?? 20 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X20)				//�㲥���ò���1(��ǩ��Ӧ���������ɺ�����ģʽ)
			{
				led_flash_mode = LED_FLASH_MODE_ALWAYS;
				para.str.FinishedLED = uart.rBuffer[5];	
				led_flash_NO = para.str.FinishedLED;			
				
				ClearFlg	= OFF;
				SaveParaToBlock();
			}
			//00 06 ?? 21 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X21)				 //�㲥���ò���2(һ�Զ�����ʱ���������ȴ�ʱ��)
			{
				para.str.WtTime = uart.rBuffer[5];		 // ���°�������ɫ
				
 				led_flash_NO   = LED_GREEN ;	   			 // ����ģʽ
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	 // ����ģʽ
				led_flash_status = 0;						       // ����	 
				led_flash_period = 100;						     // ��˸ʱ���׼
				SaveParaToBlock();							       // �Բ������ж�
			}
			//00 06 ?? 22 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X22)				 //�㲥���ò���3(���ܼ��趨)
			{
				para.str.FnDefine = uart.rBuffer[5];	 // д�����

				led_flash_NO   = LED_GREEN ;	   			 // ����ģʽ
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	 // ����ģʽ

				led_flash_status = 0;						       // ����	 
				led_flash_period = 100;						     // ��˸ʱ���׼
				SaveParaToBlock();							       // �Բ������ж�
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X23)				 //�㲥���ò���4(��ʾģʽ�趨)
			{				

				para.str.DispMode = uart.rBuffer[5];

				led_flash_NO   = LED_GREEN ;	   			  // ����ģʽ
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	 	// ����ģʽ
				led_flash_status = 0;						        // ����	 
				led_flash_period = 100;						      // ��˸ʱ���׼
				SaveParaToBlock();						        	// �Բ������ж�
			}
			//00 06 ?? 24 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X24)				  //�㲥���ò���5(������ɫ�趨)
			{
				para.str.KeyPressLed = uart.rBuffer[5];	// д�����
				led_flash_NO = para.str.KeyPressLed ;	  // ����ģʽ
				led_flash_mode = LED_FLASH_MODE_ALWAYS;	// ����ģʽ
				ClearFlg	= OFF;
				SaveParaToBlock();						          // �Բ������ж�				
			}
			//00 06 ?? 0xA0 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0XA0)				  //�㲥���ò���4(��ʾģʽ�趨)///////////                
			{

         para.str.CBM7008LMD = uart.rBuffer[5];
               
				led_flash_NO   = LED_GREEN ;	   			   // ����ģʽ
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	   // ����ģʽ
				led_flash_status = 0;						         // ����	 
				led_flash_period = 100;						       // ��˸ʱ���׼
				SaveParaToBlock();							         // �Բ������ж�	
 
			}
		}

	}
	//�������ǹ㲥���������Ϣ����Ҫ�ظ�
	else if (uart.rBuffer[0] == addr)				//��ַУ��
	{
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
    //retCRC = usCRC;
		if (retCRC == usCRC) 						//crcУ��ɹ� 
		{
			switch (uart.rBuffer[1])
			{
			case READ_MULTI_BYTE://�ն��ϴ�����,0X03��modbus���ͨ��ʱѯ���Ƿ��Ѿ�����ɼ������õ�ַʱѯ���Ƿ��Ѿ�����ɼ����յ�ַ
				{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//�Ĵ�������01��ʾ���Ե�ַ�Ƿ��Ѿ��趨������
					{
						ACK_Multi_ReadTestAddr();										//��ַ�趨�Ƿ�ɹ�����
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//�Ĵ�������03��ʾ��ȡ�Ƿ�OK��������
					{        				        
						ACK_Multi_Read();												//Ӧ��Modbus 0x03�Ųɼ���ť����
					}
					break;
				}
			case WRITE_SINGLE_BYTE:	//�������´�����,0X06��modbus�������������ǩ
				{
					if(uart.rIndex == 0X08)						//����06�������8�ֽڳ���
					{
						ACK_Single_Write();
						//���ȫ��(�����ʾ��Ϣ���ơ�������)�����OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						if(uart.rBuffer[3] == 0X00)
						{
							Display_Clear();
							sys_data_reset();
							CurDispMode = DISPMODE_NONE;
						}
						//�����ʾ����(�����ʾ��Ϣ���ơ�������)�������������OK��Ϣ+ɨ��ǹ��Ϣ+������Ϣ
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();
							sys_data_reset();
							CurDispMode = DISPMODE_NONE;
						}
						else if(uart.rBuffer[3] == 0x20)		//��������
						{
							if(uart.rBuffer[5] == 0x00)			//����
							{
								SystemLocked = OFF;
							}
							else if(uart.rBuffer[5] == 0x01)	//����
							{
								SystemLocked = ON;
							}
						}
						else if(uart.rBuffer[3] == 0x30)		//��λ���յ��ɼ���ť�����������n��OK����󣬻ش����豸����֪�豸�Ѿ����յ���n��OK��Ϣ(ֻ������λ���յ�1��OK��Ϣ�������豸�л���һ��������Ϣδ�ش�ʱ���Żᷢ�ʹ�����)
						{
							//2014.3.31 ����bug����λ�������޸�Ϊ��ͣ���Է������2����ɵ�ȷ�����ȷ��
							//���������ʱ�ſ������������ȷ���ź�ʧ������µ�ϵͳ����
							if(uart.rBuffer[5] == 0x01 && ((task_finished_num == 1 && task_unfinish_num == 1) || (task_finished_num == 2 && task_unfinish_num == 0)))			//1��OK��Ϣ
							{
								task_finished_num--;
								if(task_finished_num == 1)		//ԭ����2���Ѿ���ɵ����������ǰŲ����Ϊ�Ѿ���ȷ�յ�ǰһ������λ�����յ���Ϣ
								{
									task_finished_NO[0]		= task_finished_NO[1];
									task_ed_goods_numH[0]   = task_ed_goods_numH[1];
									task_ed_goods_numL[0]   = task_ed_goods_numL[1];
								}
							}
						}
					}
					break;
				}
			case WRITE_MULTI_BYTE:														//�ն��´�����
				{
					//����״̬�յ�0X10������,���Զ��˳������Ҳ�����
					//��ʼ����������������ݴ���ա�������Ӧ��
					if(uart.rBuffer[7]==DISPMODE_NONE && uart.rBuffer[6] == 0X0A)
					{
						ACK_Multi_Write();  
						Display_Clear();
						sys_data_reset();
					}
//					else if(uart.rBuffer[5] == 0X00 && uart.rBuffer[7] ==DISPMODE_TASK && uart.rBuffer[6] == 0X0A)
					else if( (uart.rBuffer[5] == 0X00 && uart.rBuffer[6] == 0X0A && uart.rBuffer[7] == DISPMODE_TASK)		   // �����κ�����
					       ||(uart.rBuffer[5] == 0X01 && uart.rBuffer[6] == uart.rIndex-9 && uart.rBuffer[7] ==DISPMODE_TASK && (uart.rBuffer[6]-uart.rBuffer[11]>=8)) )   // �����κ�����
					{
						//�·�������ʾ����ǰ����λ�����յ��ı���ǩ�ɼ���������������ͨ����������ȷ����������֮ǰ�Ѿ�����λ���ɼ����OK���Ĵ������ɴ˿���task_finished_num
						LastSoftGetOkCount = (uart.rBuffer[9]>>4)&0X0F; 
						//�˴������������Ѿ��������´������豸�����ұ��豸�Ѿ������ݻش������ǻش���������λ��������֮���ͨ�ų�����ʱ
						//�����½��������ݣ�ͨ������ǩ������ҵʣ��δ���������뵱ǰ����ģʽ�������ж�,�������LastSoftGetOkCount==0���������������գ�
						//2014.3.31����bug�������0��LastSoftGetOkCount�����ظ�����
						//1�Զ�����ʱ���������ʾ�����𡢽���������ʾ��ͬ�ĵ�һ�������𡢽�����......���豸ͨ�Ź���ʱ��
						//���;������ʱ��ͨ����λ���������ֱ���ǩ����������ɺ󣬷���clear������������clear״̬����ʾ״̬��
						//���clear״̬ʱ��LastSoftGetOkCount==0ʱ���ж���û�������������������������������������ACK_Multi_Write_RepeatError
						//���򣬴���
						//������������Ϣ

						if( (CurDispMode == DISPMODE_TASK)&&(f_repeat == TRUE) )
						{
							ACK_Multi_Write_RepeatError();		//���ظ��յ���ͬ�������Ӧ
							break;                                                                
						}
						ACK_Multi_Write();
						//�Ѿ��޸�Ϊ������ǰ����δ����������������������������ʾ�µģ�ԭ�Ȳ����գ�
						Commu_Task_Process(); 
						f_repeat	 = TRUE;		   // �����յ������־����ֹ�ظ�����
					}
					CurDispMode = uart.rBuffer[7];
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
		else if( (uart.rBuffer[0] == MULCAST_ADDR)	)			//�鲥��ַУ��
		{
		   
		   if( (uart.rBuffer[1] == WRITE_MULTI_BYTE)&&(uart.rBuffer[5] == 0X02)&& (uart.rBuffer[6] == uart.rIndex-9) )   // �鲥����
		   {
				
		    if(uart.rBuffer[7] == 0X00)
			  {	// �鲥�������
			  	   for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
						 Display_Clear();				// �����ʾ // �����������ܣ����飬����Ա�־�����ñ�־����ֵ���ı�־
						 sys_data_reset();				// ���������Ϣ��
						 CurDispMode = DISPMODE_NONE;	  // ��ǰ����״̬��Ϊ������״̬					     
						 break;
					  }
				   }
			  }
			  else if(uart.rBuffer[7] == 0X03)
			  {	// �鲥��ʾ����
					 CurDispMode = DISPMODE_TASK;
			  	 for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
			     		 Display_Info( &uart.rBuffer[10 + uart.rBuffer[9]]);			// ��ʾ��Ϣ
						   CurDispMode = DISPMODE_TASK;
						 break;
					  }
				   }
			  }
		   }
		}

}
/*******************************************************************************
	 					��ʾ����
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Info(INT8U * buf)
{
//	UINT8  usReTaskNum,len0;			//�����·���"���ӱ�ǩ��ʾ����"������

	UINT8  i = 0,curIndex = 0;
	//TASK_INFO XDATA *  pt;
	
	// ������״̬��ʱ�䡢ģʽ 	  uart.rBuffer[8] 
	i = * (buf);
	//1����˸�����
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	MulticastStatus = (i >> 5)&0X07;	     // �����ڵ��鱸��������
	if(digital_led_loading == OFF)         // �ǽ�����ģʽ�Ÿ��µ�����ʾֵ
	{
		// MulticastStatus  = led_flash_NO_BK;
		 led_flash_NO = MulticastStatus;
	}			 
    
	//2����˸ʱ����
	switch((i >> 3)&0X3)
	{
		case 0:		led_flash_period = 80;		break;
		case 1:		led_flash_period = 160;		break;
		case 2:		led_flash_period= 320;		break;

		default:	led_flash_period= 640;		break;
	}
	//3����˸ģʽ
  led_flash_mode   = (i)&0X07;
	//4����˸��������λ��ֹͣ��˸
	sys.led_flash_count = 0;			// ����˸ʱ�䵽
	led_flash_status = 0;			// ����
	task_unfinish_num = 0;		   // δ������������0
	task_finished_num = 0;		   // ��������������0
	task_unfinish_num ++;
}


/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void ResetAddr(void)
{

	//��ʾ���,���ݸ�λ
	Display_Clear();
	sys_data_reset();
	//��ַ����
//	address[0] = 0;		address[1] = 0;		address[2] = 0;
	para.str.address = 0;
	SaveParaToBlock();
}
/*******************************************************************************
	 					���������ʾ
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Clear(void)
{
  if( (TRUE == Key.b.Short) )
	{
	   	ClearFlg = ON;
	    
	}
  else
	{
   	ClearFlg = OFF;
		//led_flash_NO =  LED_RED;
		MulticastStatus= LED_NONE;
		led_flash_NO = LED_NONE;          //
		led_light_ctrl(LED_NONE);
	}
}
/*******************************************************************************
	 					ϵͳ������λ
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
        
        ///����������

        task_finished_NO[i] =0;    
        task_ed_goods_numH[i] =0;
        task_ed_goods_numL[i] =0; 
    }
	sys.digital_led_load_cnt = 0;
	digital_led_loading = OFF;	     
	sys.beWaitNewData = OFF;
	f_repeat	      = FALSE;		 // ���ظ����յ������־
	beSettingAddr = 0;
}
//////////////////////////////////////////////////////////////////////////////////
//// �������� : ��ַ��Ӧ����
//// ������� : 
//// ���ز��� : 
//// ˵��     :  
//////////////////////////////////////////////////////////////////////////////////
void ACK_Multi_ReadTestAddr(void)
{
	UINT8 addr;
	UINT16 usCRC;
//	UINT16 number;
        
	addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];    
//	number  = led[0].num*100 + led[1].num*10 + led[2].num; 

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
	uart.wBuffer[2] = 0x00;                         //function code
	usCRC = CRC16(uart.wBuffer, 3);
	uart.wBuffer[3] = (UINT8)usCRC;
	uart.wBuffer[4] = (UINT8)(usCRC>>8);
	uart.wLen   = 5;                            
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					�Զ��ֽڶ�������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Read(void)
{
	UINT8 addr;
	UINT16 usCRC;
//	UINT16 number;
    
	f_repeat	 = FALSE;		 // ���ظ����յ������־    
	addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];    
//	number  = led[0].num*100 + led[1].num*10 + led[2].num; 

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
        
	if(CurDispMode == DISPMODE_NONE)					//??0ģʽ�£�Ӧ���޲ɼ�����
	{
		uart.wBuffer[2] = 0x00;                     //n byte	
	        
		usCRC = CRC16(uart.wBuffer, 3);
		uart.wBuffer[3] = (UINT8)usCRC;
		uart.wBuffer[4] = (UINT8)(usCRC>>8);
		uart.wLen       = 5; 	        
	}
	else if(CurDispMode == DISPMODE_TASK)
	{
		//������ҵ�����һ�����ݣ�����Ҫ���ж�ʰȡ��Ϻ����ϴ�����,ԭ�����£�
		//���һ�����ݣ������2������ʾ�������һ����ʾ��ϲ����Ѿ�����OK������ʱ��ǩ��ʾ�ڶ�������ʱ��λ��ѯ�ʱ�ǩ�Ƿ��Ѿ�����OK��
		//��ʱʵ���Ѿ�����һ��OK�����豸�ش���һ������λ�������ѯ���Ƿ���OK������Ϊ�豸������һ�����񣩣���ʱ��ǩ�ûش𣿣�
		//������������У���λ��ѯ�ʵ���ǩ�Ѿ�����OK���󣬻Ჹ����һ���������һ��������ǩ֮ǰ���ǲ����вɼ�OK�������ģ�
		//��˿��Կ��ƣ����ǣ����2���޷���˿��ƣ���˱���һ���ϴ�
		{
			if(task_finished_num == 0)
		    {	// û�а���
				uart.wBuffer[2] = 0x02;                         //n byte    
	
				uart.wBuffer[3] = DISPMODE_TASK;                        
				uart.wBuffer[4] = 0X00;                    
	
				usCRC = CRC16(uart.wBuffer, 5);
				uart.wBuffer[5] = (UINT8)usCRC;
				uart.wBuffer[6] = (UINT8)(usCRC>>8);
				uart.wLen       = 7;                            
			}
			else  if(task_finished_num == 1)
			{	// 1������
				uart.wBuffer[2] = 0x04;                         //n byte    
	
				uart.wBuffer[3] = DISPMODE_TASK;                        
				uart.wBuffer[4] = task_finished_num;    
				uart.wBuffer[5] = task_ed_goods_numH[0];                        
				uart.wBuffer[6] = task_ed_goods_numL[0];    
	                
				usCRC = CRC16(uart.wBuffer, 7);
				uart.wBuffer[7] = (UINT8)usCRC;
				uart.wBuffer[8] = (UINT8)(usCRC>>8);
				uart.wLen       = 9;                            
			}
			else  if(task_finished_num == 2)
			{	// 2������
				uart.wBuffer[2] = 0x06;                         //n byte    
	
				uart.wBuffer[3] = DISPMODE_TASK;                        
				uart.wBuffer[4] = task_finished_num;    
				uart.wBuffer[5] = task_ed_goods_numH[0];                        
				uart.wBuffer[6] = task_ed_goods_numL[0];    
				uart.wBuffer[7] = task_ed_goods_numH[1];                        
				uart.wBuffer[8] = task_ed_goods_numL[1];    
	                
				usCRC = CRC16(uart.wBuffer, 9);
				uart.wBuffer[9] = (UINT8)usCRC;
				uart.wBuffer[10] = (UINT8)(usCRC>>8);
				uart.wLen       = 11;                            
			}
		}
	}       
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					�Զ��ֽ�д����������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 
}
/*******************************************************************************
	 					�Զ��ֽ�д����������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Write(void)
{
	UINT8 addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];
	UINT16 usCRC;

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x10;                         //function code
	uart.wBuffer[2] = 0x00;                         //start addr
	uart.wBuffer[3] = 0x00;
	uart.wBuffer[4] = 0x00;
	uart.wBuffer[5] = 0x05;

	usCRC = CRC16(uart.wBuffer, 6);

	uart.wBuffer[6] = (UINT8)usCRC;
	uart.wBuffer[7] = (UINT8)(usCRC>>8);

	uart.wLen        = 8; 
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					�Զ��ֽ�д����������Ӧ
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Write_RepeatError(void)
{
	UINT8 addr    = para.str.address;//para.str.address;//address[0] * 100 + address[1]*10 + address[2];
	UINT16 usCRC;

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x90;                         //ERROR code
	uart.wBuffer[2] = LastSoftGetOkCount;                       

	usCRC = CRC16(uart.wBuffer, 3);

	uart.wBuffer[3] = (UINT8)usCRC;
	uart.wBuffer[4] = (UINT8)(usCRC>>8);

	uart.wLen       = 5; 
	Send485SeriaDate((UINT8 *)uart.wBuffer, uart.wLen); 
}
/*******************************************************************************
	 					��ҵ����
	input:   none     
	output:  none					
*******************************************************************************/
void Commu_Task_Process(void)
{
	UINT16 usReTaskNum;				//�����·���"���ӱ�ǩ��ʾ����"������
	UINT32 unEplased_Led_LoadCount;	//������ȥ��ʱ��
	UINT8 i = 0,curIndex = 0;
	//��ʾ���ݻ�ȡ����λ�ţ�������
	//�����·���"���ӱ�ǩ��ʾ����"������     
	usReTaskNum = (uart.rBuffer[9])&0X0F;				
 	//������ҵ�л�δ�·��ĸ���
	task_unupdate_num = uart.rBuffer[10];

	if((task_unupdate_num != 0)||(usReTaskNum != 0))
	{ TaskStats = TASK_RUN; }	   // ��������
	//�ȳ�ʼ������
	//��2�����ʾ������������ԭ�������ݶ����������UN����ED��
	switch(LastSoftGetOkCount)
	{
	case 0:     //��ҵ������ʱ���һ�η�
		{
			//�������ݴ�����������һ����ҵ���������,��ǰ�������������(�豸2���ظ����մ�����ʱ),������������
			task_unfinish_num = 0;
			task_finished_num = 0;

//			ucBackDispMode = OFF;
			break;
		}
	case 1:    //��ҵ��������λ������������֮ǰ�ɼ���1����ɵ���ʾ
		{
			task_finished_num--;
			if(task_finished_num == 1)
			{
				//����ǰŲ
				task_finished_NO[0]		= task_finished_NO[1];
				task_ed_goods_numH[0]   = task_ed_goods_numH[1];
				task_ed_goods_numL[0]   = task_ed_goods_numL[1];
//				taks_finished_Full[0] 	= taks_finished_Full[1];
			}
			else
			{
				task_finished_num = 0;
			}
			break;
		}
	case 2:    //��ҵ��������λ������������֮ǰ�ɼ���2����ɵ���ʾ
		{
			task_finished_num = 0;
			break;
		}
	default:
		{
			task_finished_num = 0;
			break;
		}
	}
	//���ݱ����·���ʾ�������������Э�����
	switch(usReTaskNum)
	{
	case 0:		 //�޴������
		{
			break;
		}
	case 1:		//��1�������ݣ�����ǰʣ�����������
		{
			switch(task_unfinish_num)
			{
			case 0://δ�������0������1������������
			case 2://δ�������1������1������������,ʵ�ʴ���������ᷢ��������ʱ��ʵΪ�ϲ�������ȳ���Ŀǰ����ʽΪ���������������1������
				{	
					if(uart.rBuffer[5] == 0X00)			//û�����кŵ�Э��
					{
						curIndex = 11;
						task_unfinish_NO[0]		= uart.rBuffer[curIndex];
						task_un_goods_numH[0] 	= uart.rBuffer[curIndex+1];
						task_un_goods_numL[0]  	= uart.rBuffer[curIndex+2];                     
					}
					else if(uart.rBuffer[5] == 0X01)	//�����кŵ�Э��
					{
						//���к��Թ�
						//��λ��+����
						curIndex = 12+uart.rBuffer[11];			//uart.rBuffer[11]Ϊ��һ�����кų���
						task_unfinish_NO[0]		= uart.rBuffer[curIndex];
						task_un_goods_numH[0] 	= uart.rBuffer[curIndex+1];
						task_un_goods_numL[0]  	= uart.rBuffer[curIndex+2];                     
					}
					task_curOri_goods_numH =  task_un_goods_numH[0];
					task_curOri_goods_numL =  task_un_goods_numL[0];
					task_unfinish_num = 1;
					break;
				}
			case 1://δ�������1������1������������
				{
					if(uart.rBuffer[5] == 0X00)			//û�����кŵ�Э��
					{
						curIndex = 11;
						task_unfinish_NO[1]		= uart.rBuffer[curIndex];
						task_un_goods_numH[1] 	= uart.rBuffer[curIndex+1];
						task_un_goods_numL[1]  	= uart.rBuffer[curIndex+2];                     
					}
					else if(uart.rBuffer[5] == 0X01)	//�����кŵ�Э��
					{
						//���к��Թ�
						//��λ��+����
						curIndex = 12+uart.rBuffer[11];			//uart.rBuffer[11]Ϊ��һ�����кų���
						task_unfinish_NO[1]		= uart.rBuffer[curIndex];
						task_un_goods_numH[1] 	= uart.rBuffer[curIndex+1];
						task_un_goods_numL[1]  	= uart.rBuffer[curIndex+2];                     
					}
					task_unfinish_num = 2;
					break;
				}
			}
			break;
		}
	case 2:		//��2�������ݣ�ԭ�ȵĶ������ȫ������
		{
			if(uart.rBuffer[5] == 0X00)			//û�����кŵ�Э��
			{
				curIndex = 11;
				task_unfinish_NO[0]		= uart.rBuffer[curIndex];
				task_un_goods_numH[0] 	= uart.rBuffer[curIndex+1];
				task_un_goods_numL[0]  	= uart.rBuffer[curIndex+2];                     
	
				curIndex = 14;
				task_unfinish_NO[1]		= uart.rBuffer[curIndex];
				task_un_goods_numH[1] 	= uart.rBuffer[curIndex+1];
				task_un_goods_numL[1]  	= uart.rBuffer[curIndex+2];                     
			}
			else if(uart.rBuffer[5] == 0X01)	//�����кŵ�Э��
			{
				//���к�1�Թ�
				//��λ��+����
				curIndex = 12+uart.rBuffer[11];			//uart.rBuffer[11]Ϊ��һ�����кų���
				task_unfinish_NO[0]		= uart.rBuffer[curIndex];
				task_un_goods_numH[0] 	= uart.rBuffer[curIndex+1];
				task_un_goods_numL[0]  	= uart.rBuffer[curIndex+2];                     
				//���к�2�Թ�
				//��λ��+����
				curIndex = curIndex+3+(uart.rBuffer[6]- uart.rBuffer[11] - 11);		//(uart.rBuffer[6]- uart.rBuffer[11] - 11)Ϊ�ڶ������кų���
				task_unfinish_NO[1]		= uart.rBuffer[curIndex];
				task_un_goods_numH[1] 	= uart.rBuffer[curIndex+1];
				task_un_goods_numL[1]  	= uart.rBuffer[curIndex+2];                     
			}
			task_curOri_goods_numH =  task_un_goods_numH[0];
			task_curOri_goods_numL =  task_un_goods_numL[0];
			task_unfinish_num = 2;
			break;
		}
	}

	//��˸�����
    led_flash_NO	 = (uart.rBuffer[8] >> 5)&0X07;
	ClearFlg		 = OFF;

	//��˸ʱ����
	switch((uart.rBuffer[8] >> 3)&0X3)
	{
	case 0:		led_flash_period = 80;		break;
	case 1:		led_flash_period = 160;		break;
	case 2:		led_flash_period = 320;		break;
	case 3:		led_flash_period = 640;		break;
	default:	led_flash_period = 640;		break;
	}
	//��˸ģʽ
    led_flash_mode   = (uart.rBuffer[8] >> 0)&0X07;
	//��˸��������λ
	led_flash_status = 0;
	sys.led_flash_count = 0;

	//������ģʽ�´���
	if(sys.beWaitNewData)
	{	
		INT16U	MinWaitTime = (para.str.WtTime+1)*100;
		//��ʱ	sys.digital_led_load_cnt �ĳ�ʼֵΪDIGITAL_LED_REFRESH_DELAY_LONG
		unEplased_Led_LoadCount = DIGITAL_LED_REFRESH_DELAY_LONG - sys.digital_led_load_cnt;
		//�ȴ�ʱ�� > ���µȴ�ʱ��,ֱ��������ʾ����
		if(unEplased_Led_LoadCount >= MinWaitTime)//(WtTime+1)*100)
		{
			sys.beWaitNewData = OFF;
			sys.digital_led_load_cnt = 0;
		}
		//�ȴ�ʱ�仹û����С�ȴ�ʱ�䣬������ȴ�����Сʱ����������ʾ
		else
		{
//			sys.digital_led_load_cnt = (WtTime+1)*100 - unEplased_Led_LoadCount;
			sys.digital_led_load_cnt = MinWaitTime - unEplased_Led_LoadCount;
			//����ֱ������
		}
	}
}