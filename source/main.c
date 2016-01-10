#include "stm32f0xx.h"                  // Device header
#include "encoder_stm32f0.h"
#include "board_stm32f0.h"
#include "clockctrl_stm32f0.h"
#include "pwm_stm32f0.h"
#include "delay.h"

unsigned int encoder_position=0;

int main(void){
	setToMaxSpeed();
	short myPrescaler=47;//set Tick time to 1us (Fclk/(Prescaler+1))
	int myAutorreload=1000;//set cycle time to 1ms

	led_init();
	//Initialize encoder
	timer_3_encoder_init();
	//Initialize PWM
	TIMER2_CH2_PWM_Init(myPrescaler,myAutorreload);
	while(1){
		//Glow the led every 3 seconds aprox
		encoder_position=TIM_GetCounter(TIM3);
		for(int i=0; i<100; i++){
			TIMER2_CH2_PWM_SetDutyCycle(i,myAutorreload);
			delay_ms(0xFFFFF);
		}
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

