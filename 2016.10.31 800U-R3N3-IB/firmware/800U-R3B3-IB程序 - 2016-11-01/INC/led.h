#ifndef _LED_H_
#define _LED_H_
#include "include.h"

#ifdef  LED_GLOBAL
  #define EXT_LED
#else
#define EXT_LED extern
  #endif


sbit pEMILEDCTL  = P0^3;	//���ⷢ�����
//sbit pOUTPUT  = P0^5;		//������� 	NPN���
//sbit pINPUT   = P0^4;		//����
sbit pLEDOUTG = P0^0;			//LED״̬�����
sbit pLEDOUTB = P0^7;			//LED״̬�����
sbit pLEDOUTR = P0^6;			//LED״̬�����




/***************��˸����***************************/
#define LED_FLASH_MODE_ALWAYS		0X00
#define LED_FLASH_MODE_1V1			0X01
#define LED_FLASH_MODE_1V2			0X02
#define LED_FLASH_MODE_1V5			0X03
#define LED_FLASH_MODE_1V10			0X04
#define LED_FLASH_MODE_2V1			0X05
#define LED_FLASH_MODE_5V1			0X06
#define LED_FLASH_MODE_10V1			0X07




EXT_LED volatile INT16U ADC_BaseErrNum;    //ADC��׼��������ʱLED��˸��ʱ
EXT_LED volatile INT8U ADC_BaseLedState;   //������ʱ
#define ADC_BaseErrNumIintOFF  20000       //200ms������˸����
#define ADC_BaseErrNumIintON   20000

sbit p_signal_input   = P0^4;              //�����ź�

#define LED_NONE	0
#define LED_RED		1
#define LED_GREEN	2
#define LED_BLUE	3

#define RG_YELLOW	4
#define RB_GREEN	5
#define GB_BLUE		6
#define RGB_WHITE	7

#define LED_RG		4
#define LED_RB		5
#define LED_GB	  6
#define LED_RGB		7

#define LED_G_OFF (pLEDOUTG = 1)   
#define LED_B_OFF (pLEDOUTB = 1)
#define LED_R_OFF (pLEDOUTR = 1) 

#define LED_G_ON  (pLEDOUTG = 0)
#define LED_B_ON  (pLEDOUTB = 0)
#define LED_R_ON  (pLEDOUTR = 0)


#define OUTPUT_ON  (pOUTPUT = 1)
#define OUTPUT_OFF (pOUTPUT = 0)

#define KEY_PRESS_LED_NOIN    LED_GREEN         //������ʱ��������ʱ״̬
#define KEY_RELEASE_LED_NOIN  LED_RED	         //������ʱ�����ͷ�ʱ״̬

#define KEY_PRESS_LED_IN      LED_GREEN	       //������ʱ��������ʱ״̬
#define KEY_RELEASE_LED_IN    LED_BLUE	       //������ʱ�����ͷ�ʱ״̬

//#define EN_SIGNAL_ON_LED      LED_GREEN	     //�������ź�
//#define DISEN_SIGNAL_ON_LED   (~LED_GREEN)	 //�������ź�



EXT_LED INT8VU  led_flash_status;					// ��˸���Ƶ�ʱ���Ƶ�ǰ��״̬,0������1������
EXT_LED volatile INT8U LedStatus ;        //LED״̬
EXT_LED volatile INT8U led_flash_NO;			//���鿪��״̬
EXT_LED volatile UINT8 led_flash_mode ;   //��˸����
EXT_LED volatile UINT8 MulticastStatus;   //�鲥״ָ̬ʾ��
EXT_LED volatile INT16U LedDelayCnt;      //������ʱ


EXT_LED INT8VU   LED_MultiCast_Mode;				// LEDָʾ���鲥ģʽ
EXT_LED INT16VU  led_flash_period ;					// ��˸���ڿ���
EXT_LED INT8VU 	 led_flash_NO_BK;						// ������Ʊ�������(����������ʹ��)


typedef struct
{
	INT16U digital_led_load_cnt;		// ����ܵȴ���������
	INT8U  beWaitNewData;				// ������ʾ�������������ǰ�Ƿ�Ϊ���ڵȴ������ݹ���
									
	INT16U led_flash_count;				// �ݼ���������ϵͳ�ڶ�ʱ����1MS�Լ�1
}OS_SYS;
EXT_LED volatile OS_SYS sys;






extern INT16U GetLedOffTime();
extern INT16U GetLedOnTime();
extern void ADC_BASE_ERR_BLINK(void);
extern void ADC_BASE_ERR_ON(void);
extern void ADC_BASE_ERR_OFF(void);
extern void led_blink(void);
extern void PORT_Init (void);
extern void led_light_ctrl(INT8U leds);
extern void CMD_led_flash(void);
#endif

