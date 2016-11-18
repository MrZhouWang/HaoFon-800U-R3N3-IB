#ifndef __RS485_H_
#define __RS485_H_


sbit M485_CTRL  = P0^2;    // 485 TXenable
#define M485_CTRL_ON	  (M485_CTRL = 1)
#define M485_CTRL_OFF   (M485_CTRL = 0)











extern void UART485_Init(void);
extern void Send485SeriaDate(INT8U *Ram, INT16U  len);
#endif

