#ifndef _LED_H_
#define _LED_H_

/************************************************************************************/
#ifdef  AP_LED_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_LED_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_LED_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

sbit p_led_red        = P1^4;
sbit p_led_green      = P1^3;
sbit p_led_blue       = P1^2;

//led light 引脚配置
#define LED_NONE	0				// 不选中LED灯
#define LED_RED		1				// 选中RED灯
#define LED_GREEN	2				// 选中GREEN灯
#define LED_BLUE	3				// 选中BLUE灯

#define LED_RG		4				// 选中RED & GREEN灯
#define LED_RB		5				// 选中RED & BLUE灯
#define LED_GB		6				// 选中GREEN & BLUE灯
#define LED_RGB		7				// 选中RED & GREEN & BLUE灯

//led 灯闪烁比例模式
//9种亮灭时间比例
#define LED_FLASH_MODE_ALWAYS		0		   //常亮
#define LED_FLASH_MODE_1V1			1		   //亮1*100ms， 灭1*100ms；
#define LED_FLASH_MODE_1V2			2		   //亮1*100ms， 灭2*100ms；
#define LED_FLASH_MODE_1V5			3		   //亮1*100ms， 灭5*100ms；
#define LED_FLASH_MODE_1V10			4		   //亮1*100ms， 灭10*100ms；
#define LED_FLASH_MODE_2V1			5		   //亮2*100ms， 灭1*100ms；
#define LED_FLASH_MODE_5V1			6		   //亮5*100ms， 灭1*100ms；
#define LED_FLASH_MODE_10V1			7		   //亮10*100ms，灭1*100ms；


#define LED_FLASH_MAX_MODE9	    LED_FLASH_MODE9

#define LED_FLASH_MODE10	10
#define LED_FLASH_MODE11	11
#define LED_FLASH_MODE12	12
#define LED_FLASH_MODE13	13

#define LED_FLASH_TIME_MAX       4

//数码管显示从左到右分别为第1位-->第6位
//#define DIGITAL_LED1	0 //第1位数码管
//#define DIGITAL_LED2	1 //第2位数码管
//#define DIGITAL_LED3	2 //第3位数码管

	
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
//	UINT8     show;   //数码管是否显示，"ON"==>显示,"OFF"关闭
//}OS_NUM;


////AP_LED_EXT UINT8 ucCfgStatus;
//AP_LED_EXT UINT8 address[3];
//AP_LED_EXT UINT8 toSettedAddr[3];
////AP_LED_EXT UINT8 baudrate;
//AP_LED_EXT UINT8 ucAddr[3];               
////AP_LED_EXT UINT8 ucBaud;   
//AP_LED_EXT UINT8 FinishedLED;					   //完成后的LED亮灯情况(0,1,2,3,4,5,6,7,8 ;灭,红,绿,蓝,黄,粉,青,白)
//AP_LED_EXT UINT8 WtTime;						   //进度条等待时间(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
//AP_LED_EXT UINT8 KeyPressLed;					   //触摸键按下灯颜色定义(0,1,2,3,4,5,6,7,8 ;灭,红,绿,蓝,黄,粉,青,白,原来亮的色基础上往后加1，白变为红)
//AP_LED_EXT UINT8 DispMode;						   //显示模式定义(0,1->任务模式,指示模式)用以统一600U,900U命令统一通过批次号显示命令的方式
              
//AP_LED_EXT UINT8 ucFocus;
//AP_LED_EXT UINT8 ucShift;
//AP_LED_EXT volatile OS_NUM idata  led[LED_NUMBER];
//AP_LED_EXT volatile OS_NUM idata bled[LED_NUMBER];

//AP_LED_EXT UINT8 LED_TBL[LED_TBL_MAX] ; 	        //数码管显示对应表


AP_LED_EXT	void led_light_ctrl(UINT8 ucValue);
AP_LED_EXT	void Display_Clear(void);
AP_LED_EXT	void CMD_led_flash(void);
#endif

