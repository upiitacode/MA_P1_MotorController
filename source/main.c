#include "stm32f0xx.h"                  // Device header
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "encoder_stm32f0.h"
#include "board_stm32f0.h"
#include "clockctrl_stm32f0.h"
#include "pwm_stm32f0.h"
#include "timer_stm32f0.h"
#include "os_serial_stdio.h"
#include "os_usart_stm32f0.h"

unsigned int encoder_position=0;
char inputBuffer[40];

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
	//Start Thread switching
	osKernelStart();
	//User Application
	os_usart2_puts("Hello, World\n");
	while(1){
		os_usart2_gets(inputBuffer);
		os_serial_printf(os_usart2_puts,">>%s\n",inputBuffer);
	}

	while(1){
		//Glow the led every 3 seconds aprox
		encoder_position=TIM_GetCounter(TIM3);
		for(int i=0; i<100; i++){
			TIMER2_CH2_PWM_SetDutyCycle(i,myAutorreload);
		}
	}
}

int update_counter;

void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)){
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		//GPIO_WriteBit(GPIOB,GPIO_Pin_1,!GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_1));//toggles led
		update_counter++;
	}
}

