#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 


//LED端口定义
#define BEEP PFout(6)	// 蜂鸣器控制IO  

void BEEP_Init(void);	//初始化
void Beep_Low(int num);	//相位小于5度，低频报警
void Beep_High(int num);	//相位高于135度，高频报警
#endif

















