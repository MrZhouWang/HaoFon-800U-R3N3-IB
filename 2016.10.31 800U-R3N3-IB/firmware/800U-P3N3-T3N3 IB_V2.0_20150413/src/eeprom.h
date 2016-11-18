#ifndef _EEPROM_H_
#define _EEPROM_H_

/************************************************************************************/
#ifdef  AP_EPROM_GLOBALS				//�Ƿ���������Դ�ļ����壬����ģ������		
	#define AP_EPROM_EXT				//û������ ����ǿ��Ϊ�գ���ʱΪ��������     
#else                                                                           
	#define AP_EPROM_EXT  extern		//����������ǿ��Ϊ extern����ʱΪ��������   
#endif
/***********************************************************************************/

//#define CMD_TRIG_H	0x46
//#define CMD_TRIG_L  0xB9

// 	STC15W408AS

#define CMD_TRIG_H	0x5A
#define CMD_TRIG_L  0xA5


/*
sfr IAP_DATA    = 0xC2;
sfr IAP_ADDRH   = 0xC3;
sfr IAP_ADDRL   = 0xC4;
sfr IAP_CMD     = 0xC5;
sfr IAP_TRIG    = 0xC6;
sfr IAP_CONTR   = 0xC7;
*/
//����Flash �����ȴ�ʱ�估����IAP/ISP/EEPROM �����ĳ���
//#define ENABLE_ISP 0x80 //ϵͳ����ʱ��<30MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x81 //ϵͳ����ʱ��<24MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x82 //ϵͳ����ʱ��<20MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
#define ENABLE_ISP 0x83 //ϵͳ����ʱ��<12MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x84 //ϵͳ����ʱ��<6MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x85 //ϵͳ����ʱ��<3MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x86 //ϵͳ����ʱ��<2MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x87 //ϵͳ����ʱ��<1MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ

#define SECTOR_0_ADDR       0
#define SECTOR_1_ADDR       1 
#define SECTOR_2_ADDR       2
#define SECTOR_3_ADDR       3
#define SECTOR_4_ADDR       4
#define SECTOR_5_ADDR       5
#define SECTOR_6_ADDR       6
#define SECTOR_7_ADDR       7

AP_EPROM_EXT UINT16 code SECTOR_PAGE[];

#define EEPROM_BLOCK_SIZE   0x0200

AP_EPROM_EXT	UINT8 eeprom_read_byte(const UINT8 * addr);
AP_EPROM_EXT	void  eeprom_write_byte(const UINT8 * addr, UINT8 temp);
AP_EPROM_EXT	void  eeprom_read_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len);
AP_EPROM_EXT	void  eeprom_write_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len);
AP_EPROM_EXT	void eeprom_erase_sector(UINT16 pageAddr);

AP_EPROM_EXT	void eeprom_test(void);

// ������
////#define EE_ADDRESS		0
////#define EE_BAUDRATE 	1
////#define EE_CHECK		2
//#define EE_ADDRESS			0
//#define EE_FINISHEDLED 		1
//#define EE_WTTIME			2
//#define EE_KEYPRESSLED		3
//#define EE_DISPMODE			4
//
//
//#define MAX_ADDR        	255      //1~255	  ��ַ
//#define MAX_FINISHEDLED     7    	 //3  
//#define MAX_WTTIME        	5    	 //5
//#define MAX_KEYPRESSLED     8    	 //1  
//#define MAX_DISPMODE        1    	 //1
//
//#define DEF_ADDR        	1     	 //1	  Ĭ��ֵ
//#define DEF_FINISHEDLED     0    	 //0  
//#define DEF_WTTIME        	5    	 //5
//#define DEF_KEYPRESSLED    	8    	 //0  
//#define DEF_DISPMODE        0    	 //0

// ������ַ
#define EE_ADDRESS		0
#define EE_FINISHEDLED 	1
#define EE_WTTIME		2
#define EE_FNDEFINE		3
#define EE_DISPMODE		4
#define EE_KEYPRESSLED	5
#define EE_CBM7008LMD	6
// ������Сֵ
#define MIN_ADDR        	0        //0	  ��ַ
#define MIN_FINISHEDLED     0    	 //0  
#define MIN_WTTIME        	0    	 //0
#define MIN_FNDEFINE     	0    	 //0  
#define MIN_DISPMODE        0    	 //0
#define MIN_KEYPRESSLED     0    	 //0
#define MIN_CBM7008LMD      8    	 //0     ��������Сֵ��20150409�޸�
// �������ֵ
#define MAX_ADDR        	255      //1~255	  ��ַ
#define MAX_FINISHEDLED     7    	 //3  
#define MAX_WTTIME        	5    	 //5
#define MAX_FNDEFINE     	1    	 //1  
#define MAX_DISPMODE        1    	 //1
#define MAX_KEYPRESSLED     8    	 //1
#define MAX_CBM7008LMD      48    	 //1    ���������ֵ
// ����Ĭ��ֵ
#define DEF_ADDR        	1     	 //1	  Ĭ��ֵ
#define DEF_FINISHEDLED     0    	 //0  
#define DEF_WTTIME        	5    	 //5
#define DEF_FNDEFINE    	0    	 //0  
#define DEF_DISPMODE        0    	 //0
#define DEF_KEYPRESSLED     3//8    	 //0
#define DEF_CBM7008LMD      25    	 //1   Ĭ��������23��Ϊ25 20140408


// ��������
AP_EPROM_EXT	UINT8 ReadParaInEeprom(UINT8 ucAddr);
AP_EPROM_EXT	void SaveParaToBlock(void);
AP_EPROM_EXT	void LoadParaInEeprom(void);
#endif









