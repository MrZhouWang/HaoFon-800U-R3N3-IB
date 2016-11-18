#ifndef _UART_H_
#define _UART_H_

/************************************************************************************/
#ifdef  AP_UART_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_UART_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_UART_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/


#define BD_4800		0
#define BD_9600		1
#define BD_19200	2
#define BD_38400	3

#define BAUDRATE_MAX  BD_38400

//sbit p_led_static = P2^3;//系统状态灯
//sbit p_led_rxd = P2^2;
//sbit p_led_txd = P2^1;

sbit p_m485_ctrl = P1^1;



//#define UART_BUF_LEN	50//串口缓冲器大小
//
//typedef struct
//{
//	UINT8 wBuffer[UART_BUF_LEN];	//写缓冲区
//	UINT8 wLen;
//	UINT8 rIndex;					//接收缓冲区数量
//	UINT8 rBuffer[UART_BUF_LEN];
//	UINT8 rTimer;					// 计时器
//	UINT8 rFlag;			 		// 接收缓冲器是否有数=ON -> 有数, =OFF无数(在定时器中查询缓冲区状态)
//}OS_UART;
//
//AP_UART_EXT volatile OS_UART XDATA uart;

AP_UART_EXT	void UART_Init(void);
AP_UART_EXT	UINT8 SerialGetChar(void);
AP_UART_EXT	void SendSerialByte(UINT8 *Ram, UINT16 len);
AP_UART_EXT	void SendSerialByte1(UINT8 *Ram, UINT16 len);
AP_UART_EXT	UINT16 CRC16 ( UINT8 * puchMsg, UINT16 usDataLen ) ;

AP_UART_EXT	void UART1_Init(void);
#endif

