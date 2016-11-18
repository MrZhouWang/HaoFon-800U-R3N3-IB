#ifndef _UART_H_
#define _UART_H_

/************************************************************************************/
#ifdef  AP_UART_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_UART_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_UART_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/


#define BD_4800		0
#define BD_9600		1
#define BD_19200	2
#define BD_38400	3

#define BAUDRATE_MAX  BD_38400

//sbit p_led_static = P2^3;//ϵͳ״̬��
//sbit p_led_rxd = P2^2;
//sbit p_led_txd = P2^1;

sbit p_m485_ctrl = P1^1;



//#define UART_BUF_LEN	50//���ڻ�������С
//
//typedef struct
//{
//	UINT8 wBuffer[UART_BUF_LEN];	//д������
//	UINT8 wLen;
//	UINT8 rIndex;					//���ջ���������
//	UINT8 rBuffer[UART_BUF_LEN];
//	UINT8 rTimer;					// ��ʱ��
//	UINT8 rFlag;			 		// ���ջ������Ƿ�����=ON -> ����, =OFF����(�ڶ�ʱ���в�ѯ������״̬)
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

