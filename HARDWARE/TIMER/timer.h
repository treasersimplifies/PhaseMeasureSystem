#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//修改说明
//V1.2 20140504
//新增TIM5_CH1_Cap_Init函数,用于输入捕获
////////////////////////////////////////////////////////////////////////////////// 	

void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);
void Phase_Measure_Init(u32 arr,u16 psc);
#endif























