#include "board_stm32f0.h"
#include "stm32f0xx.h"                  // Device header

void led_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	GPIO_InitTypeDef myGPIO;
	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Pin=GPIO_Pin_1;
	myGPIO.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Init(GPIOB,&myGPIO);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	GPIO_Init(GPIOA,&myGPIO);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}
