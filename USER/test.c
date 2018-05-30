#include "sys.h"
#include "delay.h" 
#include "usart.h"

#include "led.h"
#include "beep.h"
#include "key.h"
//
//蜂鸣器: 		PF6
//按键： 		PE4(PE3/PE2未使用)
//三位数码管:与SM310361D3B的对应关系：
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
//指示Led：		PE7/PE8(未使用)
int main(void)
{ 
	int BEEP_EN=0;
	int phase_diff=0;
	Stm32_Clock_Init(336,8,2,7);//设置时钟,168Mhz
	delay_init(168);		//初始化延时函数
	uart_init(84,115200);	//串口初始化为115200
	
	DigitalTube_Init();				//初始化LED时钟  
	BEEP_Init();         	//初始化蜂鸣器端口
	KEY_Init();				//初始化按键
	
	while(1)//进入程序主循环，主循环T=0.6s, f=1.67Hz,满足秒更新1-2次的要求
	{	
		//主循环不能有delay，否则会影响数码管显示。故把delay替换成了Tube_delay

		if(0 == KEY){BEEP_EN=!BEEP_EN;} //当按键按下(持续0.6s-1.2s)，支持蜂鸣器报警功能，再按一次则禁止。
		
		//此处写如何得到相位差
		
		printf("BEEP_EN = %d\n",BEEP_EN);
		printf("Phase Difference = %d\n",phase_diff);
		DigitalTube_Set(phase_diff+151);
		
		//如果开关允许报警，此处蜂鸣器警报花费0.6s.
		if(BEEP_EN==1){
			if(phase_diff<5){
			Beep_Low(phase_diff+151);
			}
			if(phase_diff>135){
			Beep_High(phase_diff+151);
			}
		}
		
	}	
}














