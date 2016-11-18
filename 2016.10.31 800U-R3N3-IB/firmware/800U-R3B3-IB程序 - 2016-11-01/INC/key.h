#ifndef __KEY_H_
#define __KEY_H_

#ifdef  KEY_GLOBAL
  #define EXT_KEY
#else
  #define EXT_KEY extern
#endif

#define MAX_KEYPRESSLED     8    	 //1
#define CMD_KEY_NULL 	0
#define CMD_KEY_OK		1	
	
	
EXT_KEY  volatile UINT8 idata KeyStatus_flag;		//����״̬��־
EXT_KEY INT8VU idata digital_led_loading ;			// �������ȴ���־��OFF�ǵȴ���ON�ȴ�
EXT_KEY INT8VU idata ClearFlg ;							   	// ָʾ�������־����Ϊ����ָʾ���ȣ�=ON����˵�����������δִ��	
EXT_KEY UINT8 idata f_KeyEnable;	
	

EXT_KEY INT8VU idata beSettingAddr ;						// 0:������վַ		1:����վַ		2��վַ�趨�Ѿ�����OK��

EXT_KEY UINT8 idata	keyStatus;			//���ΰ��µİ���
EXT_KEY UINT8 idata	keyLastDown;		//�ϴΰ��µİ���	

	
	
EXT_KEY struct{
	struct{
		INT8U	Short:1;			// �����ͷű�־ 0�ͷ�
		INT8U	Long:1;				// ����������־ 1���� 	
		INT8U	LongOnce:1;		// ��������ִ��һ�α�־
		INT8U	RVS:5;
    }b;	
  INT8U	SysCnt;					// ��ϵͳʱ�Ӽ�ʱ
	INT8U	OnCnt;					// ���¼���ʱ�䣬��λΪ32ms
	INT8U	Backup;					// �����ϴεļ�ֵ
} Key;




#define MAX_CFGPARA      7    //6      	 //���ò���������
typedef union 
{
	struct{
	 
		INT8VU 	  address;				 // ��ַ(1~255)
		INT8VU 	  FinishedLED;		 // ��ɺ��LED�������(0,1,2,3,4,5,6,7-->��,��,��,��,��,��,��,��)
		INT8VU 	  WtTime;				   // �������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
		INT8VU 	  FnDefine;				 // ���ܼ�����(0,1->���,�������)
		INT8VU 	  DispMode;				 // ��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ
		INT8VU 	  KeyPressLed;		 // OK������ָʾ��(0,1,2,3,4,5,6,7,8-->��,��,��,��,��,��,��,��,8ǰһ�ֵ�ɫ+1)����800U ����оƬ�� 
		INT8VU	  CBM7008LMD;			 // ��������������
	}str;
	INT8VU	Byte[MAX_CFGPARA];
}STORAGE_UNI;



EXT_KEY	STORAGE_UNI idata  para;		   // ��������
EXT_KEY	STORAGE_UNI idata  para_bk;	   // ���ݲ�������


extern INT8U Scan_Val();
extern void Key_Scan(void);
extern void Process_Keyboard(void);

#endif
