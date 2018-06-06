#include "sys.h"
#include "delay.h" 
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "beep.h"
#include "key.h"
//引脚使用分配：
//输入捕获：
//PA0（TIM5_CH1）	先出现上升沿
//PA5（TIM2_CH1）	后出现上升沿
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


extern u8  	TIM2CH1_CAPTURE_STA;	//输入捕获状态		    				
extern u32	TIM2CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)

extern u8  	TIM5CH1_CAPTURE_STA;	//输入捕获状态		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)

extern u32 	TIM2TIM5_DIFF_VAL;

int main(void)
{ 
	int Beep_En = 0;
	int Fre_or_Phase = 0;
	long long T = 0;
	long long DIFF = 0;
	long long phase_diff=0;
	long long frequency = 0;
	
	Stm32_Clock_Init(336,8,2,7);		//设置时钟,168Mhz
	delay_init(168);					//初始化延时函数
	uart_init(84,115200);				//串口初始化为115200
	
	DigitalTube_Init();					//初始化LED时钟  
	BEEP_Init();         				//初始化蜂鸣器端口
	KEY_Init();							//初始化按键
	Phase_Measure_Init(0XFFFFFFFF,84-1);//以1Mhz的频率计数 
	//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1);
	
	while(1)	//进入程序主循环，主循环周期大约为0.88s, f=1.14Hz, 满足秒更新1-2次的要求
	{			//主循环不能有delay，否则会影响数码管动态显示。故把delay替换成了Tube_delay
		
		Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
		///////////////////////////////////按钮选择部分/////////////////////////////
		if(0 == KEY_BEEP){Beep_En = 1;}
		else{Beep_En = 0;}
		if(0 == KEY_FP){Fre_or_Phase = 1;}
		else{Fre_or_Phase = 0;}
		
		printf("Bepp_En = %d",Beep_En);
		Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));		//应该在每一句需要花费STM32芯片时间的语句后面加一个Tube_delay，防止闪烁，最小单位：15，保险起见16。
		printf("   Fre_or_Phase = %d\r\n",Fre_or_Phase);
		Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
		
		//////进入测量部分前，应该完成了：TIM5、TIM2各一次高电平捕获，当然也完成了差值捕获
		///////////////////////////////////输入测量部分/////////////////////////////
		if(TIM5CH1_CAPTURE_STA&0X80){//TIM5
			//成功捕获到了一次高电平
			T=TIM5CH1_CAPTURE_STA&0X3F;//5
			T*=0XFFFFFFFF;
			T+=TIM5CH1_CAPTURE_VAL;//5
			T*=2;
			DIFF = TIM2TIM5_DIFF_VAL;
			printf("T of the signal:%lld us\r",T);
			Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
			//软件滤波使得无法测量频率高于1200、频率低于900、相位大于150的情况！！！
			if((1000000/T<=1220)&&(1000000/T>=890))//软件滤波
				frequency = 1000000/T-2;//-2是为了矫正
			if(DIFF*360/T<=150)//软件滤波
				phase_diff = DIFF*360/T-1;//-1是为了进行矫正
			printf("///Frequency of the signal:%lld Hz\r",frequency);
			Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
			printf("///Phase Difference = %lld\r\n",phase_diff);
			Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
			///////////////////////////////////输出显示部分/////////////////////////////
			//printf("   Phase Difference = %d",phase_diff);
			//Tube_delay(16,phase_diff);
			//printf("   Frequency = %d",frequency);
			//Tube_delay(16,phase_diff);
			if(Fre_or_Phase==0){//选择显示相位测量
			//如果开关允许报警，此处蜂鸣器警报花费0.8s.
				if(Beep_En==1){		//当Fre_or_Phase=0，BEEP_EN=1时，才会有蜂鸣器报警
					if(phase_diff<5){
					Beep_Low(phase_diff);
					}
					if(phase_diff>135){
					Beep_High(phase_diff);
					}
				}
				else{
					Tube_delay(400,phase_diff);//delay时间尽量能被15整除？
				}
				Tube_clear(phase_diff);//此函数需要400ms
			}
			else{//Fre_or_Phase==1 选择显示频率测量
				Tube_delay(800,frequency-900);//显示频率和900的差值
			}
			TIM5CH1_CAPTURE_STA=0;			//开启下一次捕获
			TIM2CH1_CAPTURE_STA=0;
		}
	}	
}


/*

///i++;
			if(i>=10){
				TIM5->CR1&=~(1<<0);   		//第10次下降沿结束后，禁止定时器5
				TIM5->DIER&=~(1<<1);   		//禁止捕获1中断				
				TIM5->DIER&=~(1<<0);   		//禁止更新中断
				for(i=0;i<10;i++){
					T += t[i];					//8589934560 us
					printf("t[%d] of the signal:%lld us\r\n",i,2*t[i]);
					pd = PHASE_DIFFF_TOTAL_VAL[i];
					printf("pd[%d] of the signals:%lld us\r\n",i,pd);
					phase_diff += pd;
				}
				T = T/5;
				frequency = 1000000/T;
				phase_diff = pd/10;
				printf("T of the signal:%lld us\r\n",T);//打印总的高点平时间
				Tube_delay(16,phase_diff);
				printf("Frequency of the signal:%lld Hz\r\n",frequency);//打印总的高点平时间
				Tube_delay(16,phase_diff);
				printf("Phase difference of the signals：%lld Deg \r\n",phase_diff);
				Tube_delay(16,phase_diff);
			}





		TIM5->DIER|=1<<1;   	//允许捕获1中断				
		TIM5->DIER|=1<<0;   	//允许更新中断
		TIM5->CR1|=0x01;    	//使能定时器2
		//status =0;
		if(i>=10){
			i = 0;
			TIM5CH1_CAPTURE_STA=0;			//开启下一次捕获
		}

*/











