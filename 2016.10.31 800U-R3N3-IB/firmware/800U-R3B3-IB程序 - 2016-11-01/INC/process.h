#ifndef __PROCESS_H_
#define __PROCESS_H_

#ifdef PROCESS_GLOBAL
  #define EXT_PROCESS 
#else 
  #define EXT_PROCESS extern 
#endif		




#define MULCAST_ADDR        248             //�鲥��ַ
#define DISPLAY_MAX_NUM         999
//COMMAND
#define INIT_ID_COMMAND 		1
#define INIT_NETWORK_COMMAND 	2
#define TO_RUN_COMMAND		3
#define INIT_PARAM_COMMAND	0x20
#define NETWORK_IO				5
#define NETWORK_IO_STATUS		6
#define PARAM_READ_RESP		0x8e
#define PARAM_WRITE_RESP		0x90

#define ONE_CMD_MIN_LENGTH         6

#define READ_MULTI_BYTE			0x03
#define WRITE_SINGLE_BYTE		0x06
#define WRITE_MULTI_BYTE     	0x10

#define ACK_DATA_LEGTH  2


#define F1  10001
#define F2  10002
#define F3  10003
#define F4  10004
#define F5  10005


//�´���������
#define DISPMODE_NONE	0X00		//����������ݣ�����ʾ,led����ʾ
#define DISPMODE_ASII	0X02		//�ַ�����ʾ
#define DISPMODE_TASK	0X03		//��ҵ����


// ��������ʾģʽ
#define LOAD_MODE_OFF	0		   // ����ģʽ��
#define LOAD_MODE_SHORT	1		   // ����ģʽ��ʱ��
#define LOAD_MODE_LONG	2		   // ����ģʽ��ʱ��

EXT_PROCESS INT8VU 	DATA CurDispMode ;							// ��Ӧ��3����ʾģʽ��0-�����ݣ�1-��ʾ�ַ�����3-��ʾ�ּ�����(1,3ʵ��)
																



extern void DiscodeProtocalV1(void);
extern void Cmd_Process(void);
extern void ResetAddr(void);
extern void Display_Clear(void);
extern void sys_data_reset(void);
extern void ACK_Multi_ReadTestAddr(void);
extern void ACK_Multi_Read(void);
extern void ACK_Single_Write(void);
extern void ACK_Multi_Write(void);
extern void ACK_Multi_Write_RepeatError(void);
extern void Commu_Task_Process(void);
extern void Display_Info(INT8U* buf);
#endif

