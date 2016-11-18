#ifndef  __UART_H_
#define  __UART_H_

#ifdef  UART_GLOBAL 
  #define EXT_UART 
#else 
  #define EXT_UART extern
#endif

#define UART_BUF_LEN	 40        //串口缓冲器大小
#define RCV_OVER_TIMER	2       //串口接收数据帧间隔  判断
typedef struct
{
	UINT8 wLen;					         	// 发送个数
	UINT8 wBuffer[UART_BUF_LEN];	// 发送缓冲区
	UINT8 rIndex;				        	// 接收个数
	UINT8 rBuffer[UART_BUF_LEN];	// 接收缓冲区
	UINT8 rTimer;					        // 计时器
	UINT8 rFlag;			 		        // 接收缓冲器是否有数=ON -> 有数, =OFF无数(在定时器中查询缓冲区状态)
}OS_UART;

EXT_UART volatile OS_UART idata uart;







extern void UART_Init();
extern void Usart1_SendByte(UINT8 byte);
#endif




