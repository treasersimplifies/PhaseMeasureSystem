#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
////////////////////////////////////////////////////////////////////////////////// 	 
//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��

void TIM2_CH1_Cap_Init(u32 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<0;   	//TIM2 ʱ��ʹ�� 
	RCC->AHB1ENR|=1<<0;   	//ʹ��PORTAʱ��	
	GPIO_Set(GPIOA,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD);//PA5���ù���,����
	GPIO_AF_Set(GPIOA,5,1);	//PA5,AF1
	  
 	TIM2->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM2->PSC=psc;  		//Ԥ��Ƶ�� 

	TIM2->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1�ϣ���ͨ��1������������
 	TIM2->CCMR1|=0<<4; 		//IC1F=0000 ���������˲��� ���˲�
 	TIM2->CCMR1|=0<<10; 	//IC1PS=00 	���������Ƶ,����Ƶ 

	TIM2->CCER|=0<<1; 		//CC1P=0	�����ز���
	TIM2->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����

	TIM2->EGR=1<<0;			//������Ʋ��������¼�,ʹд��PSC��ֵ������Ч,���򽫻�Ҫ�ȵ���ʱ������Ż���Ч!
	TIM2->DIER|=1<<1;   	//������1�ж�				
	TIM2->DIER|=1<<0;   	//��������ж�
	TIM2->CR1|=0x01;    	//ʹ�ܶ�ʱ��2	
	MY_NVIC_Init(2,0,TIM2_IRQn,2);//��ռ2�������ȼ�0����2,��TIM5����ͬ���ȼ�����˭�ȷ�����
}

void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{		 
	RCC->APB1ENR|=1<<3;   	//TIM5 ʱ��ʹ�� 
	RCC->AHB1ENR|=1<<0;   	//ʹ��PORTAʱ��	
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD);//���ù���,����
	GPIO_AF_Set(GPIOA,0,2);	//PA0,AF2
	  
 	TIM5->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM5->PSC=psc;  		//Ԥ��Ƶ�� 

	TIM5->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1�ϣ���ͨ��1������������
 	TIM5->CCMR1|=0<<4; 		//IC1F=0000 ���������˲��� ���˲�
 	TIM5->CCMR1|=0<<10; 	//IC1PS=00 	���������Ƶ,����Ƶ 

	TIM5->CCER|=0<<1; 		//CC1P=0	�����ز���
	TIM5->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����

	TIM5->EGR=1<<0;			//������Ʋ��������¼�,ʹд��PSC��ֵ������Ч,���򽫻�Ҫ�ȵ���ʱ������Ż���Ч!
	TIM5->DIER|=1<<1;   	//������1�ж�				
	TIM5->DIER|=1<<0;   	//��������ж�	
	TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��5
	MY_NVIC_Init(2,0,TIM5_IRQn,2);//��ռ2�������ȼ�0����2	   
}

void Phase_Measure_Init(u32 arr,u16 psc){
	TIM2_CH1_Cap_Init(arr,psc);
	TIM5_CH1_Cap_Init(arr,psc);
}

//����״̬������ߵ�ƽ��1/(�ߵ�ƽʱ��*2)=Ƶ��
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û����������;1,�Ѿ�������������.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
//int i = 0;					//����10�κ�رգ���Ҫ10ms.
//int j = 0;
//int k =0;
//int status = 0;
//u32 PHASE_DIFFF_TOTAL_VAL[10];
//u32	PHASE_DIFFF_CAPTURE_VAL=0;
u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
u32	TIM2TIM5_DIFF_VAL;
//u32 TIM5_TOTAL_VAL[10];
//u32 TIM2_TOTAL_VAL[10];

//��ʱ��5�жϷ������TIM5�Ȳ���������
void TIM5_IRQHandler(void)
{ 	
	u16 tsr;
	tsr=TIM5->SR;
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(tsr&0X01)//���
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//�Ѿ�����һ�������أ�����ֲ������½���	
			{	  			
			    TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//��ȡ��ǰ�Ĳ���ֵ��ִ�д���ָ����Ҫһ����ʱ�䣬1us��
												//�����ϴζ�ȡ�ߵ�ƽ��ָ��ͱ�ָ��֮��Ĳ���þ��Ǹ�
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				//printf("���һ�β���\r\n");
				//printf("TIM5->CCR1 = %d",TIM5CH1_CAPTURE_VAL);
				//TIM5_TOTAL_VAL[i-1] = TIM5CH1_CAPTURE_VAL; //+(TIM5CH1_CAPTURE_STA&0X3F)*0XFFFFFFFF;
				/*if(i>=10){
					TIM5->DIER&=~(1<<1);   		//��ֹ����1�ж�				
					TIM5->DIER&=~(1<<0);   		//��ֹ�����ж�
					TIM5->CR1&=~(1<<0);   		//��10���½��ؽ����󣬽�ֹ��ʱ��5
					status = 1;
				}*/
	 			TIM5->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{	
				//printf("�����ز���");
				TIM5->CNT=0;					//���������
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;			//TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM5->CR1&=~(1<<0);    			//��ֹ��ʱ��5
	 			TIM5->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز���
				TIM5->CR1|=0x01;    			//ʹ�ܶ�ʱ��5
			}		    
		}			     	    					   
 	}
	TIM5->SR=0;									//����жϱ�־λ   
}

u8  TIM2CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//TIM2�󲶻������أ�������TIM2���������ص�ʱ��Ϳ��Եó����������ص�ʱ���
void TIM2_IRQHandler(void)
{ 
	u16 tsr;
	tsr=TIM2->SR;
	//printf("TIM2�ж�");
	if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(tsr&0X01)//������ж�
		{	     
			if(TIM2CH1_CAPTURE_STA&0X40)//�Ѿ����������أ� �ߵ�ƽ��
			{
				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//ȡTIM2CH1_CAPTURE_STA��5λ���������������������������������32��
				{									//˵���������Ѿ����������������ˣ�����ֱ�ӽ��������ˡ�
					TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM2CH1_CAPTURE_VAL=0XFFFFFFFF;		
				}else TIM2CH1_CAPTURE_STA++;		//���������������ͼ�¼һ���������
			}	 
		}
		if(tsr&0x02)//�ǲ����жϣ�����1���������¼�
		{	//printf("TIM2�����ж�");
			if(TIM2CH1_CAPTURE_STA&0X40)		//�����ȡ[6],�Ѿ�����һ�������أ�����β���ض����½��أ�˵�����ֲ���������Խ�����		
			{	  	
				TIM2CH1_CAPTURE_VAL=TIM2->CCR1;	//��ȡ��ǰ�Ĳ���ֵ				
				TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
				TIM2->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز�����Ϊ��ѭ����������Ҫ׼����һ�ֲ���
			}
			else  								//��δ��ʼ,��һ�β���������
			{
				TIM2->CNT=0;					//���������
				TIM2TIM5_DIFF_VAL=TIM5->CCR1;
				//printf("PD = %d",TIM2TIM5_DIFF_VAL);
				TIM2CH1_CAPTURE_STA=0;			//���
				TIM2CH1_CAPTURE_VAL=0;			
				TIM2CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM2->CR1&=~(1<<0);    			//��ֹ��ʱ��2
				TIM2->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز���
				TIM2->CR1|=0x01;    			//ʹ�ܶ�ʱ��2
			}		    
		}		
	}	
	TIM2->SR=0;								//����жϱ�־λ   
}













