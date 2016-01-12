#ifndef _OS_USART_STM32F0_H
#define _OS_USART_STM32F0_H

void os_usart2_init(int baudrate);
void os_usart2_puts(const char* pData);
int os_usart2_gets(char *pBuffer);

#endif// _OS_USART_STM32F0_H
