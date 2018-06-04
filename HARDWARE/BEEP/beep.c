#include "beep.h" 
#include "delay.h" 
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 								  
////////////////////////////////////////////////////////////////////////////////// 	 

//初始化PF8为输出口		    
//BEEP IO初始化
void BEEP_Init(void)
{    	 
	RCC->AHB1ENR|=1<<5;    		//使能PORTF时钟 
	GPIO_Set(GPIOF,PIN6,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF6设置,下拉是GPIO_PUPD_PD，上拉是GPIO_PUPD_PU
	BEEP=1;						//关闭蜂鸣器	
}

void Beep_Low(int num)	//相位小于5度，低频报警,整个函数需要 0.4s 的时间
{	
	printf("Beep low frequency blink!");
	BEEP=0;		  
	//delay_ms(400); 
	Tube_delay(200,num);
	BEEP=1;  
	//delay_ms(400);
	Tube_delay(200,num);
}

void Beep_High(int num)	//相位大于135度，高频,4倍，报警,整个函数需要 0.6s 的时间
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

