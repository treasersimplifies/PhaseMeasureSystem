#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
////////////////////////////////////////////////////////////////////////////////// 	 
//定时器5通道1输入捕获配置
//arr：自动重装值(TIM2,TIM5是32位的!!)
//psc：时钟预分频数

void TIM2_CH1_Cap_Init(u32 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<0;   	//TIM2 时钟使能 
	RCC->AHB1ENR|=1<<0;   	//使能PORTA时钟	
	GPIO_Set(GPIOA,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD);//PA5复用功能,下拉
	GPIO_AF_Set(GPIOA,5,1);	//PA5,AF1
	  
 	TIM2->ARR=arr;  		//设定计数器自动重装值   
	TIM2->PSC=psc;  		//预分频器 

	TIM2->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上，即通道1！！！！！！
 	TIM2->CCMR1|=0<<4; 		//IC1F=0000 配置输入滤波器 不滤波
 	TIM2->CCMR1|=0<<10; 	//IC1PS=00 	配置输入分频,不分频 

	TIM2->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM2->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中

	TIM2->EGR=1<<0;			//软件控制产生更新事件,使写入PSC的值立即生效,否则将会要等到定时器溢出才会生效!
	TIM2->DIER|=1<<1;   	//允许捕获1中断				
	TIM2->DIER|=1<<0;   	//允许更新中断
	TIM2->CR1|=0x01;    	//使能定时器2	
	MY_NVIC_Init(2,0,TIM2_IRQn,2);//抢占2，子优先级0，组2,与TIM5捕获同优先级，看谁先发生。
}

void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<3;   	//TIM5 时钟使能 
	RCC->AHB1ENR|=1<<0;   	//使能PORTA时钟	
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD);//复用功能,下拉
	GPIO_AF_Set(GPIOA,0,2);	//PA0,AF2
	  
 	TIM5->ARR=arr;  		//设定计数器自动重装值   
	TIM5->PSC=psc;  		//预分频器 

	TIM5->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上，即通道1！！！！！！
 	TIM5->CCMR1|=0<<4; 		//IC1F=0000 配置输入滤波器 不滤波
 	TIM5->CCMR1|=0<<10; 	//IC1PS=00 	配置输入分频,不分频 

	TIM5->CCER|=0<<1; 		//CC1P=0	上升沿捕获
	TIM5->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中

	TIM5->EGR=1<<0;			//软件控制产生更新事件,使写入PSC的值立即生效,否则将会要等到定时器溢出才会生效!
	TIM5->DIER|=1<<1;   	//允许捕获1中断				
	TIM5->DIER|=1<<0;   	//允许更新中断	
	TIM5->CR1|=0x01;    	//使能定时器5
	MY_NVIC_Init(2,0,TIM5_IRQn,2);//抢占2，子优先级0，组2	   
}

void Phase_Measure_Init(u32 arr,u16 psc){
	TIM2_CH1_Cap_Init(arr,psc);
	TIM5_CH1_Cap_Init(arr,psc);
}

//捕获状态，捕获高电平，1/(高电平时间*2)=频率
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到上升沿;1,已经捕获到上升沿了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
//int i = 0;					//捕获10次后关闭，需要10ms.
//int j = 0;
//int k =0;
//int status = 0;
//u32 PHASE_DIFFF_TOTAL_VAL[10];
//u32	PHASE_DIFFF_CAPTURE_VAL=0;
u8  TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM5CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
u32	TIM2TIM5_DIFF_VAL;
//u32 TIM5_TOTAL_VAL[10];
//u32 TIM2_TOTAL_VAL[10];

//定时器5中断服务程序，TIM5先捕获到上升沿
void TIM5_IRQHandler(void)
{ 	
	u16 tsr;
	tsr=TIM5->SR;
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tsr&0X01)//溢出
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//捕获1发生捕获事件
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//已经捕获到一个上升沿，这次又捕获到了下降沿	
			{	  			
			    TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//获取当前的捕获值，执行此条指令需要一定的时间，1us？
												//所以上次读取高电平的指令和本指令之间的差最好就是隔
				TIM5CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				//printf("完成一次捕获\r\n");
				//printf("TIM5->CCR1 = %d",TIM5CH1_CAPTURE_VAL);
				//TIM5_TOTAL_VAL[i-1] = TIM5CH1_CAPTURE_VAL; //+(TIM5CH1_CAPTURE_STA&0X3F)*0XFFFFFFFF;
				/*if(i>=10){
					TIM5->DIER&=~(1<<1);   		//禁止捕获1中断				
					TIM5->DIER&=~(1<<0);   		//禁止更新中断
					TIM5->CR1&=~(1<<0);   		//第10次下降沿结束后，禁止定时器5
					status = 1;
				}*/
	 			TIM5->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
			}else  								//还未开始,第一次捕获上升沿
			{	
				//printf("上升沿捕获");
				TIM5->CNT=0;					//计数器清空
				TIM5CH1_CAPTURE_STA=0;			//清空
				TIM5CH1_CAPTURE_VAL=0;			//TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM5->CR1&=~(1<<0);    			//禁止定时器5
	 			TIM5->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获
				TIM5->CR1|=0x01;    			//使能定时器5
			}		    
		}			     	    					   
 	}
	TIM5->SR=0;									//清除中断标志位   
}

u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u32	TIM2CH1_CAPTURE_VAL;	//输入捕获值(TIM2/TIM5是32位)
//TIM2后捕获到上升沿，所以在TIM2捕获到上升沿的时候就可以得出两者上升沿的时间差
void TIM2_IRQHandler(void)
{ 
	u16 tsr;
	tsr=TIM2->SR;
	//printf("TIM2中断");
	if((TIM2CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(tsr&0X01)//是溢出中断
		{	     
			if(TIM2CH1_CAPTURE_STA&0X40)//已经捕获到上升沿， 高电平了
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//取TIM2CH1_CAPTURE_STA后5位，即溢出次数，如果连溢出次数都超过了32次
				{									//说明，测量已经超出本程序量程了，可以直接结束测量了。
					TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获了一次
					TIM2CH1_CAPTURE_VAL=0XFFFFFFFF;		
				}else TIM2CH1_CAPTURE_STA++;		//如果是正常情况，就记录一次溢出次数
			}	 
		}
		if(tsr&0x02)//是捕获中断，捕获1发生捕获事件
		{	//printf("TIM2捕获中断");
			if(TIM2CH1_CAPTURE_STA&0X40)		//如果，取[6],已经捕获到一个上升沿，则这次捕获必定是下降沿，说明本轮测量程序可以结束了		
			{	  	
				TIM2CH1_CAPTURE_VAL=TIM2->CCR1;	//获取当前的捕获值				
				TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
				TIM2->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获，因为是循环，所以需要准备下一轮捕获。
			}
			else  								//还未开始,第一次捕获上升沿
			{
				TIM2->CNT=0;					//计数器清空
				TIM2TIM5_DIFF_VAL=TIM5->CCR1;
				//printf("PD = %d",TIM2TIM5_DIFF_VAL);
				TIM2CH1_CAPTURE_STA=0;			//清空
				TIM2CH1_CAPTURE_VAL=0;			
				TIM2CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM2->CR1&=~(1<<0);    			//禁止定时器2
				TIM2->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获
				TIM2->CR1|=0x01;    			//使能定时器2
			}		    
		}		
	}	
	TIM2->SR=0;								//清除中断标志位   
}













