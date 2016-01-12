#include "dma_usart_stm32f0.h"
#include "usart_stm32f0.h"
#include "stm32f0xx.h"
#include <string.h>

volatile int dma_usart2_tx_complete = 0;

void dma_and_usart2_init(int baudrate){
	usart2_init(baudrate);
	dma_channel_usart2_init();
	usart2_enable_dma();
}

void dma_channel_usart2_init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef myDMA;
	DMA_StructInit(&myDMA);
	myDMA.DMA_DIR = DMA_DIR_PeripheralDST;
	myDMA.DMA_M2M = DMA_M2M_Disable;
	myDMA.DMA_Mode = DMA_Mode_Normal;
	myDMA.DMA_Priority = DMA_Priority_Medium;

	myDMA.DMA_PeripheralBaseAddr =(uint32_t) &(USART2->TDR);
	myDMA.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	myDMA.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	myDMA.DMA_MemoryBaseAddr = 0x200000 ;
	myDMA.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	myDMA.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_Init(DMA1_Channel4,&myDMA);// DMA1 Channel 4 =  USART2_Tx
	NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
}

void usart2_enable_dma(void){
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
}

void dma_usart2_irq_enable(){
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
}

void dma_usart2_irq_disable(){
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,DISABLE);
}

void (*dma_usart2_tx_callback)(void);

void dma_usart2_nputs(const char* pString, int stringSize, void (*tx_complete_callback)(void)){
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA1_Channel4->CMAR =(uint32_t) pString;
	DMA_SetCurrDataCounter(DMA1_Channel4, stringSize);

	USART_ClearFlag(USART2, USART_FLAG_TC);
	DMA_ClearFlag(DMA1_FLAG_TC4);
	dma_usart2_tx_callback = tx_complete_callback;
	dma_usart2_tx_complete = 0;

	dma_usart2_irq_enable();
	DMA_Cmd(DMA1_Channel4,ENABLE);
}

void usart2_dma_async_puts(const char* pString, void (*tx_complete_callback)(void)){
	int strSize = strlen(pString);
	dma_usart2_nputs(pString, strSize, tx_complete_callback);	
}

static void dummy_callback(void){
}

void usart2_dma_sync_puts(const char* pString){
	usart2_dma_async_puts(pString,dummy_callback);
	dma_usart2_waitUntilComplete();
}

void dma_usart2_waitUntilComplete(void){
	while(dma_usart2_tx_complete == 0);
	dma_usart2_irq_disable();
}


#ifdef __cplusplus
extern "C" {
#endif
void DMA1_Channel4_5_IRQHandler(void){
	DMA_ClearITPendingBit(DMA1_IT_TC4);
	dma_usart2_tx_complete = 1;
	dma_usart2_tx_callback();
}
#ifdef __cplusplus
}
#endif
