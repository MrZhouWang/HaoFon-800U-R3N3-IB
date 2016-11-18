
#ifndef _KEY_H_
#define _KEY_H_

/************************************************************************************/
#ifdef  AP_KEY_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_KEY_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_KEY_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/
sbit KEY_OK  	  = P1^5;

//-----------------------------------------------------------------------------------
// 常量声明
#define TIMER_COUNT_100mS	100
#define TIMER_COUNT_1S	500
#define TIMER_COUNT_3S	800
#define TIMER_COUNT_32ms	16

AP_KEY_EXT	void Key_Scan(void);
AP_KEY_EXT	void Process_Keyboard(void);

#endif
