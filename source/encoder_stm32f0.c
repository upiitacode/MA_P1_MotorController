#include "encoder_stm32f0.h"
#include "stm32f0xx.h"                  // Device header

void timer_3_encoder_init(void){
	//Configure Pin B4 TIM3_CH1, PB5 TIM3_CH2
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;
	myGPIO.GPIO_Mode=GPIO_Mode_AF;
	myGPIO.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&myGPIO);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_1);

	//time base configuration
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef myTimer;
	TIM_TimeBaseStructInit(&myTimer);
	myTimer.TIM_CounterMode=TIM_CounterMode_Up;
	myTimer.TIM_Prescaler=(0);//microsecond count
	myTimer.TIM_ClockDivision=TIM_CKD_DIV1;
	myTimer.TIM_Period=(1000-1);//Overflow every 1000 pulses
	TIM_TimeBaseInit(TIM3,&myTimer);

	//capture channel 1 configuration
	TIM_ICInitTypeDef myCapture;
	TIM_ICStructInit(&myCapture);
	myCapture.TIM_Channel=TIM_Channel_1;
	myCapture.TIM_ICFilter=0xF;//no filter
	myCapture.TIM_ICPolarity=TIM_ICPolarity_Rising;
	myCapture.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	myCapture.TIM_ICSelection=TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&myCapture);

	//capture channel 3  configuration
	myCapture.TIM_Channel=TIM_Channel_2;
	TIM_ICInit(TIM3,&myCapture);

	//Encoder configuration
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI1,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

	//Interrupt configuration
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM3_IRQn);

	//start timer
	TIM_Cmd(TIM3,ENABLE);
}
