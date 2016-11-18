#ifndef _MAIN_H_
#define _MAIN_H_

#include "include.h"



#ifdef MAIN_GLOBAL
#define EXT_MAIN
#else
#define EXT_MAIN extern
#endif

#define		FSLEDONTIME	 5	 	   			  // ռ�ձȴ򿪷���ܳ���ʱ�� ��ʱ����ʱ��������ڴ�ֵʱ��һ�η����
#define		Dval	       60	       			// �ڵ����׼ֵ��ֵ�Ƚ�        Խ���豸Խ�ȶ�   60�൱�� 0.71V
#define		Dnum	       40  	          // �ڵ������Ƚ�    
#define   SYSTEM_INIT	 0         			// ϵͳ״̬
#define   KEY_COUNT    20000          //�����ضϼ�ʱ  ���ڰ����ƿ�ʱ��ɫƽ���л�
#define   BASE_AD_COUNT  50           //��¼�ϵ翪ʼ�ж�ǰ����ٸ�����������Ϊ��׼����

#define   OK 8
#define   NO_OK 0


///***��׼�������ж�����Խ�ǿ���Խ�ǿ����жϾ�ȷ************************/
//#define   First_Base  50              //��һ�ο�ʼȷ�����ڿ���
//#define   Second_Base 55              //�ȴ�ʮ����������֮����Ȼ�ڿ���
//#define   StardBase   3              //С�ڴ����ݱ�ʾ�ڿ���           


#define MAX_TASK_NUM 	2	//������ҵ��������
#define MAX_BACK_NUM	5	//��������Ϣ֧��

#define TASK_NONE		0X00		//�ǹ���״̬
#define TASK_RUN		0X01		//����״̬
#define TASK_FINISH		0X02		//�������״̬

#define DIGITAL_LED_REFRESH_DELAY  600    			    // 1=1S
#define DIGITAL_LED_REFRESH_DELAY_LONG  800000    	// 1=1S












EXT_MAIN INT8VU idata TaskStats ;							// ��������״̬	00=�ǹ���״̬��1=����״̬��2=�������״̬

EXT_MAIN INT8VU idata LastSoftGetOkCount;     				// ��λ������·��˴�����֮ǰ����0X03������ɼ������Ѿ�OK��Ӧ������
EXT_MAIN INT8VU idata task_unupdate_num ;		        	// ����δ�´���ҵ��������
EXT_MAIN INT8VU idata task_curOri_goods_numH ;				// ��ǰ��ʾ��Ʒ����ʾ������λ����
EXT_MAIN INT8VU idata task_curOri_goods_numL ;				// ��ǰ��ʾ���


EXT_MAIN INT8VU idata task_unfinish_num ;				         	// δ�����������= 0,1...MAX_TASK_NUM
EXT_MAIN INT8VU idata task_unfinish_NO[MAX_TASK_NUM];		  // ��λ��
EXT_MAIN INT8VU idata task_un_goods_numH[MAX_TASK_NUM];		// ������λ
EXT_MAIN INT8VU idata task_un_goods_numL[MAX_TASK_NUM] ;	// ������λ

EXT_MAIN INT8VU idata f_repeat;								// �����ط���־��TRUE=�ѷ�������δִ�е���������ñ�־����������OK����ȡ������
///����������
EXT_MAIN INT8VU idata task_finished_num ;		  			// �������������= 0,1...MAX_TASK_NUM
EXT_MAIN INT8VU idata task_finished_NO[MAX_TASK_NUM] ;	  	// ��λ��
EXT_MAIN INT8VU idata task_ed_goods_numH[MAX_TASK_NUM] ;	// ������λ
EXT_MAIN INT8VU idata task_ed_goods_numL[MAX_TASK_NUM];		// ������λ










EXT_MAIN bit valid_ok;		            // �Ƿ���Բ���
EXT_MAIN bit f_prossout;		          // ����һ�������־���ɼ�һ���������ٴ���TRUE ������

EXT_MAIN INT8VU SystemLocked;							// ����ϵͳ������ϵͳ	 ON=����ϵͳ���������ܲ���


EXT_MAIN volatile INT8U AD_Base_OK;            // ��ѹ�������  ��־
EXT_MAIN volatile INT16U RevBase;		           // ��׼��ѹֵ
EXT_MAIN volatile INT8U sampDat ;
EXT_MAIN volatile INT8U out ;
EXT_MAIN volatile INT16U 	count;		  	       // ��10us���������650ms
//EXT_MAIN volatile INT16U 	basecount;         // ������ѹ������ʱ



#endif


