
#define AP_EPROM_GLOBALS	
#include "extern_symbol.h" 

	

typedef union union_temp16
{
    UINT16 un_temp16;
    UINT8  un_temp8[2];
}UNION_16;


UINT8 Byte_Read(UINT8 * add);              //读一字节，调用前需打开IAP 功能
void Byte_Program(UINT8 * add, UINT8 ch);  //字节编程，调用前需打开IAP 功能
void Sector_Erase(UINT16 add);            //擦除扇区
void IAP_Disable();                       //关闭IAP 功能


//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
UINT8 Byte_Read(UINT8 * add)
{
    UNION_16 my_unTemp16;
    
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM 字节读命令

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = CMD_TRIG_L;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    return (IAP_DATA);
}

//字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void Byte_Program(UINT8 * add, UINT8 ch)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM 字节编程命令

    my_unTemp16.un_temp16 = (UINT16)add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    IAP_DATA = ch;                  //要编程的数据先送进IAP_DATA 寄存器
    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = CMD_TRIG_L;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

//擦除扇区, 入口:DPTR = 扇区地址
void Sector_Erase(UINT16 add)
{
    UNION_16 my_unTemp16;
    
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM 扇区擦除命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = CMD_TRIG_H;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = CMD_TRIG_L;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

void IAP_Disable()
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}
 

/*************************************************************************
	                    Read len bytes from eeprom
**************************************************************************/

void eeprom_read_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len)
{
    UINT8 i;

    for (i = 0; i < len; i++)
    {
        Buffer[i] = Byte_Read(beginAddr);
        beginAddr++;
    }
    
    return;
}

/*************************************************************************
	                    write len bytes to eeprom
**************************************************************************/
void eeprom_write_block(UINT8 * Buffer, UINT8 * beginAddr, UINT8 len)
{
    UINT8 i;

    for (i = 0; i < len; i++)
    {
        Byte_Program(beginAddr, Buffer[i]);
        beginAddr++;
    }

    return;
}

/*************************************************************************
	                    erase a sector
**************************************************************************/

//data flash: all 4K 
//512Byte/sector, 8 sectors
//Writing data flash must erase the sector, and then write every byte;
//Each sector address as follows:
UINT16 code SECTOR_PAGE[] = {
    0x0000, 0x0200, 0x0400, 0x0600, 
    0x0800, 0x0A00, 0x0C00, 0x0E00
};

void eeprom_erase_sector(UINT16 pageAddr)
{
    Sector_Erase(pageAddr);
    
    return;
}

/*************************************
	Read Parameter from inn EEPROM
	with verify
*************************************/
//#define PARA_LEN    5
#define PARA_LEN MAX_CFGPARA
UINT8 ReadParaInEeprom(UINT8 offset)
{
	UINT8 i = offset;
	UINT8 para_page_0[PARA_LEN];
	UINT8 para_page_1[PARA_LEN];
	UINT8 para_page_2[PARA_LEN];

	if (offset >= PARA_LEN) return 0xff;
	
    eeprom_read_block(para_page_0, (UINT8 *)SECTOR_PAGE[0], PARA_LEN);
    eeprom_read_block(para_page_1, (UINT8 *)SECTOR_PAGE[1], PARA_LEN);
    eeprom_read_block(para_page_2, (UINT8 *)SECTOR_PAGE[2], PARA_LEN);
   
	if (para_page_0[i] == para_page_1[i])
	{
		if (para_page_1[i] != para_page_2[i])           //sector 2 error
		{
		    para_page_2[i] = para_page_0[i];            //Modify error byte
            eeprom_erase_sector(SECTOR_PAGE[2]);        //Erase sector 2
                                                        //Write sector 2
	    	eeprom_write_block(para_page_2, (UINT8 *)SECTOR_PAGE[2] , PARA_LEN);
		}
		return para_page_0[i];	
	}
	else if (para_page_0[i] == para_page_2[i])          //sector 1 error
	{
        para_page_1[i] = para_page_0[i];                //Modify error byte
    	eeprom_erase_sector(SECTOR_PAGE[1]);            //Erase sector 1
    	                                                //Write sector 1
	    eeprom_write_block(para_page_1, (UINT8 *)SECTOR_PAGE[1] , PARA_LEN);
    	return para_page_0[i];	
	}
	else                                                //Sector 0 error
	{
	    para_page_0[i] = para_page_1[i];                //Modify error byte
	    eeprom_erase_sector(SECTOR_PAGE[0]);            //Erase sector 0
	    eeprom_write_block(para_page_0, (UINT8 *)SECTOR_PAGE[0] , PARA_LEN);
    	return para_page_1[i];				
	}
}

/*******************************************************************************
	 		   检查参数是否超出范围，超出则恢复默认值,并存入备份区
	input:   none
	output:  none					
*******************************************************************************/
//												1            2            3            4			 5			   6	  7
INT8U	CODE	Para_MinTB[MAX_CFGPARA] = { MIN_ADDR,MIN_FINISHEDLED,MIN_WTTIME,MIN_FNDEFINE,MIN_DISPMODE,MIN_KEYPRESSLED,MIN_CBM7008LMD };
INT8U	CODE	Para_MaxTB[MAX_CFGPARA] = { MAX_ADDR,MAX_FINISHEDLED,MAX_WTTIME,MAX_FNDEFINE,MAX_DISPMODE,MAX_KEYPRESSLED,MAX_CBM7008LMD };
INT8U	CODE	Para_DefTB[MAX_CFGPARA] = { DEF_ADDR,DEF_FINISHEDLED,DEF_WTTIME,DEF_FNDEFINE,DEF_DISPMODE,DEF_KEYPRESSLED,DEF_CBM7008LMD };

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
	 		    		save Parameter to EEPROM
	input:   none
	output:  none					
*******************************************************************************/
void SaveParaToBlock(void)
{
	INT8U i;
	// 检查参数是否溢出，是则恢复，并备份入备份区中
	ChkParaOverToDefVal();
	
	// 写入	eeprom	，写3个区域
	for (i = 0; i < 3; i++)
	{
	    eeprom_erase_sector(SECTOR_PAGE[i]);   
		eeprom_write_block (&para.Byte[0], (UINT8 *)SECTOR_PAGE[i], MAX_CFGPARA);
	}
}
/*******************************************************************************
	 		    		Load Parameter from EEPROM
	input:   none		   
	output:  none					
*******************************************************************************/
void LoadParaInEeprom(void)
{
	// 读出参数
	INT8U	i;
	for(i = 0;i < MAX_CFGPARA;i++)
	{ para.Byte[i] = ReadParaInEeprom(EE_ADDRESS+i); }

	// 检查参数是否溢出，是则恢复，并备份入备份区中
	if( (para.str.address == 0) )//||(addr == 0) )  
    { para.str.address = DEF_ADDR;  }     //默认加载参数

	ChkParaOverToDefVal();				  // 检查参数是否超出范围，超过则恢复默认值
}
/*******************************************************************************
								END					
*******************************************************************************/
