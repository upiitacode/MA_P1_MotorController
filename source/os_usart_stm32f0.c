#include "os_usart_stm32f0.h"
#include "dma_usart_stm32f0.h"
#include "usart_stm32f0.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

#define USART_SIGNAL 1

osMutexDef(usart2_tx_mutex);
osMutexId usart2_tx_mutex_id;
osThreadId usart2_tx_owner;

osMutexDef(usart2_rx_mutex);
osMutexId usart2_rx_mutex_id;
osThreadId usart2_rx_owner;
int usart2_rx_nBytesRead;

void _os_usart2_tx_callback(void);
void _os_usart2_rx_callback(int);

void os_usart2_init(int baudrate){
	dma_and_usart2_init(baudrate);
	usart2_tx_mutex_id = osMutexCreate(osMutex(usart2_tx_mutex));
	usart2_rx_mutex_id = osMutexCreate(osMutex(usart2_rx_mutex));
}

void os_usart2_puts(const char* pData){
	osMutexWait(usart2_tx_mutex_id, osWaitForever);
	usart2_tx_owner = osThreadGetId();
	osSignalClear(usart2_tx_owner, USART_SIGNAL);

	usart2_dma_async_puts(pData, _os_usart2_tx_callback);

	osSignalWait(USART_SIGNAL, osWaitForever);
	osMutexRelease(usart2_tx_mutex_id);
}

int os_usart2_gets(char *pBuffer){
	osMutexWait(usart2_rx_mutex_id, osWaitForever);
	usart2_rx_owner = osThreadGetId();
	osSignalClear(usart2_rx_owner, USART_SIGNAL);

	usart2_async_gets(pBuffer, _os_usart2_rx_callback);

	osSignalWait(USART_SIGNAL, osWaitForever);
	osMutexRelease(usart2_rx_mutex_id);
	return usart2_rx_nBytesRead;
}

void _os_usart2_tx_callback(void){
	osSignalSet(usart2_tx_owner, USART_SIGNAL);
}

void _os_usart2_rx_callback(int nBytesRead){
	usart2_rx_nBytesRead  =  nBytesRead;
	osSignalSet(usart2_rx_owner, USART_SIGNAL);
}
