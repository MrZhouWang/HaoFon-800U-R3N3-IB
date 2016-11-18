#ifndef _dnet_h_ 
#define _dnet_h_

/************************************************************************************/
#ifdef  AP_PRO_GLOBALS				//是否声明，该源文件定义，其他模块引用		
	#define AP_PRO_EXT				//没有声明 ，则强制为空，此时为变量定义     
#else                                                                           
	#define AP_PRO_EXT  extern		//已声明，则强制为 extern，此时为变量声明   
#endif
/***********************************************************************************/

#define DISPLAY_MAX_NUM         999
//COMMAND
#define INIT_ID_COMMAND 		1
#define INIT_NETWORK_COMMAND 	2
#define TO_RUN_COMMAND		3
#define INIT_PARAM_COMMAND	0x20
#define NETWORK_IO				5
#define NETWORK_IO_STATUS		6
#define PARAM_READ_RESP		0x8e
#define PARAM_WRITE_RESP		0x90

#define ONE_CMD_MIN_LENGTH         6

#define READ_MULTI_BYTE			0x03
#define WRITE_SINGLE_BYTE		0x06
#define WRITE_MULTI_BYTE     	0x10

#define ACK_DATA_LEGTH  2


//BYTE0   命令号
//BYTE1   货位号 
//BYTE2   数量高位
//BYTE3   数量低位
//BYTE4   保留
//BYTE5   保留

//协议如下
//  0      1    2   3   4   5   6   7   8     
//命令字  长度 【2 - 7字节协议如上述】  和校验 
//        6+1     
//长度：从第一个数据字节起到最后一个字节，包括校验码。
//校验：校验码的产生方式—累加和。从命令号开始计算，到数据结束。
//AP_PRO_EXT UINT16 led_flash_period ;
//AP_PRO_EXT UINT8 led_flash_NO_BK ;
//AP_PRO_EXT UINT8 led_flash_NO ;
//AP_PRO_EXT UINT8 led_flash_mode ;
//AP_PRO_EXT UINT8 led_flash_status;


AP_PRO_EXT	void Cmd_Process(void);




#endif













