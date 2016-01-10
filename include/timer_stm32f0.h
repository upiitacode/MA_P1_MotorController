#ifndef _TIMER_STM32F0_H
#define _TIMER_STM32F0_H

/**
 * Initialized the timer 15 as a downcounter periodic timer
 * Also enables the update interrupt for this timer
 * @param period_ms timer period in milliseconds
 */
void timer_15_init(int period_ms);

#endif// _TIMER_STM32F0_H
