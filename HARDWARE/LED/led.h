#ifndef __LED_H
#define __LED_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 									  
////////////////////////////////////////////////////////////////////////////////// 	

//LED�˿ڶ���
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

void LED_Init(void);//��ʼ��		
void DigitalTube_Init(void); //����ܳ�ʼ��
void Led_Set(int A,int B,int C,int D,int E,int F,int G,int DP);
void SingleTube_Set(int num);
void DigitalTube_Set(int num);		//�������ʾ����
void Tube_delay(int time,int num);	//�������ʱ����
void Tube_scan_all(void);
void Tube_set_all(void);
void Tube_demo(void);
#endif

















