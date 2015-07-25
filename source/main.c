#include "stm32f0xx.h"                  // Device header

/**
 * Set System Clock Speed to 48Mhz
 */
void setToMaxSpeed(void);

/**
 * Initialized the timer 2 as a down counter triggering from the ETR pin
 * Also enables the update interrupt for this timer
 * @param upToValue number of counts to issue and iterrupt
 */
void timer_3_encoder_init();

/**
 * Initialize Pin B1 as output
 */
void led_init(void);

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

void setToMaxSpeed(void){
	int internalClockCounter;
	RCC_PLLCmd(DISABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
	RCC_HSEConfig(RCC_HSE_OFF);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12);
	RCC_PREDIV1Config(RCC_PREDIV1_Div1);
	RCC_PLLCmd(ENABLE);
	while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	for(internalClockCounter=0;internalClockCounter<1024;internalClockCounter++){
		if(RCC_GetSYSCLKSource()==RCC_SYSCLKSource_PLLCLK){
			SystemCoreClockUpdate();
			break;
		}
	}
}

void timer_3_encoder_init(){
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
	//TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//NVIC_EnableIRQ(TIM3_IRQn);
	
	//start timer
	TIM_Cmd(TIM3,ENABLE);
}

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

