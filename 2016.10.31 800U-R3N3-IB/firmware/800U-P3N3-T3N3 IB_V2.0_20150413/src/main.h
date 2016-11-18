#ifndef _MAIN_H_
#define _MAIN_H_

/********************************************************************************************************
*                                              Global CONSTANS
********************************************************************************************************/
/***********************************************************************************
* 变量声明方法(参考uC/OS_II.H)
*	1、在 extern_symbol.h 中每个全局变量都加上 AP_CPU_EXT 的前缀
* 	2、在 main.c 中产生如下效果：  	
*      	#define OS_CPU_GLOBALS：OS_CPU_EXT 被强制为 ‘ ’(空)
*		#include "Head File/extern_symbol.h"：所有变量被定义 
*	3、在其余文件中产生如下效果：
*		未声明 OS_CPU_GLOBALS：OS_CPU_EXT 被强制为 extern
*		#include "Head File/extern_symbol.h"：所有变量被 ‘extern’ 声明、
*	4、在 main.c 中同时有 #include "Head File/parameter_map.h" 其过程描述为：
*		A、在 extern_symbol.h 中首先定义结构类型，并定义变量及分配空间)
*		B、然后在 parameter_map.h 中对变量进行初始化
* 		C、其他文件中 引用 extern_symbol.h，此时首先定义结构类型，然后再做 extern 声明
*	5、注意：在 main.c 中，必须先 #define OS_CPU_GLOBALS 然后再引用 parameter_map.h
************************************************************************************/
#ifdef  AP_CPU_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_CPU_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_CPU_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

#define	T3N3_IB_800U			0         //触摸总线型800U指示灯
#define	P3N3_IB_800U    		1         //按压总线型800U指示灯

#define	MACHINETYPE  T3N3_IB_800U          //800U-T3N3 IB
//#define	MACHINETYPE  P3N3_IB_800U       //800U—P3N3 IB

#if	 (MACHINETYPE == T3N3_IB_800U)
	#include "CBM7008.H"

#elif(MACHINETYPE == P3N3_IB_800U)
	#include "key.H"	
    
#endif

sbit p_Test        = P1^0;

#define KEY_STATUS  ( ((P3 & 0x38)>>2) +((P2& 0x10)>>4) )

#define CMD_KEY_NULL 	0
#define CMD_KEY_OK		1

//下传的命令码
#define DISPMODE_NONE	0X00		//清空所有数据，无提示,led无显示
#define DISPMODE_ASII	0X02		//字符串显示
#define DISPMODE_TASK	0X03		//作业命令


#define TASK_NONE		0X00		//非工作状态
#define TASK_RUN		0X01		//工作状态
#define TASK_FINISH		0X02		//工作完成状态



#define TIMER_DATA_CHANGE	50      //100ms
#define TIMER_CONFIG        5000    //30s  修改为 10s
#define TIMER_MODIFY        5000    //10S

#define MAX_TASK_NUM 	2	//最大的作业任务数量
#define MAX_BACK_NUM	5	//最大回显信息支持

#define DIGITAL_LED_REFRESH_DELAY  600    			// 1=1S
#define DIGITAL_LED_REFRESH_DELAY_LONG  800000    	// 1=1S

///////////////////////////////全局变量/////////////////////////////////////
//AP_CPU_EXT volatile UINT16 usCountCfg;						//无按键动作时配置计数器
//AP_CPU_EXT volatile UINT16 usCountModify;					//无按键动作时修改计数器

//AP_CPU_EXT volatile UINT16 led_static_warn_cnt;

AP_CPU_EXT INT8VU 	DATA led_flash_NO ;							// LED指示灯亮的类型，0~7，对应灭,红,绿,蓝,黄,紫,青,白
AP_CPU_EXT INT16VU 	DATA led_flash_period ;						// 闪烁周期控制
AP_CPU_EXT INT8VU 	DATA led_flash_mode ;						// 亮灭时间比例控制:0常亮
AP_CPU_EXT INT8VU 	DATA led_flash_status;						// 闪烁控制灯时，灯当前的状态,0——灭；1——亮
AP_CPU_EXT INT8VU 	DATA led_flash_NO_BK;						// 灯组控制备份数据(拉进度条后使用)

AP_CPU_EXT INT8VU 	DATA CurDispMode ;							// 对应于3种显示模式：0-无内容；1-显示字符串；3-显示分拣任务(1,3实现)
																// 00为非运行状态,01,03为运行状态

