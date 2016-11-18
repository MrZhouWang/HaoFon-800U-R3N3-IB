#ifndef __ADC_H_
#define __ADC_H_


#define 	TRUE		1		                        //标志的真
#define 	FALSE		0		                        //标志的假

#define 	ADC_BASE_CARITICAL_LOW    60			  //定义基准电压值低临界值   低界点 0.8v
#define 	ADC_BASE_CARITICAL_HIGH   115			  //定义基准电压值低临界值   高界点 1.5V


#define 	ON			1		                        //IO口控制的开关管,低电平开
#define 	OFF			0		                        //IO口控制的开关管,高电平关




extern void ADC_Init (void);
extern INT8U ADC_Samp (void);
extern void Base_ADC();
#endif


