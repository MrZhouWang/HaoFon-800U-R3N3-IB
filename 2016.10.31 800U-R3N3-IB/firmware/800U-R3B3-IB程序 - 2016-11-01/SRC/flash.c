#include "include.h"

INT16U  CODE SECTOR_PAGE[] = {             //写入第11、12、13页
    0x1600, 0x1800, 0x1A00
};
INT8U	CODE	Para_MinTB[MAX_CFGPARA] = { MIN_ADDR,MIN_FINISHEDLED,MIN_WTTIME,MIN_FNDEFINE,MIN_DISPMODE,MIN_KEYPRESSLED,MIN_CBM7008LMD };
INT8U	CODE	Para_MaxTB[MAX_CFGPARA] = { MAX_ADDR,MAX_FINISHEDLED,MAX_WTTIME,MAX_FNDEFINE,MAX_DISPMODE,MAX_KEYPRESSLED,MAX_CBM7008LMD };
INT8U	CODE	Para_DefTB[MAX_CFGPARA] = { DEF_ADDR,DEF_FINISHEDLED,DEF_WTTIME,DEF_FNDEFINE,DEF_DISPMODE,DEF_KEYPRESSLED,DEF_CBM7008LMD };


void FLASH_ByteWrite(INT16U addr, char byte)
{
	bit EA_SAVE = EA;                   // preserve EA
	char xdata *  pwrite;           // FLASH write pointer

	EA = 0;                             // disable interrupts
	//RSTSRC = 0x02;                    // enable VDD monitor as a reset source

	pwrite = (char xdata *) addr;

	PSCTL |= 0x01;                      // PSWE = 1
	FLKEY  = 0xA5;                      // Key Sequence 1
	FLKEY  = 0xF1;                      // Key Sequence 2

	//RSTSRC = 0x02;                    // enable VDD monitor as a reset source

	*pwrite = byte;                     // write the byte

	PSCTL &= ~0x01;                     // PSWE = 0

	EA = EA_SAVE;                       // restore interrupts
}


unsigned char FLASH_ByteRead (INT16U addr)
{
   bit EA_SAVE = EA;                   // preserve EA
   char code *  pread;             // FLASH read pointer
   unsigned char byte;

   EA = 0;                             // disable interrupts

   pread = (char code *) addr;

   byte = *pread;                      // read the byte

   EA = EA_SAVE;                       // restore interrupts

   return byte;
}

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : Flash 擦除页
// 输入参数 : 
// 返回参数 : 
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void FLASH_PageErase (INT16U addr)
{
	bit EA_SAVE = EA;                   // preserve EA
	char xdata * data pwrite;           // FLASH write pointer

	EA = 0;                             // disable interrupts

	//RSTSRC = 0x02;                    // enable VDD monitor as a reset source

	pwrite = (char xdata *) addr;

	PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1
	FLKEY  = 0xA5;                      // Key Sequence 1
	FLKEY  = 0xF1;                      // Key Sequence 2
	
	//RSTSRC = 0x02;                    // enable VDD monitor as a reset source
	*pwrite = 0;                        // initiate page erase

	PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0

	EA = EA_SAVE;                       // restore interrupts
} 


void FLASH_Write (INT8U * Buffer,  INT16U beginAddr, INT8U len)
{
   INT8U i;

   for (i = 0; i<len; i++) {
      FLASH_ByteWrite(beginAddr, Buffer[i]);
		  beginAddr++;
   }
}

void FLASH_Read (INT8U * Buffer, INT16U beginAddr, INT8U len)
{
   INT8U i;

    for (i = 0; i < len; i++)
    {
        Buffer[i] = FLASH_ByteRead(beginAddr);
        beginAddr++;
    }
   return ;
}

/*******************************************************************************
	 		    		save Parameter to FLASH
	input:   none
	output:  none					
*******************************************************************************/
void SaveParaToBlock(void)
{
	INT8U i;
	                                  
	ChkParaOverToDefVal();             // 检查参数是否溢出，是则恢复，并备份入备份区中
	
	
	for (i = 0; i < 3; i++)            // 写入	flash	，写3个区域
	{
	    FLASH_PageErase(SECTOR_PAGE[i]);   
		  FLASH_Write (&para.Byte[0], SECTOR_PAGE[i], MAX_CFGPARA);
	}
}
/*******************************************************************************
	 		    		check parameter value 
	input:   none
	output:  none					
*******************************************************************************/
void	ChkParaOverToDefVal(void)
{
	UINT8 i;

	for (i = 0; i < MAX_CFGPARA; i++)
	{ 
	  	// 判断是否超出范围
		if( (para.Byte[i] > Para_MaxTB[i])||(para.Byte[i] < Para_MinTB[i]) )
	  	{ para.Byte[i] = Para_DefTB[i]; }

		para_bk.Byte[i] = para.Byte[i]; 			   // 保存入备份区
	}
}
/*******************************************************************************
	 		    		Load Parameter from EEPROM
	input:   none		   
	output:  none					
*******************************************************************************/
void LoadParaFromFlash(void)
{
	
	INT8U	i;
	for(i = 0;i < MAX_CFGPARA;i++)
	{ para.Byte[i] = ReadParaFromFLASH(EE_ADDRESS+i); }     // 读出参数
	
	if( (para.str.address == 0) )
    { para.str.address = DEF_ADDR;  }                     //默认加载参数

	ChkParaOverToDefVal();				                          // 检查参数是否超出范围，超过则恢复默认值
}
/*************************************
	Read Parameter from inn EEPROM
	with verify
*************************************/
#define PARA_LEN MAX_CFGPARA
UINT8 ReadParaFromFLASH(UINT8 offset)
{
	UINT8 i = offset;
	UINT8 para_page_0[PARA_LEN];
	UINT8 para_page_1[PARA_LEN];
	UINT8 para_page_2[PARA_LEN];

	if (offset >= PARA_LEN) return 0xff;
	
  FLASH_Read (para_page_0, SECTOR_PAGE[0], PARA_LEN);
  FLASH_Read (para_page_1, SECTOR_PAGE[1], PARA_LEN);
  FLASH_Read (para_page_2, SECTOR_PAGE[2], PARA_LEN);
   
	if (para_page_0[i] == para_page_1[i])
	{
		if (para_page_1[i] != para_page_2[i])           //sector 2 error
		{
		    para_page_2[i] = para_page_0[i];            //Modify error byte
        FLASH_PageErase(SECTOR_PAGE[2]);        //Erase sector 2
                                                        //Write sector 2
	    	FLASH_Write(para_page_2, SECTOR_PAGE[2] , PARA_LEN);
		}
		return para_page_0[i];	
	}
	else if (para_page_0[i] == para_page_2[i])          //sector 1 error
	{
      para_page_1[i] = para_page_0[i];                //Modify error byte
    	FLASH_PageErase(SECTOR_PAGE[1]);            //Erase sector 1
    	                                                //Write sector 1
	    FLASH_Write(para_page_1, SECTOR_PAGE[1] , PARA_LEN);
    	return para_page_0[i];	
	}
	else                                                //Sector 0 error
	{
	    para_page_0[i] = para_page_1[i];                //Modify error byte
	    FLASH_PageErase(SECTOR_PAGE[0]);            //Erase sector 0
	    FLASH_Write(para_page_0, SECTOR_PAGE[0] , PARA_LEN);
    	return para_page_1[i];				
	}
}