AP_CPU_EXT INT8VU 	DATA TaskStats ;							// 任务运行状态	00=非工作状态，1=工作状态，2=工作完成状态
AP_CPU_EXT INT8VU 	DATA ClearFlg ;								// 指示灯清除标志，因为按键指示优先，=ON，则说明有清除请求但未执行


AP_CPU_EXT INT8VU 	DATA beSettingAddr ;						// 0:非设置站址		1:设置站址		2：站址设定已经按下OK键
AP_CPU_EXT INT8VU 	DATA LastSoftGetOkCount;     				// 上位机软件下发此次命令之前，由0X03号命令采集到的已经OK响应的组数
AP_CPU_EXT INT8VU 	DATA task_unupdate_num ;		        	// 主机未下传作业任务总数
AP_CPU_EXT INT8VU 	DATA task_curOri_goods_numH ;				// 当前显示商品的显示数量高位备份
AP_CPU_EXT INT8VU 	DATA task_curOri_goods_numL ;				// 当前显示商品的显示数量低位备份

//未完成任务块
AP_CPU_EXT INT8VU 	DATA task_unfinish_num ;					// 未完成任务总数= 0,1...MAX_TASK_NUM
AP_CPU_EXT INT8VU 	IDATA task_unfinish_NO[MAX_TASK_NUM];		// 库位号
AP_CPU_EXT INT8VU 	IDATA task_un_goods_numH[MAX_TASK_NUM];		// 数量高位
AP_CPU_EXT INT8VU 	IDATA task_un_goods_numL[MAX_TASK_NUM] ;	// 数量低位

AP_CPU_EXT INT8VU 	DATA f_repeat;								// 任务重发标志，TRUE=已发过，但未执行的任务，清除该标志在清除命令和OK键读取命令中
///已完成任务块
AP_CPU_EXT INT8VU 	DATA  task_finished_num ;		  			// 已完成任务总数= 0,1...MAX_TASK_NUM
AP_CPU_EXT INT8VU 	IDATA task_finished_NO[MAX_TASK_NUM] ;	  	// 库位号
AP_CPU_EXT INT8VU 	IDATA task_ed_goods_numH[MAX_TASK_NUM] ;	// 数量高位
AP_CPU_EXT INT8VU 	IDATA task_ed_goods_numL[MAX_TASK_NUM];		// 数量低位

//AP_CPU_EXT INT8VU 	DATA CurBackDispIndex ;						// 当前回显索引
//AP_CPU_EXT INT8VU 	IDATA DispContentStack_NO[MAX_BACK_NUM];	// 回显信息存储的库位号
//AP_CPU_EXT INT8VU 	IDATA DispContentStack_numH[MAX_BACK_NUM];	// 回显信息存储的数量高位
//AP_CPU_EXT INT8VU 	IDATA DispContentStack_numL[MAX_BACK_NUM];	// 回显信息存储的数量低位

AP_CPU_EXT INT8VU 	DATA SystemLocked;							// 锁定系统、解锁系统	 ON=锁定系统，按键不能操作

AP_CPU_EXT INT8U IDATA lmd[8];									// CMB7008灵敏度

typedef struct
{
	UINT32 digital_led_load_cnt;		// 数码管等待拉条计数
	UINT8  beWaitNewData;				// 用以显示数码管拉条，当前是否为正在等待新数据过来
										// 判断是否完成最小拉条等待（当设备中无数据，但本次作业还会有新的作业来，数码管拉条等待，当新来数据时，也是先等待拉条）
	UINT16 led_flash_count;				// 递减计算器，系统在定时器中1MS自减1
//	UINT16 count;						// 递减计算器，系统在定时器中1MS自减1
//	UINT8  flag;						//led_Scan()==>LED扫描启动
}OS_SYS;
AP_CPU_EXT volatile OS_SYS  DATA sys;


//AP_CPU_EXT  INT16VU DATA   digital_led_loading_cnt;		   // 
AP_CPU_EXT  INT8VU 	DATA    digital_led_loading ;			   // 进度条等待标志，OFF非等待，ON等待

//AP_LED_EXT UINT8 ucCfgStatus;


// 参数存储
//AP_CPU_EXT UINT8 address[3];
//AP_CPU_EXT UINT8 toSettedAddr[3];
////AP_LED_EXT UINT8 baudrate;
//AP_CPU_EXT UINT8 ucAddr[3];               
////AP_LED_EXT UINT8 ucBaud;   
//AP_CPU_EXT UINT8 FinishedLED;					   //完成后的LED亮灯情况(0,1,2,3,4,5,6,7,8 ;灭,红,绿,蓝,黄,粉,青,白)
//AP_CPU_EXT UINT8 WtTime;						   //进度条等待时间(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
//AP_CPU_EXT UINT8 KeyPressLed;					   //触摸键按下灯颜色定义(0,1,2,3,4,5,6,7,8 ;灭,红,绿,蓝,黄,粉,青,白,原来亮的色基础上往后加1，白变为红)
//AP_CPU_EXT UINT8 DispMode;						   //显示模式定义(0,1->任务模式,指示模式)用以统一600U,900U命令统一通过批次号显示命令的方式

