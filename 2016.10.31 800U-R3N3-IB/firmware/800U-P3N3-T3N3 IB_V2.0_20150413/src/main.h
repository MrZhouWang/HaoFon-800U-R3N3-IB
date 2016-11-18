#ifndef _MAIN_H_
#define _MAIN_H_

/********************************************************************************************************
*                                              Global CONSTANS
********************************************************************************************************/
/***********************************************************************************
* ������������(�ο�uC/OS_II.H)
*	1���� extern_symbol.h ��ÿ��ȫ�ֱ��������� AP_CPU_EXT ��ǰ׺
* 	2���� main.c �в�������Ч����  	
*      	#define OS_CPU_GLOBALS��OS_CPU_EXT ��ǿ��Ϊ �� ��(��)
*		#include "Head File/extern_symbol.h"�����б��������� 
*	3���������ļ��в�������Ч����
*		δ���� OS_CPU_GLOBALS��OS_CPU_EXT ��ǿ��Ϊ extern
*		#include "Head File/extern_symbol.h"�����б����� ��extern�� ������
*	4���� main.c ��ͬʱ�� #include "Head File/parameter_map.h" ���������Ϊ��
*		A���� extern_symbol.h �����ȶ���ṹ���ͣ����������������ռ�)
*		B��Ȼ���� parameter_map.h �жԱ������г�ʼ��
* 		C�������ļ��� ���� extern_symbol.h����ʱ���ȶ���ṹ���ͣ�Ȼ������ extern ����
*	5��ע�⣺�� main.c �У������� #define OS_CPU_GLOBALS Ȼ�������� parameter_map.h
************************************************************************************/
#ifdef  AP_CPU_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_CPU_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_CPU_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/

#define	T3N3_IB_800U			0         //����������800Uָʾ��
#define	P3N3_IB_800U    		1         //��ѹ������800Uָʾ��

#define	MACHINETYPE  T3N3_IB_800U          //800U-T3N3 IB
//#define	MACHINETYPE  P3N3_IB_800U       //800U��P3N3 IB

#if	 (MACHINETYPE == T3N3_IB_800U)
	#include "CBM7008.H"

#elif(MACHINETYPE == P3N3_IB_800U)
	#include "key.H"	
    
#endif

sbit p_Test        = P1^0;

#define KEY_STATUS  ( ((P3 & 0x38)>>2) +((P2& 0x10)>>4) )

#define CMD_KEY_NULL 	0
#define CMD_KEY_OK		1

//�´���������
#define DISPMODE_NONE	0X00		//����������ݣ�����ʾ,led����ʾ
#define DISPMODE_ASII	0X02		//�ַ�����ʾ
#define DISPMODE_TASK	0X03		//��ҵ����


#define TASK_NONE		0X00		//�ǹ���״̬
#define TASK_RUN		0X01		//����״̬
#define TASK_FINISH		0X02		//�������״̬



#define TIMER_DATA_CHANGE	50      //100ms
#define TIMER_CONFIG        5000    //30s  �޸�Ϊ 10s
#define TIMER_MODIFY        5000    //10S

#define MAX_TASK_NUM 	2	//������ҵ��������
#define MAX_BACK_NUM	5	//��������Ϣ֧��

#define DIGITAL_LED_REFRESH_DELAY  600    			// 1=1S
#define DIGITAL_LED_REFRESH_DELAY_LONG  800000    	// 1=1S

///////////////////////////////ȫ�ֱ���/////////////////////////////////////
//AP_CPU_EXT volatile UINT16 usCountCfg;						//�ް�������ʱ���ü�����
//AP_CPU_EXT volatile UINT16 usCountModify;					//�ް�������ʱ�޸ļ�����

//AP_CPU_EXT volatile UINT16 led_static_warn_cnt;

AP_CPU_EXT INT8VU 	DATA led_flash_NO ;							// LEDָʾ���������ͣ�0~7����Ӧ��,��,��,��,��,��,��,��
AP_CPU_EXT INT16VU 	DATA led_flash_period ;						// ��˸���ڿ���
AP_CPU_EXT INT8VU 	DATA led_flash_mode ;						// ����ʱ���������:0����
AP_CPU_EXT INT8VU 	DATA led_flash_status;						// ��˸���Ƶ�ʱ���Ƶ�ǰ��״̬,0������1������
AP_CPU_EXT INT8VU 	DATA led_flash_NO_BK;						// ������Ʊ�������(����������ʹ��)

AP_CPU_EXT INT8VU 	DATA CurDispMode ;							// ��Ӧ��3����ʾģʽ��0-�����ݣ�1-��ʾ�ַ�����3-��ʾ�ּ�����(1,3ʵ��)
																// 00Ϊ������״̬,01,03Ϊ����״̬

