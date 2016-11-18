#define PROCESS_GLOBAL
#include "include.h"
/*******************************************************************************
	 					处理MODBUS总站下发的命令
每组命令共14个字节；
BYTE_ADD,BYTE_HEAD,BYTE1,---> BYTE10 , CRC_H , CRC_L

BYTE_ADD 	: 终端地址
BYTE_HEAD	:  下传命令码
BYTE1		:  下传指令号
BYTE2		:  LED指示灯显示方式
BYTE3		:  本次下发的"电子标签"个数
BYTE4		:  主机中的此地址剩余"电子标签"个数
BYTE5-BYTE7	:   第一个"电子标签"内容
BYTE8-BYTE10:   第二个"电子标签"内容	
BYTE11		: CRC_H
BYTE12		: CRC_L

下传命令号:
00-	  清除设备的所有显示信息即标签将不工作;
01-	  电子标签与商品条码进行绑定模式；此时3~10位数据无效，数据侦长度2字节
02-    指示灯显示方式单独控制，此时3~10位数据无效，数据侦长度2字节
03-	  下发显示数据以及指示灯的信息，即拾取、上货、盘点作业；
         数据侦长度7字节或者10字节
*******************************************************************************/
void Cmd_Process(void)
{
	//数据包长度不够,不做Modbus应答
	
	if (uart.rIndex < ONE_CMD_MIN_LENGTH)
		return;   		
    //先看地址、CRC校验、数据长度等判断
	  //再根据第二个字节的范围判断是采用新协议解析还是旧协议解析
	  //地址与CRC数据校验,下传指令存储,MODBUS应答,CRC_Check函数中包含地址设定命令
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
	//0号广播命令地址，不需回复
	if(uart.rBuffer[0] == 0x00 && uart.rBuffer[1] == 0X06 && uart.rIndex == 8)	
	{
//		p_led_rxd = 0;
			
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
		//retCRC = usCRC;
		if (retCRC == usCRC) 						//crc校验成功 
		{
			if(uart.rBuffer[3] == 0X00)					//地址清零
			{
				ResetAddr();
			}
			else if(uart.rBuffer[3] == 0X01)				//接收地址，并显示，进入等待按键确认
			{
				if(beSettingAddr != 0X02)
				{				
					beSettingAddr = 1;
					para_bk.str.address = uart.rBuffer[5];	// 把设定地址存入备份区
					//设置更新信息，显示地址设定提示
					led_flash_mode = LED_FLASH_MODE_ALWAYS;	//常亮
					led_flash_NO = LED_GREEN;				//接收前亮第一组灯，接收后亮第二组灯
					ClearFlg	 = OFF;
				}
			}
			//0X10段	广播显示
			//00 06 ?? 10 ## ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X10)				//广播显示命令,如果## ## == 0X00 0X00，则显示地址
			{
				INT16U	usValue;
				usValue = ((UINT16)(uart.rBuffer[4])) *256 + (UINT16)(uart.rBuffer[5]);
				
			    //显示地址
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
			else if(uart.rBuffer[3] == 0X11)				//广播清除
			{
				Display_Clear();
				sys_data_reset();
			}
			//0X20段	广播存储段
			//00 06 ?? 20 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X20)				//广播设置参数1(标签对应拣货任务完成后亮灯模式)
			{
				led_flash_mode = LED_FLASH_MODE_ALWAYS;
				para.str.FinishedLED = uart.rBuffer[5];	
				led_flash_NO = para.str.FinishedLED;			
				
				ClearFlg	= OFF;
				SaveParaToBlock();
			}
			//00 06 ?? 21 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X21)				 //广播设置参数2(一对多任务时，进度条等待时间)
			{
				para.str.WtTime = uart.rBuffer[5];		 // 按下按键的颜色
				
 				led_flash_NO   = LED_GREEN ;	   			 // 亮的模式
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	 // 常亮模式
				led_flash_status = 0;						       // 先亮	 
				led_flash_period = 100;						     // 闪烁时间基准
				SaveParaToBlock();							       // 对参数做判断
			}
			//00 06 ?? 22 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X22)				 //广播设置参数3(功能键设定)
			{
				para.str.FnDefine = uart.rBuffer[5];	 // 写入参数

				led_flash_NO   = LED_GREEN ;	   			 // 亮的模式
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	 // 常亮模式

				led_flash_status = 0;						       // 先亮	 
				led_flash_period = 100;						     // 闪烁时间基准
				SaveParaToBlock();							       // 对参数做判断
			}
			//00 06 ?? 23 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X23)				 //广播设置参数4(显示模式设定)
			{				

				para.str.DispMode = uart.rBuffer[5];

				led_flash_NO   = LED_GREEN ;	   			  // 亮的模式
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	 	// 常亮模式
				led_flash_status = 0;						        // 先亮	 
				led_flash_period = 100;						      // 闪烁时间基准
				SaveParaToBlock();						        	// 对参数做判断
			}
			//00 06 ?? 24 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0X24)				  //广播设置参数5(按键颜色设定)
			{
				para.str.KeyPressLed = uart.rBuffer[5];	// 写入参数
				led_flash_NO = para.str.KeyPressLed ;	  // 亮的模式
				led_flash_mode = LED_FLASH_MODE_ALWAYS;	// 常亮模式
				ClearFlg	= OFF;
				SaveParaToBlock();						          // 对参数做判断				
			}
			//00 06 ?? 0xA0 ?? ## CRCH CRCL
			else if(uart.rBuffer[3] == 0XA0)				  //广播设置参数4(显示模式设定)///////////                
			{

         para.str.CBM7008LMD = uart.rBuffer[5];
               
				led_flash_NO   = LED_GREEN ;	   			   // 亮的模式
				ClearFlg	   = OFF;
				led_flash_mode = LED_FLASH_MODE_1V1;	   // 常亮模式
				led_flash_status = 0;						         // 先亮	 
				led_flash_period = 100;						       // 闪烁时间基准
				SaveParaToBlock();							         // 对参数做判断	
 
			}
		}

	}
	//其它，非广播命令，接收信息，需要回复
	else if (uart.rBuffer[0] == addr)				//地址校验
	{
		retCRC = CRC16   	(uart.rBuffer, uart.rIndex-2);		//CRC
		usCRC  = (UINT16)	(uart.rBuffer [uart.rIndex-2]);		//CRC_L;
		usCRC += (UINT16)	(uart.rBuffer [uart.rIndex-1]<<8);	//CRC_H
    //retCRC = usCRC;
		if (retCRC == usCRC) 						//crc校验成功 
		{
			switch (uart.rBuffer[1])
			{
			case READ_MULTI_BYTE://终端上传数据,0X03号modbus命令：通信时询问是否已经按完成键，设置地址时询问是否已经按完成键接收地址
				{
					if((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x01))			//寄存器数量01表示尝试地址是否已经设定的命令
					{
						ACK_Multi_ReadTestAddr();										//地址设定是否成功测试
					}
					else if ((uart.rBuffer[4] == 0x00) && (uart.rBuffer[5] == 0x03))	//寄存器数量03表示读取是否按OK键的命令
					{        				        
						ACK_Multi_Read();												//应答Modbus 0x03号采集按钮命令
					}
					break;
				}
			case WRITE_SINGLE_BYTE:	//控制器下传数据,0X06号modbus命令：锁定解锁标签
				{
					if(uart.rIndex == 0X08)						//所有06号命令都是8字节长度
					{
						ACK_Single_Write();
						//清除全部(清除显示信息、灯、蜂鸣器)，清除OK信息+扫描枪信息+灯塔信息
						if(uart.rBuffer[3] == 0X00)
						{
							Display_Clear();
							sys_data_reset();
							CurDispMode = DISPMODE_NONE;
						}
						//清除显示部分(清除显示信息、灯、蜂鸣器)，不包括，清除OK信息+扫描枪信息+灯塔信息
						else if(uart.rBuffer[3] == 0X01 || uart.rBuffer[3] == 0X02)
						{
							Display_Clear();
							sys_data_reset();
							CurDispMode = DISPMODE_NONE;
						}
						else if(uart.rBuffer[3] == 0x20)		//锁定解锁
						{
							if(uart.rBuffer[5] == 0x00)			//解锁
							{
								SystemLocked = OFF;
							}
							else if(uart.rBuffer[5] == 0x01)	//锁定
							{
								SystemLocked = ON;
							}
						}
						else if(uart.rBuffer[3] == 0x30)		//上位机收到采集按钮命令带回来的n个OK命令后，回传给设备，告知设备已经接收到了n个OK信息(只有在上位机收到1个OK信息，但是设备中还有一个任务信息未回传时，才会发送此命令)
						{
							//2014.3.31 发现bug，上位机驱动修改为不停尝试发送最后2个完成的确认命令，确保
							//下述情况下时才可以清除，避免确认信号失败情况下的系统错误
							if(uart.rBuffer[5] == 0x01 && ((task_finished_num == 1 && task_unfinish_num == 1) || (task_finished_num == 2 && task_unfinish_num == 0)))			//1个OK信息
							{
								task_finished_num--;
								if(task_finished_num == 1)		//原先有2个已经完成的任务，则后往前挪，因为已经明确收到前一个被上位机接收的信息
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
			case WRITE_MULTI_BYTE:														//终端下传数据
				{
					//配置状态收到0X10号命令,则自动退出配置且不保存
					//开始处理命令，包括数据暂存接收、各种响应等
					if(uart.rBuffer[7]==DISPMODE_NONE && uart.rBuffer[6] == 0X0A)
					{
						ACK_Multi_Write();  
						Display_Clear();
						sys_data_reset();
					}
//					else if(uart.rBuffer[5] == 0X00 && uart.rBuffer[7] ==DISPMODE_TASK && uart.rBuffer[6] == 0X0A)
					else if( (uart.rBuffer[5] == 0X00 && uart.rBuffer[6] == 0X0A && uart.rBuffer[7] == DISPMODE_TASK)		   // 无批次号命令
					       ||(uart.rBuffer[5] == 0X01 && uart.rBuffer[6] == uart.rIndex-9 && uart.rBuffer[7] ==DISPMODE_TASK && (uart.rBuffer[6]-uart.rBuffer[11]>=8)) )   // 有批次号命令
					{
						//下发本次显示数据前，上位机接收到的本标签采集到的数据组数，通过此数据来确定发此数据之前已经被上位机采集点的OK键的次数，由此控制task_finished_num
						LastSoftGetOkCount = (uart.rBuffer[9]>>4)&0X0F; 
						//此处纠正当网关已经将命令下传到本设备，并且本设备已经将数据回传，但是回传过程中上位机与网关之间的通信出问题时
						//不重新接收新数据（通过本标签本次作业剩余未操作个数与当前命令模式的与来判定,另外如果LastSoftGetOkCount==0的情况，则允许接收）
						//2014.3.31发现bug，如果是0个LastSoftGetOkCount，会重复接收
						//1对多任务时，会出现显示、拍灭、进度条、显示相同的第一个、拍灭、进度条......（设备通信故障时）
						//解决途径，暂时可通过上位驱动程序发现本标签本次任务完成后，发送clear命令，本程序记下clear状态与显示状态，
						//如果clear状态时，LastSoftGetOkCount==0时，判断有没有任务接收下来，如果有任务接收下来，则发送ACK_Multi_Write_RepeatError
						//否则，处理
						//任务驱动型信息

						if( (CurDispMode == DISPMODE_TASK)&&(f_repeat == TRUE) )
						{
							ACK_Multi_Write_RepeatError();		//回重复收到相同命令的响应
							break;                                                                
						}
						ACK_Multi_Write();
						//已经修改为（即当前若有未处理完的任务，来了新任务，则接收显示新的，原先不接收）
						Commu_Task_Process(); 
						f_repeat	 = TRUE;		   // 置已收到任务标志，防止重复发送
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
		else if( (uart.rBuffer[0] == MULCAST_ADDR)	)			//组播地址校验
		{
		   
		   if( (uart.rBuffer[1] == WRITE_MULTI_BYTE)&&(uart.rBuffer[5] == 0X02)&& (uart.rBuffer[6] == uart.rIndex-9) )   // 组播命令
		   {
				
		    if(uart.rBuffer[7] == 0X00)
			  {	// 组播清除命令
			  	   for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
						 Display_Clear();				// 清除显示 // 包含清除数码管，灯组，清回显标志和配置标志，数值更改标志
						 sys_data_reset();				// 清除任务信息区
						 CurDispMode = DISPMODE_NONE;	  // 当前任务状态设为非运行状态					     
						 break;
					  }
				   }
			  }
			  else if(uart.rBuffer[7] == 0X03)
			  {	// 组播显示命令
					 CurDispMode = DISPMODE_TASK;
			  	 for( i=0 ; i < uart.rBuffer[9]; i++ )
				   { 
				      if(uart.rBuffer[10+i] == addr)
					  {
			     		 Display_Info( &uart.rBuffer[10 + uart.rBuffer[9]]);			// 显示信息
						   CurDispMode = DISPMODE_TASK;
						 break;
					  }
				   }
			  }
		   }
		}

}
/*******************************************************************************
	 					显示命令
	input:   none     
	output:  none					
*******************************************************************************/
void Display_Info(INT8U * buf)
{
//	UINT8  usReTaskNum,len0;			//本次下发的"电子标签显示内容"个数，

	UINT8  i = 0,curIndex = 0;
	//TASK_INFO XDATA *  pt;
	
	// 灯亮的状态、时间、模式 	  uart.rBuffer[8] 
	i = * (buf);
	//1、闪烁灯序号
//	led.light_type	 = (uart.rBuffer[8] >> 5)&0X07;
	MulticastStatus = (i >> 5)&0X07;	     // 保存在灯组备份区域中
	if(digital_led_loading == OFF)         // 非进度条模式才更新灯组显示值
	{
		// MulticastStatus  = led_flash_NO_BK;
		 led_flash_NO = MulticastStatus;
	}			 
    
	//2、闪烁时间间隔
	switch((i >> 3)&0X3)
	{
		case 0:		led_flash_period = 80;		break;
		case 1:		led_flash_period = 160;		break;
		case 2:		led_flash_period= 320;		break;

		default:	led_flash_period= 640;		break;
	}
	//3、闪烁模式
  led_flash_mode   = (i)&0X07;
	//4、闪烁用数据置位，停止闪烁
	sys.led_flash_count = 0;			// 让闪烁时间到
	led_flash_status = 0;			// 先亮
	task_unfinish_num = 0;		   // 未完成任务个数清0
	task_finished_num = 0;		   // 已完成任务个数清0
	task_unfinish_num ++;
}


/*******************************************************************************
	input:   none     
	output:  none					
*******************************************************************************/
void ResetAddr(void)
{

	//显示清除,数据复位
	Display_Clear();
	sys_data_reset();
	//地址清零
//	address[0] = 0;		address[1] = 0;		address[2] = 0;
	para.str.address = 0;
	SaveParaToBlock();
}
/*******************************************************************************
	 					清除所有显示
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
//////////////////////////////////////////////////////////////////////////////////
//// 功能描述 : 地址响应测试
//// 输入参数 : 
//// 返回参数 : 
//// 说明     :  
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
	 					对多字节读命令响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Multi_Read(void)
{
	UINT8 addr;
	UINT16 usCRC;
//	UINT16 number;
    
	f_repeat	 = FALSE;		 // 清重复接收到任务标志    
	addr    = para.str.address;//address[0] * 100 + address[1]*10 + address[2];    
//	number  = led[0].num*100 + led[1].num*10 + led[2].num; 

	uart.wBuffer[0] = addr;                         //addr
	uart.wBuffer[1] = 0x03;                         //function code
        
	if(CurDispMode == DISPMODE_NONE)					//??0模式下，应该无采集命令
	{
		uart.wBuffer[2] = 0x00;                     //n byte	
	        
		usCRC = CRC16(uart.wBuffer, 3);
		uart.wBuffer[3] = (UINT8)usCRC;
		uart.wBuffer[4] = (UINT8)(usCRC>>8);
		uart.wLen       = 5; 	        
	}
	else if(CurDispMode == DISPMODE_TASK)
	{
		//本次作业的最后一组数据，则需要所有都拾取完毕后再上传数据,原因如下：
		//最后一组数据，如果有2个待显示，如果第一个显示完毕并且已经按下OK键，此时标签显示第二个，此时上位机询问标签是否已经按下OK键
		//此时实际已经按下一个OK键，设备回答按下一个，上位机会继续询问是否按下OK键（因为设备还有下一个任务），此时标签该回答？？
		//但是任务过程中，上位机询问到标签已经按下OK键后，会补发下一道命令，在下一道命令到达标签之前，是不会有采集OK键命令到达的，
		//因此可以控制，但是，最后2组无法如此控制，因此必须一起上传
		{
			if(task_finished_num == 0)
		    {	// 没有按下
				uart.wBuffer[2] = 0x02;                         //n byte    
	
				uart.wBuffer[3] = DISPMODE_TASK;                        
				uart.wBuffer[4] = 0X00;                    
	
				usCRC = CRC16(uart.wBuffer, 5);
				uart.wBuffer[5] = (UINT8)usCRC;
				uart.wBuffer[6] = (UINT8)(usCRC>>8);
				uart.wLen       = 7;                            
			}
			else  if(task_finished_num == 1)
			{	// 1个按下
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
			{	// 2个按下
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
	 					对多字节写命令正常响应
	input:   none     
	output:  none					
*******************************************************************************/
void ACK_Single_Write(void)
{
	Send485SeriaDate((UINT8 *)uart.rBuffer, uart.rIndex); 
}
/*******************************************************************************
	 					对多字节写命令正常响应
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
	 					对多字节写命令错误的响应
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
	 					作业命令
	input:   none     
	output:  none					
*******************************************************************************/
void Commu_Task_Process(void)
{
	UINT16 usReTaskNum;				//本次下发的"电子标签显示内容"个数，
	UINT32 unEplased_Led_LoadCount;	//拉条过去的时间
	UINT8 i = 0,curIndex = 0;
	//显示数据获取（货位号，数量）
	//本次下发的"电子标签显示内容"个数，     
	usReTaskNum = (uart.rBuffer[9])&0X0F;				
 	//本次作业中还未下发的个数
	task_unupdate_num = uart.rBuffer[10];

	if((task_unupdate_num != 0)||(usReTaskNum != 0))
	{ TaskStats = TASK_RUN; }	   // 任务运行
	//先初始化数据
	//有2组待显示数据下来，则将原来的数据都冲掉，包括UN区与ED区
	switch(LastSoftGetOkCount)
	{
	case 0:     //作业启动的时候第一次发
		{
			//回显数据处理区域，新来一次作业任务的数据,当前所有任务先清除(设备2此重复接收此命令时),回显数据清零
			task_unfinish_num = 0;
			task_finished_num = 0;

//			ucBackDispMode = OFF;
			break;
		}
	case 1:    //作业启动后，上位机发本次命令之前采集到1组完成的显示
		{
			task_finished_num--;
			if(task_finished_num == 1)
			{
				//后往前挪
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
	case 2:    //作业启动后，上位机发本次命令之前采集到2组完成的显示
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
	//根据本次下发显示的任务个数进行协议解析
	switch(usReTaskNum)
	{
	case 0:		 //无此种情况
		{
			break;
		}
	case 1:		//来1组新数据，看当前剩余个数做处理
		{
			switch(task_unfinish_num)
			{
			case 0://未处理完的0个，来1个，接收数据
			case 2://未处理完的1个，来1个，接收数据,实际此种情况不会发生，发生时其实为上层软件调度出错，目前处理方式为缓冲区情况，接收1组数据
				{	
					if(uart.rBuffer[5] == 0X00)			//没有序列号的协议
					{
						curIndex = 11;
						task_unfinish_NO[0]		= uart.rBuffer[curIndex];
						task_un_goods_numH[0] 	= uart.rBuffer[curIndex+1];
						task_un_goods_numL[0]  	= uart.rBuffer[curIndex+2];                     
					}
					else if(uart.rBuffer[5] == 0X01)	//带序列号的协议
					{
						//序列号略过
						//库位号+数量
						curIndex = 12+uart.rBuffer[11];			//uart.rBuffer[11]为第一个序列号长度
						task_unfinish_NO[0]		= uart.rBuffer[curIndex];
						task_un_goods_numH[0] 	= uart.rBuffer[curIndex+1];
						task_un_goods_numL[0]  	= uart.rBuffer[curIndex+2];                     
					}
					task_curOri_goods_numH =  task_un_goods_numH[0];
					task_curOri_goods_numL =  task_un_goods_numL[0];
					task_unfinish_num = 1;
					break;
				}
			case 1://未处理完的1个，来1个，接收数据
				{
					if(uart.rBuffer[5] == 0X00)			//没有序列号的协议
					{
						curIndex = 11;
						task_unfinish_NO[1]		= uart.rBuffer[curIndex];
						task_un_goods_numH[1] 	= uart.rBuffer[curIndex+1];
						task_un_goods_numL[1]  	= uart.rBuffer[curIndex+2];                     
					}
					else if(uart.rBuffer[5] == 0X01)	//带序列号的协议
					{
						//序列号略过
						//库位号+数量
						curIndex = 12+uart.rBuffer[11];			//uart.rBuffer[11]为第一个序列号长度
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
	case 2:		//来2组新数据，原先的都冲掉，全部接收
		{
			if(uart.rBuffer[5] == 0X00)			//没有序列号的协议
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
			else if(uart.rBuffer[5] == 0X01)	//带序列号的协议
			{
				//序列号1略过
				//库位号+数量
				curIndex = 12+uart.rBuffer[11];			//uart.rBuffer[11]为第一个序列号长度
				task_unfinish_NO[0]		= uart.rBuffer[curIndex];
				task_un_goods_numH[0] 	= uart.rBuffer[curIndex+1];
				task_un_goods_numL[0]  	= uart.rBuffer[curIndex+2];                     
				//序列号2略过
				//库位号+数量
				curIndex = curIndex+3+(uart.rBuffer[6]- uart.rBuffer[11] - 11);		//(uart.rBuffer[6]- uart.rBuffer[11] - 11)为第二个序列号长度
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

	//闪烁灯序号
    led_flash_NO	 = (uart.rBuffer[8] >> 5)&0X07;
	ClearFlg		 = OFF;

	//闪烁时间间隔
	switch((uart.rBuffer[8] >> 3)&0X3)
	{
	case 0:		led_flash_period = 80;		break;
	case 1:		led_flash_period = 160;		break;
	case 2:		led_flash_period = 320;		break;
	case 3:		led_flash_period = 640;		break;
	default:	led_flash_period = 640;		break;
	}
	//闪烁模式
    led_flash_mode   = (uart.rBuffer[8] >> 0)&0X07;
	//闪烁用数据置位
	led_flash_status = 0;
	sys.led_flash_count = 0;

	//进度条模式下处理
	if(sys.beWaitNewData)
	{	
		INT16U	MinWaitTime = (para.str.WtTime+1)*100;
		//此时	sys.digital_led_load_cnt 的初始值为DIGITAL_LED_REFRESH_DELAY_LONG
		unEplased_Led_LoadCount = DIGITAL_LED_REFRESH_DELAY_LONG - sys.digital_led_load_cnt;
		//等待时间 > 最新等待时间,直接亮灯显示数据
		if(unEplased_Led_LoadCount >= MinWaitTime)//(WtTime+1)*100)
		{
			sys.beWaitNewData = OFF;
			sys.digital_led_load_cnt = 0;
		}
		//等待时间还没到最小等待时间，则继续等待到最小时间后才亮灯显示
		else
		{
//			sys.digital_led_load_cnt = (WtTime+1)*100 - unEplased_Led_LoadCount;
			sys.digital_led_load_cnt = MinWaitTime - unEplased_Led_LoadCount;
			//避免直接亮灯
		}
	}
}