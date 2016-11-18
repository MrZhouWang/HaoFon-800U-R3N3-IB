
#ifndef _KEY_H_
#define _KEY_H_

/************************************************************************************/
#ifdef  AP_KEY_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_KEY_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_KEY_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/
sbit KEY_OK  	  = P1^5;

//-----------------------------------------------------------------------------------
// ��������
#define TIMER_COUNT_100mS	100
#define TIMER_COUNT_1S	500
#define TIMER_COUNT_3S	800
#define TIMER_COUNT_32ms	16

AP_KEY_EXT	void Key_Scan(void);
AP_KEY_EXT	void Process_Keyboard(void);

#endif