AP_CPU_EXT INT8VU 	DATA TaskStats ;							// ��������״̬	00=�ǹ���״̬��1=����״̬��2=�������״̬
AP_CPU_EXT INT8VU 	DATA ClearFlg ;								// ָʾ�������־����Ϊ����ָʾ���ȣ�=ON����˵�����������δִ��


AP_CPU_EXT INT8VU 	DATA beSettingAddr ;						// 0:������վַ		1:����վַ		2��վַ�趨�Ѿ�����OK��
AP_CPU_EXT INT8VU 	DATA LastSoftGetOkCount;     				// ��λ������·��˴�����֮ǰ����0X03������ɼ������Ѿ�OK��Ӧ������
AP_CPU_EXT INT8VU 	DATA task_unupdate_num ;		        	// ����δ�´���ҵ��������
AP_CPU_EXT INT8VU 	DATA task_curOri_goods_numH ;				// ��ǰ��ʾ��Ʒ����ʾ������λ����
AP_CPU_EXT INT8VU 	DATA task_curOri_goods_numL ;				// ��ǰ��ʾ��Ʒ����ʾ������λ����

//δ��������
AP_CPU_EXT INT8VU 	DATA task_unfinish_num ;					// δ�����������= 0,1...MAX_TASK_NUM
AP_CPU_EXT INT8VU 	IDATA task_unfinish_NO[MAX_TASK_NUM];		// ��λ��
AP_CPU_EXT INT8VU 	IDATA task_un_goods_numH[MAX_TASK_NUM];		// ������λ
AP_CPU_EXT INT8VU 	IDATA task_un_goods_numL[MAX_TASK_NUM] ;	// ������λ

AP_CPU_EXT INT8VU 	DATA f_repeat;								// �����ط���־��TRUE=�ѷ�������δִ�е���������ñ�־����������OK����ȡ������
///����������
AP_CPU_EXT INT8VU 	DATA  task_finished_num ;		  			// �������������= 0,1...MAX_TASK_NUM
AP_CPU_EXT INT8VU 	IDATA task_finished_NO[MAX_TASK_NUM] ;	  	// ��λ��
AP_CPU_EXT INT8VU 	IDATA task_ed_goods_numH[MAX_TASK_NUM] ;	// ������λ
AP_CPU_EXT INT8VU 	IDATA task_ed_goods_numL[MAX_TASK_NUM];		// ������λ

//AP_CPU_EXT INT8VU 	DATA CurBackDispIndex ;						// ��ǰ��������
//AP_CPU_EXT INT8VU 	IDATA DispContentStack_NO[MAX_BACK_NUM];	// ������Ϣ�洢�Ŀ�λ��
//AP_CPU_EXT INT8VU 	IDATA DispContentStack_numH[MAX_BACK_NUM];	// ������Ϣ�洢��������λ
//AP_CPU_EXT INT8VU 	IDATA DispContentStack_numL[MAX_BACK_NUM];	// ������Ϣ�洢��������λ

AP_CPU_EXT INT8VU 	DATA SystemLocked;							// ����ϵͳ������ϵͳ	 ON=����ϵͳ���������ܲ���

AP_CPU_EXT INT8U IDATA lmd[8];									// CMB7008������

typedef struct
{
	UINT32 digital_led_load_cnt;		// ����ܵȴ���������
	UINT8  beWaitNewData;				// ������ʾ�������������ǰ�Ƿ�Ϊ���ڵȴ������ݹ���
										// �ж��Ƿ������С�����ȴ������豸�������ݣ���������ҵ�������µ���ҵ��������������ȴ�������������ʱ��Ҳ���ȵȴ�������
	UINT16 led_flash_count;				// �ݼ���������ϵͳ�ڶ�ʱ����1MS�Լ�1
//	UINT16 count;						// �ݼ���������ϵͳ�ڶ�ʱ����1MS�Լ�1
//	UINT8  flag;						//led_Scan()==>LEDɨ������
}OS_SYS;
AP_CPU_EXT volatile OS_SYS  DATA sys;


//AP_CPU_EXT  INT16VU DATA   digital_led_loading_cnt;		   // 
AP_CPU_EXT  INT8VU 	DATA    digital_led_loading ;			   // �������ȴ���־��OFF�ǵȴ���ON�ȴ�

//AP_LED_EXT UINT8 ucCfgStatus;


