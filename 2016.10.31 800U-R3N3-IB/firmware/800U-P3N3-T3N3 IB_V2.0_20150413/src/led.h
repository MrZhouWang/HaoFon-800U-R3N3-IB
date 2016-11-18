#ifndef _LED_H_
#define _LED_H_

/************************************************************************************/
#ifdef  AP_LED_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_LED_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_LED_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/

sbit p_led_red        = P1^4;
sbit p_led_green      = P1^3;
sbit p_led_blue       = P1^2;

//led light ��������
#define LED_NONE	0				// ��ѡ��LED��
#define LED_RED		1				// ѡ��RED��
#define LED_GREEN	2				// ѡ��GREEN��
#define LED_BLUE	3				// ѡ��BLUE��

#define LED_RG		4				// ѡ��RED & GREEN��
#define LED_RB		5				// ѡ��RED & BLUE��
#define LED_GB		6				// ѡ��GREEN & BLUE��
#define LED_RGB		7				// ѡ��RED & GREEN & BLUE��

//led ����˸����ģʽ
//9������ʱ�����
#define LED_FLASH_MODE_ALWAYS		0		   //����
#define LED_FLASH_MODE_1V1			1		   //��1*100ms�� ��1*100ms��
#define LED_FLASH_MODE_1V2			2		   //��1*100ms�� ��2*100ms��
#define LED_FLASH_MODE_1V5			3		   //��1*100ms�� ��5*100ms��
#define LED_FLASH_MODE_1V10			4		   //��1*100ms�� ��10*100ms��
#define LED_FLASH_MODE_2V1			5		   //��2*100ms�� ��1*100ms��
#define LED_FLASH_MODE_5V1			6		   //��5*100ms�� ��1*100ms��
#define LED_FLASH_MODE_10V1			7		   //��10*100ms����1*100ms��


#define LED_FLASH_MAX_MODE9	    LED_FLASH_MODE9

#define LED_FLASH_MODE10	10
#define LED_FLASH_MODE11	11
#define LED_FLASH_MODE12	12
#define LED_FLASH_MODE13	13

#define LED_FLASH_TIME_MAX       4

//�������ʾ�����ҷֱ�Ϊ��1λ-->��6λ
//#define DIGITAL_LED1	0 //��1λ�����
//#define DIGITAL_LED2	1 //��2λ�����
//#define DIGITAL_LED3	2 //��3λ�����

	
#define LED_R_ON	(p_led_red = 0)
#define LED_R_OFF	(p_led_red = 1)

#define LED_G_ON	(p_led_green = 0)
#define LED_G_OFF	(p_led_green = 1)

#define LED_B_ON	(p_led_blue = 0)
#define LED_B_OFF	(p_led_blue = 1)

//#define COUNT_FLICKER_ON	    500
//#define COUNT_FLICKER_OFF		200

//#define LED_LOAD_CNT_PER		2000

//#define LED_NUMBER		3
//#define LED_NOT_DISPLAY 0xff
//#define LED_TBL_MAX     17

//typedef struct 
//{
//	UINT8 	  num;
//	UINT16	  delay;
//	UINT8     show;   //������Ƿ���ʾ��"ON"==>��ʾ,"OFF"�ر�
//}OS_NUM;


////AP_LED_EXT UINT8 ucCfgStatus;
//AP_LED_EXT UINT8 address[3];
//AP_LED_EXT UINT8 toSettedAddr[3];
////AP_LED_EXT UINT8 baudrate;
//AP_LED_EXT UINT8 ucAddr[3];               
////AP_LED_EXT UINT8 ucBaud;   
//AP_LED_EXT UINT8 FinishedLED;					   //��ɺ��LED�������(0,1,2,3,4,5,6,7,8 ;��,��,��,��,��,��,��,��)
//AP_LED_EXT UINT8 WtTime;						   //�������ȴ�ʱ��(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
//AP_LED_EXT UINT8 KeyPressLed;					   //���������µ���ɫ����(0,1,2,3,4,5,6,7,8 ;��,��,��,��,��,��,��,��,ԭ������ɫ�����������1���ױ�Ϊ��)
//AP_LED_EXT UINT8 DispMode;						   //��ʾģʽ����(0,1->����ģʽ,ָʾģʽ)����ͳһ600U,900U����ͳһͨ�����κ���ʾ����ķ�ʽ
              
//AP_LED_EXT UINT8 ucFocus;
//AP_LED_EXT UINT8 ucShift;
//AP_LED_EXT volatile OS_NUM idata  led[LED_NUMBER];
//AP_LED_EXT volatile OS_NUM idata bled[LED_NUMBER];

//AP_LED_EXT UINT8 LED_TBL[LED_TBL_MAX] ; 	        //�������ʾ��Ӧ��


AP_LED_EXT	void led_light_ctrl(UINT8 ucValue);
AP_LED_EXT	void Display_Clear(void);
AP_LED_EXT	void CMD_led_flash(void);
#endif

