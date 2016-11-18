#ifndef _dnet_h_ 
#define _dnet_h_

/************************************************************************************/
#ifdef  AP_PRO_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_PRO_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_PRO_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
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


//BYTE0   �����
//BYTE1   ��λ�� 
//BYTE2   ������λ
//BYTE3   ������λ
//BYTE4   ����
//BYTE5   ����

//Э������
//  0      1    2   3   4   5   6   7   8     
//������  ���� ��2 - 7�ֽ�Э����������  ��У�� 
//        6+1     
//���ȣ��ӵ�һ�������ֽ������һ���ֽڣ�����У���롣
//У�飺У����Ĳ�����ʽ���ۼӺ͡�������ſ�ʼ���㣬�����ݽ�����
//AP_PRO_EXT UINT16 led_flash_period ;
//AP_PRO_EXT UINT8 led_flash_NO_BK ;
//AP_PRO_EXT UINT8 led_flash_NO ;
//AP_PRO_EXT UINT8 led_flash_mode ;
//AP_PRO_EXT UINT8 led_flash_status;


AP_PRO_EXT	void Cmd_Process(void);




#endif













