/*
 * oversampling.c
 *
 * Created: 07.02.2018 15:17:37
 * Author : Karol
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"
#include "lcd.h"

long int oversample(void);
long int average(void);

int main(void)
{
    /* Replace with your application code */
	//int temperatura, gnd, diff;
	lcd_init();
	adc_init();
	lcd_locate(0,0);
	Slide_text("test ADC");
	
	long srednia=0, ovr;
	//int sample=10;
	
    while (1) 
    {
		srednia = average();
		ovr = oversample();
		lcd_cls();
		lcd_put_string("ovrsmpl: ");
	    lcd_put_int(ovr/1000,10);
		lcd_put_string(",");
		lcd_put_int(ovr%1000,10);
		lcd_locate(0,1);
		lcd_put_string("average: ");
		lcd_put_int(srednia/10, 10);
		lcd_put_string(",");
		lcd_put_int(srednia%10, 10);
	    _delay_ms(500);

    }
}

long int average(void){
		int gnd=0, lm_out=0;
		long result= adc_measure(2) - adc_measure(1);
		int sample = 100;
		unsigned char loop_ctr;
		
		for(loop_ctr = 0; loop_ctr < sample; loop_ctr++){
			gnd =  adc_measure(1);
			lm_out = adc_measure(2);
			result = result * sample;
			result = result + (lm_out - gnd);
			result = result / (sample+1);			
		}
		
		return result*1100/1024;
	
}

long int oversample(void){
	
	long gnd=0, lm_out=0;
	long result;
	int ctr;
	for(ctr = 0; ctr <1024; ctr++){
		gnd +=  adc_measure(1);
		lm_out += adc_measure(2);
	}
	result = lm_out - gnd;
	
	result = result >> 5;
	
	result = result * 110000/32678;
	
	return result;
}