//iic.h - 实现相关的头文件

#ifndef __CBM7008_H__
#define __CBM7008_H__
//#include "extern_symbol.h" 
// *******************************常量定义********************************** //
sbit pSDA = P3^2;
sbit pSCL = P3^3;

// 常量声明
#define TIMER_COUNT_100mS	100
#define TIMER_COUNT_1S	500
#define TIMER_COUNT_3S	800
#define TIMER_COUNT_32ms	16

//#define    CBM7008GPIO    GPIOB

#define	   SETSDA()	       (pSDA = 1)
#define	   RESETSDA()	   (pSDA = 0)  
#define	   SETSCL()	       (pSCL = 1) 
#define	   RESETSCL()	   (pSCL = 0)
#define	   READSDABIT()	    pSDA

// 设备地址0x00~0xFF,默认0x22
#define DEV_ADDR_W		((0x22<<1)&0xFE)
#define DEV_ADDR_R		((0x22<<1)|0x01)	

// 内部寄存器地址
#define MEM_ADDR_GET_ID		0x01	//芯片ID,                读，2byte 数据	 byte0:0xCB,byte1:0x72,
#define MEM_ADDR_GET_NLMD	0x0B	//获取所有按键灵敏度参数 读  Nbyte 数据
#define MEM_ADDR_SET_1LMD	0x21	//设置单个按键灵敏度参数 写	 2byte 数据
#define MEM_ADDR_SET_NLMD	0x22	//设置多个按键灵敏度参数 写	 Nbyte 数据
#define MEM_ADDR_GET_KEYSTS	0xA8 	//获取按键信息			 读  2byte 数据

																				
#define CBM7008ID	0xCB72

//======================================================
AP_CPU_EXT	void CBM7008_init(void);
AP_CPU_EXT	INT8U Get_CBM7008_KeySts(void);
AP_CPU_EXT void Set_CBM7008_nLMD(void);
AP_CPU_EXT	void Key_Scan(void);
//======================================================

#endif
