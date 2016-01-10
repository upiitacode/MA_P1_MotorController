#include "stm32f0xx.h"                  // Device header
#include "encoder_stm32f0.h"
#include "board_stm32f0.h"
#include "clockctrl_stm32f0.h"


unsigned int encoder_position=0;

int main(void){
	setToMaxSpeed();
	led_init();
	timer_3_encoder_init();
	while(1){
		encoder_position=TIM_GetCounter(TIM3);
		//for(int i=0; i<0xFFFFF;i++);
		//GPIO_WriteBit(GPIOA,GPIO_Pin_1,!GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1));//toggles led
	}
}

int update_counter;
int capture_counter;
int capture_val;

void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)){
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		//GPIO_WriteBit(GPIOB,GPIO_Pin_1,!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1));//toggles led
		update_counter++;
	}
}

