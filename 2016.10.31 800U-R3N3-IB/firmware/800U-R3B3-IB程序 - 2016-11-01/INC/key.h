#ifndef __KEY_H_
#define __KEY_H_

#ifdef  KEY_GLOBAL
  #define EXT_KEY
#else
  #define EXT_KEY extern
#endif

#define MAX_KEYPRESSLED     8    	 //1
#define CMD_KEY_NULL 	0
#define CMD_KEY_OK		1	
	
	
EXT_KEY  volatile UINT8 idata KeyStatus_flag;		//按键状态标志
EXT_KEY INT8VU idata digital_led_loading ;			// 进度条等待标志，OFF非等待，ON等待
EXT_KEY INT8VU idata ClearFlg ;							   	// 指示灯清除标志，因为按键指示优先，=ON，则说明有清除请求但未执行	
EXT_KEY UINT8 idata f_KeyEnable;	
	

EXT_KEY INT8VU idata beSettingAddr ;						// 0:非设置站址		1:设置站址		2：站址设定已经按下OK键

EXT_KEY UINT8 idata	keyStatus;			//本次按下的按键
EXT_KEY UINT8 idata	keyLastDown;		//上次按下的按键	

	
	
EXT_KEY struct{
	struct{
		INT8U	Short:1;			// 按键释放标志 0释放
		INT8U	Long:1;				// 按键长按标志 1长按 	
		INT8U	LongOnce:1;		// 按键长按执行一次标志
		INT8U	RVS:5;
    }b;	
  INT8U	SysCnt;					// 对系统时钟计时
	INT8U	OnCnt;					// 按下计数时间，单位为32ms
	INT8U	Backup;					// 保存上次的键值
} Key;




#define MAX_CFGPARA      7    //6      	 //配置参数最大个数
typedef union 
{
	struct{
	 
		INT8VU 	  address;				 // 地址(1~255)
		INT8VU 	  FinishedLED;		 // 完成后的LED亮灯情况(0,1,2,3,4,5,6,7-->灭,红,绿,蓝,黄,紫,青,白)
		INT8VU 	  WtTime;				   // 进度条等待时间(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
		INT8VU 	  FnDefine;				 // 功能键定义(0,1->零拣,满箱操作)
		INT8VU 	  DispMode;				 // 显示模式定义(0,1->任务模式,指示模式)用以统一600U,900U命令统一通过批次号显示命令的方式
		INT8VU 	  KeyPressLed;		 // OK键按下指示灯(0,1,2,3,4,5,6,7,8-->灭,红,绿,蓝,黄,紫,青,白,8前一种灯色+1)，（800U 触摸芯片） 
		INT8VU	  CBM7008LMD;			 // 触摸按键灵敏度
	}str;
	INT8VU	Byte[MAX_CFGPARA];
}STORAGE_UNI;



EXT_KEY	STORAGE_UNI idata  para;		   // 参数联合
EXT_KEY	STORAGE_UNI idata  para_bk;	   // 备份参数联合


extern INT8U Scan_Val();
extern void Key_Scan(void);
extern void Process_Keyboard(void);

#endif
