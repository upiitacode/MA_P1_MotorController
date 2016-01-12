#include "usart_stm32f0.h"
#include "stm32f0xx.h"

void usart2_putc(char c){
	USART_SendData(USART2,c);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==0);
	//delay(0x1000);
}

char usart2_getc(void){
	while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE)==0);
	return USART_ReceiveData(USART2);
}

char* UART2_Rx_buffer;
int UART2_Rx_stringLength;
void (*UART2_Rx_callback)(int);
volatile int UART2_Rx_inProgress;

void usart2_async_gets(char* pString, void (*rx_complete_callback)(int)){
	//Prepare data
	UART2_Rx_callback = rx_complete_callback;
	UART2_Rx_buffer = pString;
	UART2_Rx_stringLength = 0;
	UART2_Rx_inProgress = 1;
	//Enable RX interrupt
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	NVIC_EnableIRQ(USART2_IRQn);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

static void rx_dummy_callback(int string_length){
}

int usart2_sync_gets(char* pString){
	usart2_async_gets(pString, rx_dummy_callback);
	while(UART2_Rx_inProgress);
	return UART2_Rx_stringLength;
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET){
		char rx_data = USART_ReceiveData(USART2);
		if(UART2_Rx_inProgress){
			if(rx_data == '\r' || rx_data == '\n'){
				UART2_Rx_buffer[UART2_Rx_stringLength] = '\0';
				UART2_Rx_callback(UART2_Rx_stringLength);
				UART2_Rx_inProgress = 0;
				USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
			}else{
				UART2_Rx_buffer[UART2_Rx_stringLength] = rx_data;
				UART2_Rx_stringLength++;
			}
		}
	}
}

void usart2_init(int baudrate){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*Enable peripherial clock for GPIOA*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	/*Enable peipherial clock for UART2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	/*GPIOA Configuration PA2 as TX PA3 as RX*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*Connect USART2 pins to AF2*/
	//TX=PA2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);

	USART_InitStructure.USART_BaudRate=baudrate;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Init(USART2,&USART_InitStructure);

	USART_Cmd(USART2,ENABLE);
}
