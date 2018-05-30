#include "led.h" 
#include "delay.h"
//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{    	 
	RCC->AHB1ENR|=1<<5;//使能PORTF时钟 
	GPIO_Set(GPIOF,PIN9|PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF9,PF10设置
	LED0=1;//LED0关闭
	LED1=1;//LED1关闭
}

void DigitalTube_Init(void)
{   //与SM310361D3B的对应关系：
	//12-PE15
	//9- PE14
	//8- PE13
	
	//11-PF14
	//7- PF13
	//4- PF12
	//2- PF11
	//1- PF10
	//10-PF9
	//5- PF8
	//3- PF7
	RCC->AHB1ENR|=1<<5; //使能PORTF时钟 
	RCC->AHB1ENR|=1<<4; //使能PORTE时钟
	GPIO_Set(GPIOF,PIN7|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PF7-PF14设置
	GPIO_Set(GPIOE,PIN13|PIN14|PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); //PE13 -PE15
	PFout(10)=1;
	
}

void Led_Set(int A,int B,int C,int D,int E,int F,int G,int DP){
	
	if(A==1){//需要点亮Led A，以下类推
		PFout(14)=0;
	}else{PFout(14)=1;}
	if(B==1){//需要点亮Led B，以下类推
		PFout(13)=0;
	}else{PFout(13)=1;}
	if(C==1){//需要点亮Led C，以下类推
		PFout(12)=0;
	}else{PFout(12)=1;}
	if(D==1){//需要点亮Led D，以下类推
		PFout(11)=0;
	}else{PFout(11)=1;}
	if(E==1){//需要点亮Led E，以下类推
		PFout(10)=0;
	}else{PFout(10)=1;}
	if(F==1){//需要点亮Led F，以下类推
		PFout(9)=0;
	}else{PFout(9)=1;}
	if(G==1){//需要点亮Led G，以下类推
		PFout(8)=0;
	}else{PFout(8)=1;}
	if(DP==1){//需要点亮Led DP，以下类推
		PFout(7)=0;
	}else{PFout(7)=1;}
	
}

void SingleTube_Set(int num){

	if(num>=10){
		//显示“E”，错误
		Led_Set(1,0,0,1,1,1,1,0);//ADEFG
		return;
	}
	switch (num){
		case 0:Led_Set(1,1,1,1,1,1,0,0);break;
		case 1:Led_Set(0,1,1,0,0,0,0,0);break;
		case 2:Led_Set(1,1,0,1,1,0,1,0);break;
		case 3:Led_Set(1,1,1,1,0,0,0,0);break;
		case 4:Led_Set(0,1,1,0,0,1,1,0);break;
		case 5:Led_Set(1,0,1,1,0,1,1,0);break;//AFGCD
		case 6:Led_Set(1,0,1,1,1,1,1,0);break;//AFGCDE
		case 7:Led_Set(1,1,1,0,0,0,0,0);break;//ABC
		case 8:Led_Set(1,1,1,1,1,1,1,0);break;//ABCEDFG
		case 9:Led_Set(1,1,1,1,0,1,1,0);break;//ABCDFG
	}
}

void DigitalTube_Set(int num){//只支持整数，不支持显示小数！！！
	int low = num-num/10*10;
	int mid = (num-num/100*100)/10;
	int high = num/100;
	
	//对三位数码管进行扫描显示，扫描一次需要15ms。
	PEout(13)=1;
	PEout(14)=0;
	PEout(15)=0;
	SingleTube_Set(low);
	delay_ms(5);
	PEout(13)=0;
	PEout(14)=1;
	PEout(15)=0;
	SingleTube_Set(mid);
	delay_ms(5);
	PEout(13)=0;
	PEout(14)=0;
	PEout(15)=1;
	SingleTube_Set(high);
	delay_ms(5);
}

void Tube_delay(int time,int num){
	time = time/15;
	for(;time>0;time--){
		DigitalTube_Set(num);
	}
}

