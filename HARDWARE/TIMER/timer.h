#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//�޸�˵��
//V1.1 20140504
//����TIM14_PWM_Init����,����PWM���
//V1.2 20140504
//����TIM5_CH1_Cap_Init����,�������벶��
////////////////////////////////////////////////////////////////////////////////// 	

//ͨ���ı�TIM14->CCR1��ֵ���ı�ռ�ձȣ��Ӷ�����LED0������
#define LED0_PWM_VAL TIM14->CCR1    


void TIM3_Int_Init(u16 arr,u16 psc);
void TIM14_PWM_Init(u32 arr,u32 psc);
void TIM5_CH1_Cap_Init(u32 arr,u16 psc);
#endif























