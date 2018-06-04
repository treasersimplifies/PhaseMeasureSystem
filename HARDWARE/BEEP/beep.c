#include "beep.h" 
#include "delay.h" 
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 								  
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ��PF8Ϊ�����		    
//BEEP IO��ʼ��
void BEEP_Init(void)
{    	 
	RCC->AHB1ENR|=1<<5;    		//ʹ��PORTFʱ�� 
	GPIO_Set(GPIOF,PIN6,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF6����,������GPIO_PUPD_PD��������GPIO_PUPD_PU
	BEEP=1;						//�رշ�����	
}

void Beep_Low(int num)	//��λС��5�ȣ���Ƶ����,����������Ҫ 0.4s ��ʱ��
{	
	printf("Beep low frequency blink!");
	BEEP=0;		  
	//delay_ms(400); 
	Tube_delay(200,num);
	BEEP=1;  
	//delay_ms(400);
	Tube_delay(200,num);
}

void Beep_High(int num)	//��λ����135�ȣ���Ƶ,4��������,����������Ҫ 0.6s ��ʱ��
{	
	printf("Beep high frequency blink!");
	BEEP=0;  
	Tube_delay(100,num);
	BEEP=1;		  
	Tube_delay(100,num);
	BEEP=0;  
	Tube_delay(100,num);
	BEEP=1;		  
	Tube_delay(100,num);  
}

