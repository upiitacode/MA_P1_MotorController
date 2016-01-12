#include "stm32f0xx.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "encoder_stm32f0.h"
#include "board_stm32f0.h"
#include "clockctrl_stm32f0.h"
#include "pwm_stm32f0.h"
#include "timer_stm32f0.h"
#include "os_serial_stdio.h"
#include "os_usart_stm32f0.h"
#include "adc_stm32f0.h"

unsigned int encoder_position=0;
char inputBuffer[40];
int turns_counter;


int main(void){
	//Set procesor speed
	setToMaxSpeed();
	//Initialize kernel
	osKernelInitialize();
	//Hardware initialize
	led_init();
	//Initialize encoder
	timer_3_encoder_init();
	//Initialize PWM
	short myPrescaler=47;//set Tick time to 1us (Fclk/(Prescaler+1))
	int myAutorreload=1000;//set cycle time to 1ms
	TIMER2_CH2_PWM_Init(myPrescaler,myAutorreload);
	//Initialize serial
	os_serial_init();
	os_usart2_init(9600);
	//Initialize adc
	adc_poll_init();
	//Start Thread switching
	osKernelStart();
	//User Application
	float adc_reading;
	os_usart2_puts("Hello, World\n");
	while(1){
		adc_reading = adc_poll_read()*(100.0/4095);
		encoder_position=TIM_GetCounter(TIM3);
		
		os_serial_printf(os_usart2_puts,">>%d\n",turns_counter);
		TIMER2_CH2_PWM_SetDutyCycle((unsigned int) adc_reading,myAutorreload);
		osDelay(200);
	}
}


void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)){
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		//GPIO_WriteBit(GPIOB,GPIO_Pin_1,!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1));//toggles led
		turns_counter++;
	}
}

