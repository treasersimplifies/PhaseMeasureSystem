#include "sys.h"
#include "delay.h" 
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "beep.h"
#include "key.h"
//����ʹ�÷��䣺
//���벶��
//PA0��TIM5_CH1��	�ȳ���������
//PA5��TIM2_CH1��	�����������
//������: 		PF6
//������ 		PE4��PE3(PE2δʹ��)
//��λ�����:��SM310361D3B�Ķ�Ӧ��ϵ��
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
//ָʾLed��		PE7/PE8(δʹ��)


extern u8  	TIM2CH1_CAPTURE_STA;	//���벶��״̬		    				
extern u32	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

extern u8  	TIM5CH1_CAPTURE_STA;	//���벶��״̬		    				
extern u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)

extern u32 	TIM2TIM5_DIFF_VAL;

int main(void)
{ 
	int Beep_En = 0;
	int Fre_or_Phase = 0;
	long long T = 0;
	long long DIFF = 0;
	long long phase_diff=0;
	long long frequency = 0;
	
	Stm32_Clock_Init(336,8,2,7);		//����ʱ��,168Mhz
	delay_init(168);					//��ʼ����ʱ����
	uart_init(84,115200);				//���ڳ�ʼ��Ϊ115200
	
	DigitalTube_Init();					//��ʼ��LEDʱ��  
	BEEP_Init();         				//��ʼ���������˿�
	KEY_Init();							//��ʼ������
	Phase_Measure_Init(0XFFFFFFFF,84-1);//��1Mhz��Ƶ�ʼ��� 
	//TIM5_CH1_Cap_Init(0XFFFFFFFF,84-1);
	
	while(1)	//���������ѭ������ѭ�����ڴ�ԼΪ0.88s, f=1.14Hz, ���������1-2�ε�Ҫ��
	{			//��ѭ��������delay�������Ӱ������ܶ�̬��ʾ���ʰ�delay�滻����Tube_delay
		
		Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
		///////////////////////////////////��ťѡ�񲿷�/////////////////////////////
		if(0 == KEY_BEEP){Beep_En = 1;}
		else{Beep_En = 0;}
		if(0 == KEY_FP){Fre_or_Phase = 1;}
		else{Fre_or_Phase = 0;}
		
		printf("Bepp_En = %d",Beep_En);
		Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));		//Ӧ����ÿһ����Ҫ����STM32оƬʱ����������һ��Tube_delay����ֹ��˸����С��λ��15���������16��
		printf("   Fre_or_Phase = %d\r\n",Fre_or_Phase);
		Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
		
		//////�����������ǰ��Ӧ������ˣ�TIM5��TIM2��һ�θߵ�ƽ���񣬵�ȻҲ����˲�ֵ����
		///////////////////////////////////�����������/////////////////////////////
		if(TIM5CH1_CAPTURE_STA&0X80){//TIM5
			//�ɹ�������һ�θߵ�ƽ
			T=TIM5CH1_CAPTURE_STA&0X3F;//5
			T*=0XFFFFFFFF;
			T+=TIM5CH1_CAPTURE_VAL;//5
			T*=2;
			DIFF = TIM2TIM5_DIFF_VAL;
			printf("T of the signal:%lld us\r",T);
			Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
			//����˲�ʹ���޷�����Ƶ�ʸ���1200��Ƶ�ʵ���900����λ����150�����������
			if((1000000/T<=1220)&&(1000000/T>=890))//����˲�
				frequency = 1000000/T-2;//-2��Ϊ�˽���
			if(DIFF*360/T<=150)//����˲�
				phase_diff = DIFF*360/T-1;//-1��Ϊ�˽��н���
			printf("///Frequency of the signal:%lld Hz\r",frequency);
			Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
			printf("///Phase Difference = %lld\r\n",phase_diff);
			Tube_delay(16,Fre_or_Phase==0?phase_diff:(frequency-900));
			///////////////////////////////////�����ʾ����/////////////////////////////
			//printf("   Phase Difference = %d",phase_diff);
			//Tube_delay(16,phase_diff);
			//printf("   Frequency = %d",frequency);
			//Tube_delay(16,phase_diff);
			if(Fre_or_Phase==0){//ѡ����ʾ��λ����
			//����������������˴���������������0.8s.
				if(Beep_En==1){		//��Fre_or_Phase=0��BEEP_EN=1ʱ���Ż��з���������
					if(phase_diff<5){
					Beep_Low(phase_diff);
					}
					if(phase_diff>135){
					Beep_High(phase_diff);
					}
				}
				else{
					Tube_delay(400,phase_diff);//delayʱ�価���ܱ�15������
				}
				Tube_clear(phase_diff);//�˺�����Ҫ400ms
			}
			else{//Fre_or_Phase==1 ѡ����ʾƵ�ʲ���
				Tube_delay(800,frequency-900);//��ʾƵ�ʺ�900�Ĳ�ֵ
			}
			TIM5CH1_CAPTURE_STA=0;			//������һ�β���
			TIM2CH1_CAPTURE_STA=0;
		}
	}	
}


/*

///i++;
			if(i>=10){
				TIM5->CR1&=~(1<<0);   		//��10���½��ؽ����󣬽�ֹ��ʱ��5
				TIM5->DIER&=~(1<<1);   		//��ֹ����1�ж�				
				TIM5->DIER&=~(1<<0);   		//��ֹ�����ж�
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
				printf("T of the signal:%lld us\r\n",T);//��ӡ�ܵĸߵ�ƽʱ��
				Tube_delay(16,phase_diff);
				printf("Frequency of the signal:%lld Hz\r\n",frequency);//��ӡ�ܵĸߵ�ƽʱ��
				Tube_delay(16,phase_diff);
				printf("Phase difference of the signals��%lld Deg \r\n",phase_diff);
				Tube_delay(16,phase_diff);
			}





		TIM5->DIER|=1<<1;   	//������1�ж�				
		TIM5->DIER|=1<<0;   	//��������ж�
		TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��2
		//status =0;
		if(i>=10){
			i = 0;
			TIM5CH1_CAPTURE_STA=0;			//������һ�β���
		}

*/











