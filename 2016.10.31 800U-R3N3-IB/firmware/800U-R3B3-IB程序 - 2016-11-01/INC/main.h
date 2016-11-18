#ifndef _MAIN_H_
#define _MAIN_H_

#include "include.h"



#ifdef MAIN_GLOBAL
#define EXT_MAIN
#else
#define EXT_MAIN extern
#endif

#define		FSLEDONTIME	 5	 	   			  // Õ¼¿Õ±È´ò¿ª·¢Éä¹Ü³ÖĞøÊ±¼ä ¶¨Ê±Æ÷ÄÚÊ±¼ä¼ÆÊı´óÓÚ´ËÖµÊ±¿ªÒ»´Î·¢Éä¹Ü
#define		Dval	       60	       			// ÕÚµ²Óë»ù×¼Öµ²îÖµ±È½Ï        Ô½´óÉè±¸Ô½ÎÈ¶¨   60Ïàµ±ÓÚ 0.71V
#define		Dnum	       40  	          // ÕÚµ²´ÎÊı±È½Ï    
#define   SYSTEM_INIT	 0         			// ÏµÍ³×´Ì¬
#define   KEY_COUNT    20000          //°´¼ü¹Ø¶Ï¼ÆÊ±  ÓÃÓÚ°´¼üÒÆ¿ªÊ±µÆÉ«Æ½ÎÈÇĞ»»
#define   BASE_AD_COUNT  50           //¼ÇÂ¼ÉÏµç¿ªÊ¼ÖĞ¶ÏÇ°Ãæ¶àÉÙ¸ö²ÉÑù¿ÉÒÔ×÷Îª»ù×¼²ÉÑù

#define   OK 8
#define   NO_OK 0


///***»ù×¼²ÉÑù£¬ÅĞ¶ÏÌõ¼şÔ½ÊÇ¿Á¿ÌÔ½ÊÇ¿ÉÒÔÅĞ¶Ï¾«È·************************/
//#define   First_Base  50              //µÚÒ»´Î¿ªÊ¼È·¶¨²»ÔÚ¿ÕÏĞ
//#define   Second_Base 55              //µÈ´ıÊ®¸ö·¢ÉäÂö³åÖ®ºóÒÀÈ»ÔÚ¿ÕÏĞ
//#define   StardBase   3              //Ğ¡ÓÚ´ËÊı¾İ±íÊ¾ÔÚ¿ÕÏĞ           


#define MAX_TASK_NUM 	2	//×î´óµÄ×÷ÒµÈÎÎñÊıÁ¿
#define MAX_BACK_NUM	5	//×î´ó»ØÏÔĞÅÏ¢Ö§³Ö

#define TASK_NONE		0X00		//·Ç¹¤×÷×´Ì¬
#define TASK_RUN		0X01		//¹¤×÷×´Ì¬
#define TASK_FINISH		0X02		//¹¤×÷Íê³É×´Ì¬

#define DIGITAL_LED_REFRESH_DELAY  600    			    // 1=1S
#define DIGITAL_LED_REFRESH_DELAY_LONG  800000    	// 1=1S












EXT_MAIN INT8VU idata TaskStats ;							// ÈÎÎñÔËĞĞ×´Ì¬	00=·Ç¹¤×÷×´Ì¬£¬1=¹¤×÷×´Ì¬£¬2=¹¤×÷Íê³É×´Ì¬

EXT_MAIN INT8VU idata LastSoftGetOkCount;     				// ÉÏÎ»»úÈí¼şÏÂ·¢´Ë´ÎÃüÁîÖ®Ç°£¬ÓÉ0X03ºÅÃüÁî²É¼¯µ½µÄÒÑ¾­OKÏìÓ¦µÄ×éÊı
EXT_MAIN INT8VU idata task_unupdate_num ;		        	// Ö÷»úÎ´ÏÂ´«×÷ÒµÈÎÎñ×ÜÊı
EXT_MAIN INT8VU idata task_curOri_goods_numH ;				// µ±Ç°ÏÔÊ¾ÉÌÆ·µÄÏÔÊ¾ÊıÁ¿¸ßÎ»±¸·İ
EXT_MAIN INT8VU idata task_curOri_goods_numL ;				// µ±Ç°ÏÔÊ¾ÉÌÆ


EXT_MAIN INT8VU idata task_unfinish_num ;				         	// Î´Íê³ÉÈÎÎñ×ÜÊı= 0,1...MAX_TASK_NUM
EXT_MAIN INT8VU idata task_unfinish_NO[MAX_TASK_NUM];		  // ¿âÎ»ºÅ
EXT_MAIN INT8VU idata task_un_goods_numH[MAX_TASK_NUM];		// ÊıÁ¿¸ßÎ»
EXT_MAIN INT8VU idata task_un_goods_numL[MAX_TASK_NUM] ;	// ÊıÁ¿µÍÎ»

EXT_MAIN INT8VU idata f_repeat;								// ÈÎÎñÖØ·¢±êÖ¾£¬TRUE=ÒÑ·¢¹ı£¬µ«Î´Ö´ĞĞµÄÈÎÎñ£¬Çå³ı¸Ã±êÖ¾ÔÚÇå³ıÃüÁîºÍOK¼ü¶ÁÈ¡ÃüÁîÖĞ
///ÒÑÍê³ÉÈÎÎñ¿é
EXT_MAIN INT8VU idata task_finished_num ;		  			// ÒÑÍê³ÉÈÎÎñ×ÜÊı= 0,1...MAX_TASK_NUM
EXT_MAIN INT8VU idata task_finished_NO[MAX_TASK_NUM] ;	  	// ¿âÎ»ºÅ
EXT_MAIN INT8VU idata task_ed_goods_numH[MAX_TASK_NUM] ;	// ÊıÁ¿¸ßÎ»
EXT_MAIN INT8VU idata task_ed_goods_numL[MAX_TASK_NUM];		// ÊıÁ¿µÍÎ»










EXT_MAIN bit valid_ok;		            // ÊÇ·ñ¿ÉÒÔ²ÉÑù
EXT_MAIN bit f_prossout;		          // ´¦ÀíÒ»´ÎÊä³ö±êÖ¾£¬²É¼¯Ò»¶¨Âö³åÊıÔÙ´¦Àí£¬TRUE ´ı´¦Àí

EXT_MAIN INT8VU SystemLocked;							// Ëø¶¨ÏµÍ³¡¢½âËøÏµÍ³	 ON=Ëø¶¨ÏµÍ³£¬°´¼ü²»ÄÜ²Ù×÷


EXT_MAIN volatile INT8U AD_Base_OK;            // µçÑ¹²ÉÑùÍê³É  ±êÖ¾
EXT_MAIN volatile INT16U RevBase;		           // »ù×¼µçÑ¹Öµ
EXT_MAIN volatile INT8U sampDat ;
EXT_MAIN volatile INT8U out ;
EXT_MAIN volatile INT16U 	count;		  	       // ¶Ô10us¼ÆÊı£¬×î´ó650ms
//EXT_MAIN volatile INT16U 	basecount;         // »ù´¡µçÑ¹²ÉÑù¼ÆÊ±



#endif


