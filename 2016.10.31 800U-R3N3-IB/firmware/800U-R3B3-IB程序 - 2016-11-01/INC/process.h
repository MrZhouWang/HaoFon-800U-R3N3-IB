#ifndef __PROCESS_H_
#define __PROCESS_H_

#ifdef PROCESS_GLOBAL
  #define EXT_PROCESS 
#else 
  #define EXT_PROCESS extern 
#endif		




#define MULCAST_ADDR        248             //组播地址
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


//下传的命令码
#define DISPMODE_NONE	0X00		//清空所有数据，无提示,led无显示
#define DISPMODE_ASII	0X02		//字符串显示
#define DISPMODE_TASK	0X03		//作业命令


// 进度条显示模式
#define LOAD_MODE_OFF	0		   // 进度模式关
#define LOAD_MODE_SHORT	1		   // 进度模式短时间
#define LOAD_MODE_LONG	2		   // 进度模式长时间

EXT_PROCESS INT8VU 	DATA CurDispMode ;							// 对应于3种显示模式：0-无内容；1-显示字符串；3-显示分拣任务(1,3实现)
																



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

