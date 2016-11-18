//IIC总线模拟实现文件

#include "extern_symbol.h"
#if(MACHINETYPE == T3N3_IB_800U)
	#include "CBM7008.H"	   

/************************************************************************
** 功能描述：循环延时　Delay1us*tim 
************************************************************************/
void delay_us(INT16U us)
{
 //  	while (--us > 1);
// 		while (--us > 10);
//	while (--us > 0);
//	INT16U	j;
	for (; us > 0; us--)
//	{	for(j=0;j<8;j++);    }
	{	_nop_();  _nop_(); _nop_();  }
}

/************************************************************************
** 功能描述：START Data Transfer
 	A high-to-low transition of SDA with SCL high is a start condition
which must precede any other command

* Not Busy Stutas: scl = 1; scl = 1;
************************************************************************/

void iic_start(void)
{
	  SETSDA();       //需在SCL之前设定
	  delay_us(5);    //延时至少4.7us
    SETSCL();       //硬件进入SDA检测状态
    delay_us(5);    //延时至少4.7us
    RESETSDA();     //SDA由1->0,产生开始信号
    delay_us(5);    //延时至少4us
    RESETSCL();     //SCL变为无效
}
/************************************************************************
** 功能描述： Stop Data Transfer
	A low-to-high transition of SDA with SCL high is a stop condition.

* Not Busy Stutas: scl = 1; scl = 1;
************************************************************************/

void iic_stop(void)
{
//    RESETSCL();
//	delay_us(5);    //至少延时4us
	RESETSDA();     //在SCL之前拉低
	delay_us(5);    //至少延时4us
    SETSCL();       //硬件进入SDA检测状态
    delay_us(5);    //至少延时4us
    SETSDA();       //SDA由0->1,产生结束信号
    delay_us(5);    //延时至少4.7us
}


/************************************************************************
** 功能描述：写IIC总线写1个字节 
** 输入参数: 8位数据dat		             
** 输出参数: 无
** 时序: SCL上升沿传送数据,数据改变必须在SCL低电平时
		 SCL 9个上升沿 其中8个为传送数据, 第9个为检测应答信号用        
** 检测应答信号,并作10ms的超时保护
** data change at scl = 0;
************************************************************************/

INT8U iic_write_byte(INT8U dat)
{
 	INT8U i;
	INT8U flg;

	for(i = 8; i > 0; i--)			// 这样写就会用djnz指令
	{				
		RESETSCL(); delay_us(2);
		
		//编译器漏洞：加括号，确保 & 运算在 == 之前
		if((dat  & 0x80) == 0x80) 
			{ SETSDA(); }
		else{ RESETSDA(); }
		dat <<= 1;delay_us(3);
		SETSCL(); delay_us(5);	
	}
	
	//检测应答信号
	//ACKNOWLEDGE：写数据时，应答信号由24LCXX发出 
	RESETSCL();delay_us(4);
	SETSDA();  delay_us(1);
	SETSCL();  delay_us(5);
	
	//While the SCL is high,then ADDRESS OR ACKNOWLEDGE VALID
	flg = FALSE;
	for(i = 200; i > 0; i--)	//这样写就会用djnz指令
	{						
		if(READSDABIT() == 0)
		{flg = TRUE; break;}			//Acknowledge Succ
		else{ delay_us(250); }			//250us
	}
	
	RESETSCL();delay_us(3);
	SETSDA();//Delay1us(2);

	return(flg);						//Acknowledge Err
}
/************************************************************************
** 功能描述：从IIC总线读取1个字节 
** 输入参数: ack = 1 则回发应答位0，否则发应答位1		             
** 输出参数: 返回8位数据
** 时序: SCL高电平输出数据,数据改变必须在SCL低电平时
		 SCL 8个下降沿       
** data change at scl = 0;;
************************************************************************/

INT8U iic_read_byte(INT8U ack)
{	
	INT8U i,dat = 0;
		
	RESETSCL();delay_us(4);
	SETSDA();  delay_us(1);		//写1，做为输入 防止钳位
	
	for(i = 8; i > 0; i--)
	{			//这样写就会用djnz指令
		SETSCL(); delay_us(2);
		(0 != READSDABIT()) ? (dat = ((dat << 1) | 0x01)) 
				   : (dat = ((dat << 1) & 0xfe));
		delay_us(3) ;
		RESETSCL(); delay_us(5);
	}
	// 输出应答信号
	if(1 == ack)
		{ RESETSDA(); delay_us(2); }
	else{   SETSDA(); delay_us(2); }
	SETSCL(); delay_us(5);
	RESETSCL(); delay_us(3);
//	SETSDA(); 
	return(dat);		
}




/************************************************************************
** 功能描述：IO配置		  
************************************************************************/
void   CBM7008IO_Config(void)
{


	pSDA = 1;//	配置开漏


	pSCL = 1 ;	  //配置开漏

}
/************************************************************************
** 功能描述：复位24C08	  
************************************************************************/
//void Ini_CBM7008(void)
//{	
//	SETSCL()  ;Delay_us(10);
//	SETSDA()  ;Delay_us(10);
//}

/************************************************************************
** 功能描述：设置CBM7008灵敏度            
** 输出参数: 返回状态 ,
灵敏度顺序ADD，SUB，FUNC，>>,<<
lmd=25 	 ,ADD 和 SUB 和FUNC 用力按有响应
************************************************************************/

