#ifndef _DMA_USART_STM32F0_H
#define _DMA_USART_STM32F0_H

#ifdef __cplusplus
extern "C" {
#endif

void dma_and_usart2_init(int baudrate);

void dma_channel_usart2_init(void);

void usart2_enable_dma(void);

void dma_usart2_irq_enable(void);

void dma_usart2_irq_disable(void);

void usart2_dma_async_puts(const char* pString, void (*tx_complete_callback)(void));
	
void usart2_dma_sync_puts(const char* pString);
	
void dma_usart2_nputs(const char* pString, int stringSize, void (*tx_complete_callback)(void));

void dma_usart2_waitUntilComplete(void);


#ifdef __cplusplus
}
#endif

#endif// _DMA_USART_STM32F0_H