#define MAX_CFGPARA       	7//6      	 //配置参数最大个数
typedef union 
{
	struct{
	 
		INT8VU 	  address;				 // 地址(1~255)
		INT8VU 	  FinishedLED;			 // 完成后的LED亮灯情况(0,1,2,3,4,5,6,7-->灭,红,绿,蓝,黄,紫,青,白)
		INT8VU 	  WtTime;				 // 进度条等待时间(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
		INT8VU 	  FnDefine;				 // 功能键定义(0,1->零拣,满箱操作)
		INT8VU 	  DispMode;				 // 显示模式定义(0,1->任务模式,指示模式)用以统一600U,900U命令统一通过批次号显示命令的方式
		INT8VU 	  KeyPressLed;			 // OK键按下指示灯(0,1,2,3,4,5,6,7,8-->灭,红,绿,蓝,黄,紫,青,白,8前一种灯色+1)，（800U 触摸芯片） 
		INT8VU	  CBM7008LMD;			 // 触摸按键灵敏度
	}str;
	INT8VU	Byte[MAX_CFGPARA];
}STORAGE_UNI;



AP_CPU_EXT	STORAGE_UNI  IDATA  para;		   // 参数联合
AP_CPU_EXT	STORAGE_UNI  IDATA  para_bk;	   // 备份参数联合


// 按键变量定义
//AP_CPU_EXT  bit bdata f_KeyEnable ;
AP_CPU_EXT UINT8 DATA f_KeyEnable;

AP_CPU_EXT struct{
	struct{
//		INT8U	Enable:1;			// 按键允许
		INT8U	Short:1;			// 按键释放标志 0释放
		INT8U	Long:1;				// 按键长按标志 1长按 	
		INT8U	LongOnce:1;			// 按键长按执行一次标志
		INT8U	RVS:5;
    }b;	
	INT8U	SysCnt;					// 对系统时钟计时
	INT8U	OnCnt;					// 按下计数时间，单位为32ms
	INT8U	Backup;					// 保存上次的键值
}DATA Key;


AP_CPU_EXT UINT8 DATA	keyStatus;			//本次按下的按键
AP_CPU_EXT UINT8 DATA	keyLastDown;		//上次按下的按键
//AP_CPU_EXT UINT8 DATA	keyIsCFGLPressed;	//当前按下CFG键的时候是否有CFGL有效过
//AP_KEY_EXT UINT8 ucConfig;			//系统配置标志位=0 -> 不在配置状态，=1 ->在配置状态
//AP_CPU_EXT UINT8 DATA	ucBackStatus ;		//0：非回翻状态；1：回翻第一条；2：回翻第二条；......最大回翻10条
//AP_CPU_EXT UINT8 DATA	ucChangeValue;		//工作显示数值状态下，表示是否正在修改焦点位，以区分（修改焦点位下，高位0需要显示，否则不需要显示）
//AP_CPU_EXT UINT8 DATA	ucBackDispMode;	//是否为回显模式


// 串口
#define UART_BUF_LEN	50//串口缓冲器大小

typedef struct
{
	UINT8 wLen;						// 发送个数
	UINT8 wBuffer[UART_BUF_LEN];	// 发送缓冲区
	UINT8 rIndex;					// 接收个数
	UINT8 rBuffer[UART_BUF_LEN];	// 接收缓冲区
	UINT8 rTimer;					// 计时器
	UINT8 rFlag;			 		// 接收缓冲器是否有数=ON -> 有数, =OFF无数(在定时器中查询缓冲区状态)
}OS_UART;

AP_CPU_EXT volatile OS_UART XDATA uart;
///////////////////////////////全局变量-END-////////////////////////////////
// 函数声明
AP_CPU_EXT	void sys_data_reset();
AP_CPU_EXT	void Delay1Ms(); 
AP_CPU_EXT	void DelayMs(UINT16 usDelay);
AP_CPU_EXT  void delay_us(INT16U us);

//AP_CPU_EXT	INT8U 	iic_write_data(INT8U waddr, INT8U wdata);
//AP_CPU_EXT	INT8U 	iic_rea_Ndata(INT8U raddr);



#endif
