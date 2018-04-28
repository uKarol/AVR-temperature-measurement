/*
 * ADC.c
 *
 * Created: 07.02.2018 15:59:51
 *  Author: Karol
 */ 

#include "ADC.h"
#include <avr/io.h>

void adc_init(){
	ADMUX |= (1<<REFS0)|(1<<REFS1); // 1,1V odniesienia 
	ADCSRA |= (1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); // prescaler 128
	ADCSRA |= (1<<ADEN);
	DIDR0 |= (1<<ADC0D)|(1<<ADC1D);
}

int adc_measure(unsigned char channel){
	
	ADMUX = (ADMUX & 0xF8)|channel;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	return ADCW;
	
}