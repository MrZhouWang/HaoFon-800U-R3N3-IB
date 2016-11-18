/*=========================================================
// �������˵��
//  2015��2��12�ո���  �汾v1.0
//	 1.��800U-T3N3_V1.2�Ļ������޸ģ������ذ�����Ϊ��ͨ����
     2.�������ܲ���
//	 MCU:STC15W408AS   8K Flash  512RAM  EEPROM 5K 	  UART ����ɵ������� SOP16��װ��14��IO�ڣ�
         �ڴ�������ϵ縴λ��·

// ����˵��
F1���� �������������ɺ��������ɫ������0~7��8����ɫ��Ĭ����
F2���� �������Ļ�׼ʱ��
F3���� �������µĵ���ɫ������0~8��9�֣�0~7��Ӧ���Ե���ɫ��8Ϊ����

����״̬��
���°������Ƹ���F3�����ã�����Ӧ�ĵ�
�ɿ�ʱ������ȫ�������F1����ɫ��δ��ɣ���������ʱ����ʱ�䣬����ͨѶ�·��ĵ�ɫ
�ǹ���״̬��
���°������Ƹ���F3�����ã�����Ӧ�ĵ�
�ɿ�ʱ����
����״̬��
�������õ�ַ���̵�
�������������
�����������

// 2014��11��10�ո���
1. F3 ����������ΪKeyPressLed��=8ʱ��������ɫΪ�ϸ�״̬+1������7���1
2. ����ֻ��������
3. ������ɺ󣬵�״̬��������������������
4. FT PTL ����������ʱ�ᷢ��������F1�����򲻷��������
5. ����������ɫ�������������ĵ���ɫ

// 2014��12��19�ո��� V1.1
1. �����洢��������P6һ��
2. ͨѶ�ط��ж�ͬP6һ��
3. �������������Ȼ���32�ȽϺ���
4. �������µ�ֵĬ��Ϊ��ɫ
5. ������ɨ�谴����ԭ��ɨ�谴������ִ��

// 2014��12��24�ո��� V1.2
1. �㲥���趨�����ȣ�0xA0,������Ϊ32~48�����ڴ˷�Χ����ָ���Ĭ��ֵ32

//2015��4��7�ո���Ϊ800U-P3N3-T3N3 IB_V2.0_20150407
1.��800U-T3N3_V1.2�����ϣ��������������ܣ���ͨ����������ѡ�񴥿ع��ܻ��߰�ѹ����
2.����������������ԭ����32��Ϊ10
3.ɾ������ע�͵��Ĺ����ظ��ĺ����Ĵ���

=========================================================*/

#define AP_CPU_GLOBALS	
#include "extern_symbol.h" 

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

/*******************************************************************************
	 					1�����ӳٺ���
*******************************************************************************/
void Delay1Ms() 
{
    UINT16 i = 852;
    while (i != 0) {i--;}
}

/*******************************************************************************
	 					�����ӳٺ���
    input   usDelay     �ӳ�ʱ��ֵ
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
	 					  �˿ڳ�ʼ��
*******************************************************************************/
void IO_Init(void)
{
	//��©���	PxM1��PxM0��Ӧ���11
	P3M1 = 0x0C;				   // SDA��SCL����ɿ�©��3.3V
	P3M0 = 0x0C;	

	P1M1 = 0x1C;				   // BLUE��RED��GREEN����ɿ�©��5V
	P1M0 = 0x1C;
	//������� PxM1��PxM0��Ӧ���01
	P1M1 &= (~0x03);			  // 485CTL������죬5V
	P1M0 |= 0x03;		


	//IO״̬��ʼ��
	led_light_ctrl(LED_NONE);

//	sys.flag = OFF;
//	sys.count= 2;

//	address[0] = 0xff;
//	address[1] = 0xff;
//	address[2] = 0xff;
//	baudrate   = BD_38400;
}

/***************************************************
		T0��ʱ����ʼ��
***************************************************/
#define TH0_INIT        0xf8
#define TL0_INIT        0xcc

void InitT0(void)
{
    //��ʱ1ms//Ĭ��Ϊ12Tģʽ=>T0_CLK=SYS_CLK/12=11 059 200 /12 =921600
    TMOD = (TMOD & 0xf0) | 0x01;    //mode 1: 16 bit counter
    TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

    ET0  = 1;                       //allow interrupt
    TR0  = 1;                       //open timer0
}

/*******************************************************************************
	��ʱ��T0�жϷ������(ms�ж�)
	Input :		�ж�����
	Output :	sys.flag ��Ϊ��Ч	2ms
*******************************************************************************/

void timer0_int (void) interrupt 1
{ 
//    p_Test = ~p_Test;
	  TH0  = TH0_INIT;
    TL0  = TL0_INIT;  

//    sys.flag  = ON;
    //���ڻ�����������
    if (uart.rIndex != 0)	 			
    {
        if (uart.rTimer-- == 0) 
        {
			uart.rFlag  = ON;    
        }
    }

	// ����ɨ�����ʱ��
	if(Key.SysCnt++ >= TIMER_COUNT_32ms )
	{ f_KeyEnable = TRUE; Key.SysCnt = 0; }

    //�ݼ����������Լ�1
    if (sys.digital_led_load_cnt != 0)
        sys.digital_led_load_cnt--;    
    
    //�ݼ����������Լ�1
//    if (sys.count != 0)
//        sys.count--;

    //�ݼ����������Լ�1
    if (sys.led_flash_count != 0)
        sys.led_flash_count--;

 }

/*******************************************************************************
	 					�����Լ�����
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

	DelayMs(300);    		// �ȴ���Դ�ȶ�
	
	InitT0();
	
	UART_Init();		  	//

	Start_Init();			// ����״̬
	
 	LoadParaInEeprom();	    // ��ȡ������ֵ
    
    #if	( MACHINETYPE == T3N3_IB_800U)
       CBM7008_init();			// ���뿪����200ms������������	
    #endif
    
	EA = 1;					//ʹ���ж�

	Display_Clear();

// 	uart.wBuffer[0] = 0x55;
// 	SendSerialByte(&para.Byte[0],MAX_CFGPARA);
// 	SendSerialByte((UINT8 *)uart.wBuffer,1);
// 	SendSerialByte(&lmd[0], 8);
// 	uart.wBuffer[0] = 0xAA;
// 	SendSerialByte((UINT8 *)uart.wBuffer,1);

	p_m485_ctrl = 0;							// �رշ���DE�˿ڣ��򿪽���/RE485�˿�
	led_flash_period = 300;						//��˸���ڿ���
	led_flash_mode 	 = LED_FLASH_MODE_ALWAYS ;	//����ģʽ
	TaskStats 		 = TASK_NONE;		// �ǹ���״̬
	ClearFlg	   = OFF;
	Key.b.Short    = FALSE;				// �̰��ͷ�
	Key.b.LongOnce = FALSE;				// ����һ���ͷ�	
	Key.b.Long 	   = FALSE;				// �����ͷ�
	SystemLocked   = OFF;				// ϵͳ����
	CurDispMode    = DISPMODE_NONE;  	// ��ǰ����״̬��Ϊ������״̬
    
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
