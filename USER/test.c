#include "sys.h"
#include "delay.h" 
#include "usart.h"

#include "led.h"
#include "beep.h"
#include "key.h"
//
//������: 		PF6
//������ 		PE4(PE3/PE2δʹ��)
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
int main(void)
{ 
	int BEEP_EN=0;
	int phase_diff=0;
	Stm32_Clock_Init(336,8,2,7);//����ʱ��,168Mhz
	delay_init(168);		//��ʼ����ʱ����
	uart_init(84,115200);	//���ڳ�ʼ��Ϊ115200
	
	DigitalTube_Init();				//��ʼ��LEDʱ��  
	BEEP_Init();         	//��ʼ���������˿�
	KEY_Init();				//��ʼ������
	
	while(1)//���������ѭ������ѭ��T=0.6s, f=1.67Hz,���������1-2�ε�Ҫ��
	{	
		//��ѭ��������delay�������Ӱ���������ʾ���ʰ�delay�滻����Tube_delay

		if(0 == KEY){BEEP_EN=!BEEP_EN;} //����������(����0.6s-1.2s)��֧�ַ������������ܣ��ٰ�һ�����ֹ��
		
		//�˴�д��εõ���λ��
		
		printf("BEEP_EN = %d\n",BEEP_EN);
		printf("Phase Difference = %d\n",phase_diff);
		DigitalTube_Set(phase_diff+151);
		
		//����������������˴���������������0.6s.
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














