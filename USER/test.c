#include "sys.h"
#include "delay.h" 
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "beep.h"
#include "key.h"
//
//蜂鸣器: 		PF6
//按键： 		PE4、PE3(PE2未使用)
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
	int i =0;
	int j =0;
	int Beep_En = 0;
	int Fre_or_Phase = 0;
	int phase_diff=0;
	int frequency = 0;
	Stm32_Clock_Init(336,8,2,7); //设置时钟,168Mhz
	delay_init(168);		//初始化延时函数
	uart_init(84,115200);	//串口初始化为115200
	
	DigitalTube_Init();				//初始化LED时钟  
	BEEP_Init();         	//初始化蜂鸣器端口
	KEY_Init();				//初始化按键
	
	while(1)//进入程序主循环，主循环T=0.6s, f=1.67Hz,满足秒更新1-2次的要求
	{	
		//主循环不能有delay，否则会影响数码管显示。故把delay替换成了Tube_delay
		
		if(0 == KEY_BEEP){Beep_En = 1;}
		else{Beep_En = 0;}
		if(0 == KEY_FP){Fre_or_Phase = 1;}
		else{Fre_or_Phase = 0;}
		
		printf("Bepp_En = %d",Beep_En);
		Tube_delay(16,phase_diff);		//应该在每一句需要花费STM32芯片时间的语句后面加一个Tube_delay，防止闪烁，最小单位：15，保险起见16。
		printf("   Fre_or_Phase = %d",Fre_or_Phase);
		Tube_delay(16,phase_diff);
		printf("   Phase Difference = %d",phase_diff);
		Tube_delay(16,phase_diff);
		printf("   Frequency = %d",frequency);
		Tube_delay(16,phase_diff);
		
		if(Fre_or_Phase==0){//选择显示相位测量
			//DigitalTube_Set(012);//
			//Tube_set_all();
			//Tube_demo();
			//Tube_delay(1000,phase_diff);
			//如果开关允许报警，此处蜂鸣器警报花费0.6s.
			
			if(Beep_En==1){		//当Fre_or_Phase=0，BEEP_EN=1时，才会有蜂鸣器报警
				if(phase_diff<5){
				Beep_Low(phase_diff);
				}
				if(phase_diff>135){
				Beep_High(phase_diff);
				}
			}
			else{
				Tube_delay(900,phase_diff);//delay时间尽量能被15整除？
			}
		}
		else{//Fre_or_Phase==1 选择显示频率测量
			Tube_delay(1000,frequency+500);
		}
		
	}	
}














