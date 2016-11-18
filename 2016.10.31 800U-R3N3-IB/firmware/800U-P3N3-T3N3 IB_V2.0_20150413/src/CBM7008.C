//IIC����ģ��ʵ���ļ�

#include "extern_symbol.h"
#if(MACHINETYPE == T3N3_IB_800U)
	#include "CBM7008.H"	   

/************************************************************************
** ����������ѭ����ʱ��Delay1us*tim 
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
** ����������START Data Transfer
 	A high-to-low transition of SDA with SCL high is a start condition
which must precede any other command

* Not Busy Stutas: scl = 1; scl = 1;
************************************************************************/

void iic_start(void)
{
	  SETSDA();       //����SCL֮ǰ�趨
	  delay_us(5);    //��ʱ����4.7us
    SETSCL();       //Ӳ������SDA���״̬
    delay_us(5);    //��ʱ����4.7us
    RESETSDA();     //SDA��1->0,������ʼ�ź�
    delay_us(5);    //��ʱ����4us
    RESETSCL();     //SCL��Ϊ��Ч
}
/************************************************************************
** ���������� Stop Data Transfer
	A low-to-high transition of SDA with SCL high is a stop condition.

* Not Busy Stutas: scl = 1; scl = 1;
************************************************************************/

void iic_stop(void)
{
//    RESETSCL();
//	delay_us(5);    //������ʱ4us
	RESETSDA();     //��SCL֮ǰ����
	delay_us(5);    //������ʱ4us
    SETSCL();       //Ӳ������SDA���״̬
    delay_us(5);    //������ʱ4us
    SETSDA();       //SDA��0->1,���������ź�
    delay_us(5);    //��ʱ����4.7us
}


/************************************************************************
** ����������дIIC����д1���ֽ� 
** �������: 8λ����dat		             
** �������: ��
** ʱ��: SCL�����ش�������,���ݸı������SCL�͵�ƽʱ
		 SCL 9�������� ����8��Ϊ��������, ��9��Ϊ���Ӧ���ź���        
** ���Ӧ���ź�,����10ms�ĳ�ʱ����
** data change at scl = 0;
************************************************************************/

