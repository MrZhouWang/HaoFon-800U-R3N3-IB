#ifndef __FLASH_H_
#define __FLASH_H_


// ������ַ
#define EE_ADDRESS		0
#define EE_FINISHEDLED 	1
#define EE_WTTIME		2
#define EE_FNDEFINE		3
#define EE_DISPMODE		4
#define EE_KEYPRESSLED	5
#define EE_CBM7008LMD	6
// ������Сֵ
#define MIN_ADDR        	  0      //0	  ��ַ
#define MIN_FINISHEDLED     0    	 //0    ��ɺ��LED�������(0,1,2,3,4,5,6,7-->��,��,��,��,��,��,��,��)
#define MIN_WTTIME        	0    	 //0    �������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
#define MIN_FNDEFINE     	  0    	 //0    ���ܼ�����(0,1->���,�������)
#define MIN_DISPMODE        0    	 //0    ��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ
#define MIN_KEYPRESSLED     0    	 //0    OK������ָʾ��(0,1,2,3,4,5,6,7,8-->��,��,��,��,��,��,��,��,8ǰһ�ֵ�ɫ+1)����800U ����оƬ��
#define MIN_CBM7008LMD      8    	 //0    ��������Сֵ��20150409�޸�
// �������ֵ
#define MAX_ADDR        	255      //1~255	  ��ַ
#define MAX_FINISHEDLED     7    	 //3  
#define MAX_WTTIME        	5    	 //5
#define MAX_FNDEFINE     	  1    	 //1  
#define MAX_DISPMODE        1    	 //1
#define MAX_KEYPRESSLED     8    	 //1
#define MAX_CBM7008LMD      48     //1    ���������ֵ
// ����Ĭ��ֵ
#define DEF_ADDR        	  1      //1	  Ĭ��ֵ
#define DEF_FINISHEDLED     0    	 //0  
#define DEF_WTTIME        	5    	 //5
#define DEF_FNDEFINE    	  0    	 //0  
#define DEF_DISPMODE        0    	 //0
#define DEF_KEYPRESSLED     3      //0
#define DEF_CBM7008LMD      25     //1   Ĭ��������23��Ϊ25 20140408



extern void ChkParaOverToDefVal(void);
extern void FLASH_PageErase (INT16U addr);
extern void SaveParaToBlock(void);
extern void LoadParaFromFlash(void);
extern INT8U ReadParaFromFLASH(INT8U offset);

extern void FLASH_Read (INT8U * Buffer, INT16U beginAddr, INT8U len);
extern void FLASH_Write (INT8U * Buffer, INT16U beginAddr, INT8U len);
#endif
