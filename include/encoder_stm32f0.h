#ifndef _ENCODER_STM32F0_H
#define _ENCODER_STM32F0_H

/**
 * Initialized the timer 2 as a down counter triggering from the ETR pin
 * Also enables the update interrupt for this timer
 * @param upToValue number of counts to issue and iterrupt
 */
void timer_3_encoder_init(void);

#endif// _ENCODER_STM32F0_H
