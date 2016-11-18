#ifndef _LED_H_
#define _LED_H_
#include "include.h"

#ifdef  LED_GLOBAL
  #define EXT_LED
#else
#define EXT_LED extern
  #endif


sbit pEMILEDCTL  = P0^3;	//红外发射输出
//sbit pOUTPUT  = P0^5;		//开关输出 	NPN输出
//sbit pINPUT   = P0^4;		//输入
sbit pLEDOUTG = P0^0;			//LED状态灯输出
sbit pLEDOUTB = P0^7;			//LED状态灯输出
sbit pLEDOUTR = P0^6;			//LED状态灯输出




/***************闪烁比例***************************/
#define LED_FLASH_MODE_ALWAYS		0X00
#define LED_FLASH_MODE_1V1			0X01
#define LED_FLASH_MODE_1V2			0X02
#define LED_FLASH_MODE_1V5			0X03
#define LED_FLASH_MODE_1V10			0X04
#define LED_FLASH_MODE_2V1			0X05
#define LED_FLASH_MODE_5V1			0X06
#define LED_FLASH_MODE_10V1			0X07




EXT_LED volatile INT16U ADC_BaseErrNum;    //ADC集准采样错误时LED闪烁延时
EXT_LED volatile INT8U ADC_BaseLedState;   //按键延时
#define ADC_BaseErrNumIintOFF  20000       //200ms左右闪烁周期
#define ADC_BaseErrNumIintON   20000

sbit p_signal_input   = P0^4;              //输入信号

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

#define KEY_PRESS_LED_NOIN    LED_GREEN         //无输入时按键按下时状态
#define KEY_RELEASE_LED_NOIN  LED_RED	         //无输入时按键释放时状态

#define KEY_PRESS_LED_IN      LED_GREEN	       //有输入时按键按下时状态
#define KEY_RELEASE_LED_IN    LED_BLUE	       //有输入时按键释放时状态

//#define EN_SIGNAL_ON_LED      LED_GREEN	     //有输入信号
//#define DISEN_SIGNAL_ON_LED   (~LED_GREEN)	 //无输入信号



EXT_LED INT8VU  led_flash_status;					// 闪烁控制灯时，灯当前的状态,0——灭；1——亮
EXT_LED volatile INT8U LedStatus ;        //LED状态
EXT_LED volatile INT8U led_flash_NO;			//灯组开关状态
EXT_LED volatile UINT8 led_flash_mode ;   //闪烁周期
EXT_LED volatile UINT8 MulticastStatus;   //组播状态指示灯
EXT_LED volatile INT16U LedDelayCnt;      //按键延时


EXT_LED INT8VU   LED_MultiCast_Mode;				// LED指示灯组播模式
EXT_LED INT16VU  led_flash_period ;					// 闪烁周期控制
EXT_LED INT8VU 	 led_flash_NO_BK;						// 灯组控制备份数据(拉进度条后使用)


typedef struct
{
	INT16U digital_led_load_cnt;		// 数码管等待拉条计数
	INT8U  beWaitNewData;				// 用以显示数码管拉条，当前是否为正在等待新数据过来
									
	INT16U led_flash_count;				// 递减计算器，系统在定时器中1MS自减1
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