INT8U iic_write_byte(INT8U dat)
{
 	INT8U i;
	INT8U flg;

	for(i = 8; i > 0; i--)			// ����д�ͻ���djnzָ��
	{				
		RESETSCL(); delay_us(2);
		
		//������©���������ţ�ȷ�� & ������ == ֮ǰ
		if((dat  & 0x80) == 0x80) 
			{ SETSDA(); }
		else{ RESETSDA(); }
		dat <<= 1;delay_us(3);
		SETSCL(); delay_us(5);	
	}
	
	//���Ӧ���ź�
	//ACKNOWLEDGE��д����ʱ��Ӧ���ź���24LCXX���� 
	RESETSCL();delay_us(4);
	SETSDA();  delay_us(1);
	SETSCL();  delay_us(5);
	
	//While the SCL is high,then ADDRESS OR ACKNOWLEDGE VALID
	flg = FALSE;
	for(i = 200; i > 0; i--)	//����д�ͻ���djnzָ��
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
** ������������IIC���߶�ȡ1���ֽ� 
** �������: ack = 1 ��ط�Ӧ��λ0������Ӧ��λ1		             
** �������: ����8λ����
** ʱ��: SCL�ߵ�ƽ�������,���ݸı������SCL�͵�ƽʱ
		 SCL 8���½���       
** data change at scl = 0;;
************************************************************************/

INT8U iic_read_byte(INT8U ack)
{	
	INT8U i,dat = 0;
		
	RESETSCL();delay_us(4);
	SETSDA();  delay_us(1);		//д1����Ϊ���� ��ֹǯλ
	
	for(i = 8; i > 0; i--)
	{			//����д�ͻ���djnzָ��
		SETSCL(); delay_us(2);
		(0 != READSDABIT()) ? (dat = ((dat << 1) | 0x01)) 
				   : (dat = ((dat << 1) & 0xfe));
		delay_us(3) ;
		RESETSCL(); delay_us(5);
	}
	// ���Ӧ���ź�
	if(1 == ack)
		{ RESETSDA(); delay_us(2); }
	else{   SETSDA(); delay_us(2); }
	SETSCL(); delay_us(5);
	RESETSCL(); delay_us(3);
//	SETSDA(); 
	return(dat);		
}




/************************************************************************
** ����������IO����		  
************************************************************************/
void   CBM7008IO_Config(void)
{


	pSDA = 1;//	���ÿ�©


	pSCL = 1 ;	  //���ÿ�©

}
/************************************************************************
** ������������λ24C08	  
************************************************************************/
//void Ini_CBM7008(void)
//{	
//	SETSCL()  ;Delay_us(10);
//	SETSDA()  ;Delay_us(10);
//}

/************************************************************************
** ��������������CBM7008������            
** �������: ����״̬ ,
������˳��ADD��SUB��FUNC��>>,<<
lmd=25 	 ,ADD �� SUB ��FUNC ����������Ӧ
************************************************************************/

void Set_CBM7008_nLMD(void)
{
	INT8U	OkLmd = para.str.CBM7008LMD;

	if( (OkLmd > MAX_CBM7008LMD)||(OkLmd < MIN_CBM7008LMD) )
	{ OkLmd = DEF_CBM7008LMD; }

	iic_start();				// ��ʼ����
	iic_write_byte(DEV_ADDR_W);	// д�豸��ַ,���Ӧ��λ
//	delay_us(5);			    // ��ʱ

	iic_write_byte(MEM_ADDR_SET_NLMD);	 	// д�Ĵ�����ַ,���Ӧ��λ
//	delay_us(5);			   	// ��ʱ

	iic_write_byte(48);		// дKEY0������,���Ӧ��λ	  �ް���
//	iic_write_byte(30);		// дKEY1������,���Ӧ��λ	  �ް���
//	iic_write_byte(32);		// дKEY1������,���Ӧ��λ	  �ް���
//	iic_write_byte(32);		// дKEY1������,���Ӧ��λ	  OK��
	iic_write_byte(OkLmd);   // OK��
	iic_write_byte(48);		// дKEY2������,���Ӧ��λ	  �ް���
	iic_write_byte(48);		// дKEY3������,���Ӧ��λ	  �ް���
	iic_write_byte(48);		// дKEY4������,���Ӧ��λ	  �ް���
	iic_write_byte(48);		// дKEY5������,���Ӧ��λ	  �ް���
	iic_write_byte(48);		// дKEY6������,���Ӧ��λ	  �ް���
	iic_write_byte(48);		// дKEY7������,���Ӧ��λ	  �ް���
//	delay_us(5);				// ��ʱ
	iic_stop();	
    // ��������	
}


/************************************************************************
** ������������CBM7008 ID            
** �������: ����ID��
************************************************************************/

INT16U Get_CBM7008_ID(void)
{
	INT16U temp=0;//,i;
	iic_start();				// ��ʼ����
	iic_write_byte(DEV_ADDR_W);	// д�豸��ַ(д)
//	delay_us(5);				// ��ʱ
	iic_write_byte(MEM_ADDR_GET_ID);		// д�Ĵ�����ַ
//	delay_us(5);				// ��ʱ
	iic_stop();					// ��������
	iic_start();				// ��ʼ����
	iic_write_byte(DEV_ADDR_R);	// �豸��ַ(��)
//	delay_us(5);				// ��ʱ

	temp = iic_read_byte(1)*256; //�����ݣ��ط�Ӧ��λ 		CB
	temp += iic_read_byte(0);	// �����ݣ����ط�Ӧ��λ

	iic_stop();					// ��������	
	return(temp);	  	
}

/************************************************************************
** ������������CBM7008 ������            
** �������: ����ID��
************************************************************************/
//INT8U lmd[8];
void Get_CBM7008_nLMD(void)
{
//	INT8U lmd[8];//,i;
	iic_start();				// ��ʼ����
	iic_write_byte(DEV_ADDR_W);	// д�豸��ַ(д)
//	delay_us(5);				// ��ʱ
	iic_write_byte(MEM_ADDR_GET_NLMD);		// д�Ĵ�����ַ
//	delay_us(5);				// ��ʱ
	iic_stop();					// ��������
	iic_start();				// ��ʼ����
	iic_write_byte(DEV_ADDR_R);	// �豸��ַ(��)
//	delay_us(5);				// ��ʱ

	lmd[0] =  iic_read_byte(1);
	lmd[1] =  iic_read_byte(1);
	lmd[2] =  iic_read_byte(1);
	lmd[3] =  iic_read_byte(1);
	lmd[4] =  iic_read_byte(1);
	lmd[5] =  iic_read_byte(1);
	lmd[6] =  iic_read_byte(1);
	lmd[7] =  iic_read_byte(0);

	iic_stop();					// ��������		  	
}

/************************************************************************
** ������������CBM7008 ����״̬            
** �������: ���ذ�����Ϣ
************************************************************************/

INT8U Get_CBM7008_KeySts(void)
{
	INT8U	chksum,keysts;
	iic_start();				// ��ʼ����
	iic_write_byte(DEV_ADDR_W);	// д�豸��ַ(д)
//	delay_us(5);				// ��ʱ
	iic_write_byte(MEM_ADDR_GET_KEYSTS);		// д�Ĵ�����ַ
//	delay_us(5);				// ��ʱ
	iic_stop();					// ��������
	iic_start();				// ��ʼ����
	iic_write_byte(DEV_ADDR_R);	// �豸��ַ(��)
//	delay_us(5);				// ��ʱ

	keysts = iic_read_byte(1);	// ������״̬
	chksum = iic_read_byte(0);	// ��У���

	iic_stop();					// ��������	

	if( chksum != (INT8U)(keysts+MEM_ADDR_GET_KEYSTS) )
	{ keysts = 0; }
	return(keysts);	  	
}

/************************************************************************
** ����������CBM7008 initlize
** 
************************************************************************/
void CBM7008_init(void)
{
	INT8U	i;
	CBM7008IO_Config();			// IO ������
	
	for(i = 0; i < 3; i++)
	{
		if(CBM7008ID == Get_CBM7008_ID())			// ��ID
		{ 
			Set_CBM7008_nLMD();			// ����������
			Get_CBM7008_nLMD();			// ��������		
			return; 
		}
		DelayMs(300);						 // ��ʱ150ms
	}
}

#endif