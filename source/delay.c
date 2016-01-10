#include "delay.h"
#include <stdint.h>

extern uint32_t SystemCoreClock;

#if defined ( __GNUC__ )
	#define DELAY_LOOP_CYCLES 12
#else
	#define DELAY_LOOP_CYCLES 7
#endif

void delay_ms(int delay_time){
	int compensatedDelay =  delay_time * (SystemCoreClock / (1000*DELAY_LOOP_CYCLES));
	for(int i = 0; i < compensatedDelay; i++){
	}
}

void delay_us(int delay_time){
	int compensatedDelay =  delay_time * (SystemCoreClock / (1000000*DELAY_LOOP_CYCLES));
	for(int i = 0; i < compensatedDelay; i++){
	}
}
