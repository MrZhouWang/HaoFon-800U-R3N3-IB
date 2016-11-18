#ifndef _MYTYPE_H_
#define _MYTYPE_H_

/********************************************************************************************************
*                                      DATA TYPES
*                                    (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  		BOOLEAN;
typedef unsigned char  		INT8U;              /* Unsigned  8 bit quantity           */
typedef signed   char  		INT8S;              /* Signed    8 bit quantity           */
typedef unsigned int		INT16U;             /* Unsigned 16 bit quantity           */
typedef signed   int 		INT16S;          	/* Signed   16 bit quantity           */
typedef unsigned long int   INT32U;          	/* Unsigned 32 bit quantity           */
typedef signed   long int   INT32S;         	/* Signed   32 bit quantity           */
typedef float          		FP32;               /* Single precision floating point    */
typedef double         		FP64;               /* Double precision floating point    */

typedef INT16U   OS_STK;                  		/* Each stack entry is 32-bit wide                    */
typedef INT16U   OS_CPU_SR;                		/* Define size of CPU status register (PSR = 32 bits) */

typedef unsigned char  		UINT8;              /* Unsigned  8 bit quantity           */
typedef signed   char  		SINT8;              /* Signed    8 bit quantity           */
typedef unsigned int		UINT16;             /* Unsigned 16 bit quantity           */
typedef signed   int 		SINT16;          	/* Signed   16 bit quantity           */
typedef unsigned long int   UINT32;          	/* Unsigned 32 bit quantity           */
typedef signed   long int   SINT32;         	/* Signed   32 bit quantity           */

typedef volatile unsigned char  INT8VU;
typedef volatile unsigned short int	INT16VU;
typedef volatile unsigned long  int INT32VU;

#define DATA	data
#define IDATA	idata
#define XDATA	xdata		  	
	
#define CODE	const code
//=============================================================================================================


#define FAIL 		0
#define OK   		1

#define FALSE       0
#define TRUE        1


#define NO          0
#define YES         1

#define ON			1
#define OFF			0

#define HIGH(x) ((x &0xff00)>>8)
#define LOW(x)  (x &0xff)
#define _MIN(a,b)  (a)<(b) ? a :b
#define _MAX(a,b)  (a)>(b) ? a :b

#define NOP	    (_nop_())



#endif
