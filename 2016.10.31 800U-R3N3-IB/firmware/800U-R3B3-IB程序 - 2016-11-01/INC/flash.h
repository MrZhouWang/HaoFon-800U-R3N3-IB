#ifndef __FLASH_H_
#define __FLASH_H_


// 参数地址
#define EE_ADDRESS		0
#define EE_FINISHEDLED 	1
#define EE_WTTIME		2
#define EE_FNDEFINE		3
#define EE_DISPMODE		4
#define EE_KEYPRESSLED	5
#define EE_CBM7008LMD	6
// 参数最小值
#define MIN_ADDR        	  0      //0	  地址
#define MIN_FINISHEDLED     0    	 //0    完成后的LED亮灯情况(0,1,2,3,4,5,6,7-->灭,红,绿,蓝,黄,紫,青,白)
#define MIN_WTTIME        	0    	 //0    进度条等待时间(0,1,2,3,4,5-->100ms,200ms,300ms,400ms,500ms,600ms)
#define MIN_FNDEFINE     	  0    	 //0    功能键定义(0,1->零拣,满箱操作)
#define MIN_DISPMODE        0    	 //0    显示模式定义(0,1->任务模式,指示模式)用以统一600U,900U命令统一通过批次号显示命令的方式
#define MIN_KEYPRESSLED     0    	 //0    OK键按下指示灯(0,1,2,3,4,5,6,7,8-->灭,红,绿,蓝,黄,紫,青,白,8前一种灯色+1)，（800U 触摸芯片）
#define MIN_CBM7008LMD      8    	 //0    灵敏度最小值，20150409修改
// 参数最大值
#define MAX_ADDR        	255      //1~255	  地址
#define MAX_FINISHEDLED     7    	 //3  
#define MAX_WTTIME        	5    	 //5
#define MAX_FNDEFINE     	  1    	 //1  
#define MAX_DISPMODE        1    	 //1
#define MAX_KEYPRESSLED     8    	 //1
#define MAX_CBM7008LMD      48     //1    灵敏度最大值
// 参数默认值
#define DEF_ADDR        	  1      //1	  默认值
#define DEF_FINISHEDLED     0    	 //0  
#define DEF_WTTIME        	5    	 //5
#define DEF_FNDEFINE    	  0    	 //0  
#define DEF_DISPMODE        0    	 //0
#define DEF_KEYPRESSLED     3      //0
#define DEF_CBM7008LMD      25     //1   默认灵敏度23改为25 20140408



extern void ChkParaOverToDefVal(void);
extern void FLASH_PageErase (INT16U addr);
extern void SaveParaToBlock(void);
extern void LoadParaFromFlash(void);
extern INT8U ReadParaFromFLASH(INT8U offset);

extern void FLASH_Read (INT8U * Buffer, INT16U beginAddr, INT8U len);
extern void FLASH_Write (INT8U * Buffer, INT16U beginAddr, INT8U len);
#endif
