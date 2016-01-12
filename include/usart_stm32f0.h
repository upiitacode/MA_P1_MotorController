#ifndef _USART_STM32F0_H
#define _USART_STM32F0_H

void usart2_init(int baudrate);
void usart2_putc(char c);

void usart2_async_gets(char* pString, void (*rx_complete_callback)(int));
int usart2_sync_gets(char* pString);

char usart2_getc(void);

#endif// _USART_STM32F0_H
