#ifndef __LED_H
#define __LED_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 									  
////////////////////////////////////////////////////////////////////////////////// 	

//LED端口定义
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

void LED_Init(void);//初始化		
void DigitalTube_Init(void); //数码管初始化
void Led_Set(int A,int B,int C,int D,int E,int F,int G,int DP);
void SingleTube_Set(int num);
void DigitalTube_Set(int num);		//数码管显示函数
void Tube_delay(int time,int num);	//数码管延时函数
void Tube_scan_all(void);
void Tube_set_all(void);
void Tube_demo(void);
#endif

















