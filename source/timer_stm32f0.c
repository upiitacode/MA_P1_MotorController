#include "timer_stm32f0.h"
#include "stm32f0xx.h"                  // Device header

void timer_15_init(int period_ms){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
	TIM_TimeBaseInitTypeDef myTimer;
	TIM_TimeBaseStructInit(&myTimer);
	myTimer.TIM_CounterMode=TIM_CounterMode_Down;
	myTimer.TIM_Prescaler=(48000-1);//CountFreq=(48MHz/Prescaler+1)=1KHz
	myTimer.TIM_ClockDivision=TIM_CKD_DIV1;
	myTimer.TIM_Period=period_ms;
	TIM_TimeBaseInit(TIM15,&myTimer);

	TIM_InternalClockConfig(TIM15);

	TIM_ITConfig(TIM15,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM15_IRQn);

	TIM_Cmd(TIM15,ENABLE);
}

void TIM15_IRQHandler(void){
	TIM_ClearITPendingBit(TIM15,TIM_IT_Update);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1));//toggles led
}
