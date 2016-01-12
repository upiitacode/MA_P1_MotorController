#include "stm32f0xx.h"                  // Device header

void adc_poll_init(void){

	ADC_InitTypeDef myADC_Init;
	//Configure PA6 ADC_IN6
	GPIO_InitTypeDef myAnalogPin;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);//Turn on Port A
	ADC_StructInit(&myADC_Init);
	myAnalogPin.GPIO_Pin=GPIO_Pin_6;//configure PA6
	myAnalogPin.GPIO_Mode=GPIO_Mode_AN;//Pins as anlog pins
	GPIO_Init(GPIOA,&myAnalogPin);//init gpio port with configuration

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//Turn on peripherial ADC1
	/*Calibrate ADC*/
	ADC_Cmd(ADC1,DISABLE);
	ADC_GetCalibrationFactor(ADC1);
	//Configure Init Structure
	ADC_StructInit(&myADC_Init);
	myADC_Init.ADC_ScanDirection=ADC_ScanDirection_Upward;
	myADC_Init.ADC_DataAlign=ADC_DataAlign_Right;
	myADC_Init.ADC_Resolution=ADC_Resolution_12b;
	myADC_Init.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_Init(ADC1,&myADC_Init);
	ADC_Cmd(ADC1,ENABLE);

	ADC_ChannelConfig(ADC1,ADC_Channel_6,ADC_SampleTime_239_5Cycles);
	ADC_Cmd(ADC1,ENABLE);
}

uint16_t adc_poll_read(void){
	ADC_StartOfConversion(ADC1);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOSEQ));
	return ADC_GetConversionValue(ADC1);
}