void Set_CBM7008_nLMD(void)
{
	INT8U	OkLmd = para.str.CBM7008LMD;

	if( (OkLmd > MAX_CBM7008LMD)||(OkLmd < MIN_CBM7008LMD) )
	{ OkLmd = DEF_CBM7008LMD; }

	iic_start();				// 起始条件
	iic_write_byte(DEV_ADDR_W);	// 写设备地址,检测应答位
//	delay_us(5);			    // 延时

	iic_write_byte(MEM_ADDR_SET_NLMD);	 	// 写寄存器地址,检测应答位
//	delay_us(5);			   	// 延时

	iic_write_byte(48);		// 写KEY0灵敏度,检测应答位	  无按键
//	iic_write_byte(30);		// 写KEY1灵敏度,检测应答位	  无按键
//	iic_write_byte(32);		// 写KEY1灵敏度,检测应答位	  无按键
//	iic_write_byte(32);		// 写KEY1灵敏度,检测应答位	  OK键
	iic_write_byte(OkLmd);   // OK键
	iic_write_byte(48);		// 写KEY2灵敏度,检测应答位	  无按键
	iic_write_byte(48);		// 写KEY3灵敏度,检测应答位	  无按键
	iic_write_byte(48);		// 写KEY4灵敏度,检测应答位	  无按键
	iic_write_byte(48);		// 写KEY5灵敏度,检测应答位	  无按键
	iic_write_byte(48);		// 写KEY6灵敏度,检测应答位	  无按键
	iic_write_byte(48);		// 写KEY7灵敏度,检测应答位	  无按键
//	delay_us(5);				// 延时
	iic_stop();	
    // 结束条件	
}


/************************************************************************
** 功能描述：读CBM7008 ID            
** 输出参数: 返回ID号
************************************************************************/

INT16U Get_CBM7008_ID(void)
{
	INT16U temp=0;//,i;
	iic_start();				// 起始条件
	iic_write_byte(DEV_ADDR_W);	// 写设备地址(写)
//	delay_us(5);				// 延时
	iic_write_byte(MEM_ADDR_GET_ID);		// 写寄存器地址
//	delay_us(5);				// 延时
	iic_stop();					// 结束条件
	iic_start();				// 起始条件
	iic_write_byte(DEV_ADDR_R);	// 设备地址(读)
//	delay_us(5);				// 延时

	temp = iic_read_byte(1)*256; //读数据，回发应答位 		CB
	temp += iic_read_byte(0);	// 读数据，不回发应答位

	iic_stop();					// 结束条件	
	return(temp);	  	
}

/************************************************************************
** 功能描述：读CBM7008 灵敏度            
** 输出参数: 返回ID号
************************************************************************/
//INT8U lmd[8];
void Get_CBM7008_nLMD(void)
{
//	INT8U lmd[8];//,i;
	iic_start();				// 起始条件
	iic_write_byte(DEV_ADDR_W);	// 写设备地址(写)
//	delay_us(5);				// 延时
	iic_write_byte(MEM_ADDR_GET_NLMD);		// 写寄存器地址
//	delay_us(5);				// 延时
	iic_stop();					// 结束条件
	iic_start();				// 起始条件
	iic_write_byte(DEV_ADDR_R);	// 设备地址(读)
//	delay_us(5);				// 延时

	lmd[0] =  iic_read_byte(1);
	lmd[1] =  iic_read_byte(1);
	lmd[2] =  iic_read_byte(1);
	lmd[3] =  iic_read_byte(1);
	lmd[4] =  iic_read_byte(1);
	lmd[5] =  iic_read_byte(1);
	lmd[6] =  iic_read_byte(1);
	lmd[7] =  iic_read_byte(0);

	iic_stop();					// 结束条件		  	
}

/************************************************************************
** 功能描述：读CBM7008 按键状态            
** 输出参数: 返回按键信息
************************************************************************/

INT8U Get_CBM7008_KeySts(void)
{
	INT8U	chksum,keysts;
	iic_start();				// 起始条件
	iic_write_byte(DEV_ADDR_W);	// 写设备地址(写)
//	delay_us(5);				// 延时
	iic_write_byte(MEM_ADDR_GET_KEYSTS);		// 写寄存器地址
//	delay_us(5);				// 延时
	iic_stop();					// 结束条件
	iic_start();				// 起始条件
	iic_write_byte(DEV_ADDR_R);	// 设备地址(读)
//	delay_us(5);				// 延时

	keysts = iic_read_byte(1);	// 读按键状态
	chksum = iic_read_byte(0);	// 读校验和

	iic_stop();					// 结束条件	

	if( chksum != (INT8U)(keysts+MEM_ADDR_GET_KEYSTS) )
	{ keysts = 0; }
	return(keysts);	  	
}

/************************************************************************
** 功能描述：CBM7008 initlize
** 
************************************************************************/
void CBM7008_init(void)
{
	INT8U	i;
	CBM7008IO_Config();			// IO 口配置
	
	for(i = 0; i < 3; i++)
	{
		if(CBM7008ID == Get_CBM7008_ID())			// 读ID
		{ 
			Set_CBM7008_nLMD();			// 设置灵敏度
			Get_CBM7008_nLMD();			// 读灵敏度		
			return; 
		}
		DelayMs(300);						 // 延时150ms
	}
}

#endif