// �����洢
//AP_CPU_EXT UINT8 address[3];
//AP_CPU_EXT UINT8 toSettedAddr[3];
////AP_LED_EXT UINT8 baudrate;
//AP_CPU_EXT UINT8 ucAddr[3];               
////AP_LED_EXT UINT8 ucBaud;   
//AP_CPU_EXT UINT8 FinishedLED;					   //��ɺ��LED�������(0,1,2,3,4,5,6,7,8 ;��,��,��,��,��,��,��,��)
//AP_CPU_EXT UINT8 WtTime;						   //�������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
//AP_CPU_EXT UINT8 KeyPressLed;					   //���������µ���ɫ����(0,1,2,3,4,5,6,7,8 ;��,��,��,��,��,��,��,��,ԭ������ɫ�����������1���ױ�Ϊ��)
//AP_CPU_EXT UINT8 DispMode;						   //��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ

#define MAX_CFGPARA       	7//6      	 //���ò���������
typedef union 
{
	struct{
	 
		INT8VU 	  address;				 // ��ַ(1~255)
		INT8VU 	  FinishedLED;			 // ��ɺ��LED�������(0,1,2,3,4,5,6,7-->��,��,��,��,��,��,��,��)
		INT8VU 	  WtTime;				 // �������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
		INT8VU 	  FnDefine;				 // ���ܼ�����(0,1->���,�������)
		INT8VU 	  DispMode;				 // ��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ
		INT8VU 	  KeyPressLed;			 // OK������ָʾ��(0,1,2,3,4,5,6,7,8-->��,��,��,��,��,��,��,��,8ǰһ�ֵ�ɫ+1)����800U ����оƬ�� 
		INT8VU	  CBM7008LMD;			 // ��������������
	}str;
	INT8VU	Byte[MAX_CFGPARA];
}STORAGE_UNI;



AP_CPU_EXT	STORAGE_UNI  IDATA  para;		   // ��������
AP_CPU_EXT	STORAGE_UNI  IDATA  para_bk;	   // ���ݲ�������


// ������������
//AP_CPU_EXT  bit bdata f_KeyEnable ;
AP_CPU_EXT UINT8 DATA f_KeyEnable;

AP_CPU_EXT struct{
	struct{
//		INT8U	Enable:1;			// ��������
		INT8U	Short:1;			// �����ͷű�־ 0�ͷ�
		INT8U	Long:1;				// ����������־ 1���� 	
		INT8U	LongOnce:1;			// ��������ִ��һ�α�־
		INT8U	RVS:5;
    }b;	
	INT8U	SysCnt;					// ��ϵͳʱ�Ӽ�ʱ
	INT8U	OnCnt;					// ���¼���ʱ�䣬��λΪ32ms
	INT8U	Backup;					// �����ϴεļ�ֵ
}DATA Key;


AP_CPU_EXT UINT8 DATA	keyStatus;			//���ΰ��µİ���
AP_CPU_EXT UINT8 DATA	keyLastDown;		//�ϴΰ��µİ���
//AP_CPU_EXT UINT8 DATA	keyIsCFGLPressed;	//��ǰ����CFG����ʱ���Ƿ���CFGL��Ч��
//AP_KEY_EXT UINT8 ucConfig;			//ϵͳ���ñ�־λ=0 -> ��������״̬��=1 ->������״̬
//AP_CPU_EXT UINT8 DATA	ucBackStatus ;		//0���ǻط�״̬��1���ط���һ����2���ط��ڶ�����......���ط�10��
//AP_CPU_EXT UINT8 DATA	ucChangeValue;		//������ʾ��ֵ״̬�£���ʾ�Ƿ������޸Ľ���λ�������֣��޸Ľ���λ�£���λ0��Ҫ��ʾ��������Ҫ��ʾ��
//AP_CPU_EXT UINT8 DATA	ucBackDispMode;	//�Ƿ�Ϊ����ģʽ


// ����
#define UART_BUF_LEN	50//���ڻ�������С

typedef struct
{
	UINT8 wLen;						// ���͸���
	UINT8 wBuffer[UART_BUF_LEN];	// ���ͻ�����
	UINT8 rIndex;					// ���ո���
	UINT8 rBuffer[UART_BUF_LEN];	// ���ջ�����
	UINT8 rTimer;					// ��ʱ��
	UINT8 rFlag;			 		// ���ջ������Ƿ�����=ON -> ����, =OFF����(�ڶ�ʱ���в�ѯ������״̬)
}OS_UART;

AP_CPU_EXT volatile OS_UART XDATA uart;
///////////////////////////////ȫ�ֱ���-END-////////////////////////////////
// ��������
AP_CPU_EXT	void sys_data_reset();
AP_CPU_EXT	void Delay1Ms(); 
AP_CPU_EXT	void DelayMs(UINT16 usDelay);
AP_CPU_EXT  void delay_us(INT16U us);

//AP_CPU_EXT	INT8U 	iic_write_data(INT8U waddr, INT8U wdata);
//AP_CPU_EXT	INT8U 	iic_rea_Ndata(INT8U raddr);



#endif
