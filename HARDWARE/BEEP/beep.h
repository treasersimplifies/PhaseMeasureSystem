#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 


//LED�˿ڶ���
#define BEEP PFout(6)	// ����������IO  

void BEEP_Init(void);	//��ʼ��
void Beep_Low(int num);	//��λС��5�ȣ���Ƶ����
void Beep_High(int num);	//��λ����135�ȣ���Ƶ����
#endif

















