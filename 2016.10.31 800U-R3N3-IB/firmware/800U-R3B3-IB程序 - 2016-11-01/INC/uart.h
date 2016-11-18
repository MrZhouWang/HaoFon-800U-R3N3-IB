#ifndef  __UART_H_
#define  __UART_H_

#ifdef  UART_GLOBAL 
  #define EXT_UART 
#else 
  #define EXT_UART extern
#endif

#define UART_BUF_LEN	 40        //���ڻ�������С
#define RCV_OVER_TIMER	2       //���ڽ�������֡���  �ж�
typedef struct
{
	UINT8 wLen;					         	// ���͸���
	UINT8 wBuffer[UART_BUF_LEN];	// ���ͻ�����
	UINT8 rIndex;				        	// ���ո���
	UINT8 rBuffer[UART_BUF_LEN];	// ���ջ�����
	UINT8 rTimer;					        // ��ʱ��
	UINT8 rFlag;			 		        // ���ջ������Ƿ�����=ON -> ����, =OFF����(�ڶ�ʱ���в�ѯ������״̬)
}OS_UART;

EXT_UART volatile OS_UART idata uart;







extern void UART_Init();
extern void Usart1_SendByte(UINT8 byte);
#endif




