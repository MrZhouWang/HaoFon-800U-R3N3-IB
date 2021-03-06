#ifndef _MAIN_H_
#define _MAIN_H_

#include "include.h"



#ifdef MAIN_GLOBAL
#define EXT_MAIN
#else
#define EXT_MAIN extern
#endif

#define		FSLEDONTIME	 5	 	   			  // 占空比打开发射管持续时间 定时器内时间计数大于此值时开一次发射管
#define		Dval	       60	       			// 遮挡与基准值差值比较        越大设备越稳定   60相当于 0.71V
#define		Dnum	       40  	          // 遮挡次数比较    
#define   SYSTEM_INIT	 0         			// 系统状态
#define   KEY_COUNT    20000          //按键关断计时  用于按键移开时灯色平稳切换
#define   BASE_AD_COUNT  50           //记录上电开始中断前面多少个采样可以作为基准采样

#define   OK 8
#define   NO_OK 0


///***基准采样，判断条件越是苛刻越是可以判断精确************************/
//#define   First_Base  50              //第一次开始确定不在空闲
//#define   Second_Base 55              //等待十个发射脉冲之后依然在空闲
//#define   StardBase   3              //小于此数据表示在空闲           


#define MAX_TASK_NUM 	2	//最大的作业任务数量
#define MAX_BACK_NUM	5	//最大回显信息支持

#define TASK_NONE		0X00		//非工作状态
#define TASK_RUN		0X01		//工作状态
#define TASK_FINISH		0X02		//工作完成状态

#define DIGITAL_LED_REFRESH_DELAY  600    			    // 1=1S
#define DIGITAL_LED_REFRESH_DELAY_LONG  800000    	// 1=1S












EXT_MAIN INT8VU idata TaskStats ;							// 任务运行状态	00=非工作状态，1=工作状态，2=工作完成状态

EXT_MAIN INT8VU idata LastSoftGetOkCount;     				// 上位机软件下发此次命令之前，由0X03号命令采集到的已经OK响应的组数
EXT_MAIN INT8VU idata task_unupdate_num ;		        	// 主机未下传作业任务总数
EXT_MAIN INT8VU idata task_curOri_goods_numH ;				// 当前显示商品的显示数量高位备份
EXT_MAIN INT8VU idata task_curOri_goods_numL ;				// 当前显示商�


EXT_MAIN INT8VU idata task_unfinish_num ;				         	// 未完成任务总数= 0,1...MAX_TASK_NUM
EXT_MAIN INT8VU idata task_unfinish_NO[MAX_TASK_NUM];		  // 库位号
EXT_MAIN INT8VU idata task_un_goods_numH[MAX_TASK_NUM];		// 数量高位
EXT_MAIN INT8VU idata task_un_goods_numL[MAX_TASK_NUM] ;	// 数量低位

EXT_MAIN INT8VU idata f_repeat;								// 任务重发标志，TRUE=已发过，但未执行的任务，清除该标志在清除命令和OK键读取命令中
///已完成任务块
EXT_MAIN INT8VU idata task_finished_num ;		  			// 已完成任务总数= 0,1...MAX_TASK_NUM
EXT_MAIN INT8VU idata task_finished_NO[MAX_TASK_NUM] ;	  	// 库位号
EXT_MAIN INT8VU idata task_ed_goods_numH[MAX_TASK_NUM] ;	// 数量高位
EXT_MAIN INT8VU idata task_ed_goods_numL[MAX_TASK_NUM];		// 数量低位










EXT_MAIN bit valid_ok;		            // 是否可以采样
EXT_MAIN bit f_prossout;		          // 处理一次输出标志，采集一定脉冲数再处理，TRUE 待处理

EXT_MAIN INT8VU SystemLocked;							// 锁定系统、解锁系统	 ON=锁定系统，按键不能操作


EXT_MAIN volatile INT8U AD_Base_OK;            // 电压采样完成  标志
EXT_MAIN volatile INT16U RevBase;		           // 基准电压值
EXT_MAIN volatile INT8U sampDat ;
EXT_MAIN volatile INT8U out ;
EXT_MAIN volatile INT16U 	count;		  	       // 对10us计数，最大650ms
//EXT_MAIN volatile INT16U 	basecount;         // 基础电压采样计时



#endif


