/*
 * BarGraphTest.c
 *
 *  Created on: Aug 3, 2015
 *      Author: ken
 */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define BIT(x) (1<<x)
#define CHAN0	0
#define CHAN1	BIT(MUX0)
#define CHAN2	BIT(MUX1)
#define CHAN3	CHAN1|CHAN2

#define NOISE_REDUCTION

//#define DOUBLE_STEPS
#ifdef DOUBLE_STEPS
#define SCALE_FACTOR 10;
#else
#define SCALE_FACTOR 20;
#endif

#ifndef DOUBLE_STEPS
void LedBar(unsigned char value)
{
	switch(value)
	{
	case 0:
		PORTC=0xff;
		PORTD |= (unsigned char)0xF0;
		break;
	case 1:
		PORTC=~1;
		PORTD |= (unsigned char)0xF0;
		break;
	case 2:
		PORTC=~3; //(1+2)
		PORTD |= (unsigned char)0xF0;
		break;
	case 3:
		PORTC=~5; //(1+4)
		PORTD |= (unsigned char)0xF0;
		break;
	case 4:
		PORTC=~0x0d;
		PORTD |= (unsigned char)0XF0;
		break;
	case 5:
		PORTC=~0x1d;
		PORTD |= (unsigned char)0xF0;
		break;
	case 6:
		PORTC=~0x2d;
		PORTD |= (unsigned char)0xF0;
		break;
	case 7:
		PORTC=~0x6d;
		PORTD |= (unsigned char)0xF0;
		break;
	case 8:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		break;
	case 9:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= ~0x10;
		break;
	case 10:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= ~0x30;
		break;
	case 11:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= ~0x50;
		break;
	case 12:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= (unsigned char)~0xD0;
		break;
	}
}

#else

void LedBar(unsigned char value)
{
	switch(value)
	{
	case 0:
		PORTC=0xff;
		PORTD |= (unsigned char)0xF0;
		break;
	case 1:
	case 2:
		PORTC=~1;
		PORTD |= (unsigned char)0xF0;
		break;
	case 3:
	case 4:
		PORTC=~3; //(1+2)
		PORTD |= (unsigned char)0xF0;
		break;
	case 5:
	case 6:
		PORTC=~5; //(1+4)
		PORTD |= (unsigned char)0xF0;
		break;
	case 7:
	case 8:
		PORTC=~0x0d;
		PORTD |= (unsigned char)0XF0;
		break;
	case 9:
	case 10:
		PORTC=~0x1d;
		PORTD |= (unsigned char)0xF0;
		break;
	case 11:
	case 12:
		PORTC=~0x2d;
		PORTD |= (unsigned char)0xF0;
		break;
	case 13:
	case 14:
		PORTC=~0x6d;
		PORTD |= (unsigned char)0xF0;
		break;
	case 15:
	case 16:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		break;
	case 17:
	case 18:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= ~0x10;
		break;
	case 19:
	case 20:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= ~0x30;
		break;
	case 21:
	case 22:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= ~0x50;
		break;
	case 23:
	case 24:
		PORTC=(unsigned char)~0xAD;
		PORTD |= (unsigned char)0xF0;
		PORTD &= (unsigned char)~0xD0;
		break;
	}
}
#endif
int adc_result;  //now a global

#ifdef NOISE_REDUCTION
ISR(ADC_vect)
{
	//don't need to do anything, let the main code read the ADC
	sleep_disable();	//kill sleep mode
}
#endif

int main(void)
{

	PORTC = 0xFF;	//no leds on
	PORTD = 0xF0;	//no leds on
	DDRC = 0xff;	//all bits are outputs
	DDRD = 0xf0;	//upper 4 bits are outputs
	DDRA = 0;		//all inputs on A
	PORTA = 0xF0;	//upper four bits for channel select

	const int scale_factor = SCALE_FACTOR;	//divides into 24 segments

	ADMUX = BIT(REFS0)|BIT(ADLAR); //internal reference AVCC, channel 0, left justified
#ifdef NOISE_REDUCTION //enable ADC interrupt
	ADCSRA = BIT(ADEN)|BIT(ADPS1)|BIT(ADPS0)|BIT(ADIE);	//enable the ADC converter at 125khz clock (1mhz/8)
#else
	ADCSRA = BIT(ADEN)|BIT(ADPS1)|BIT(ADPS0);	//enable the ADC converter at 125khz clock (1mhz/8)
#endif
	set_sleep_mode(SLEEP_MODE_ADC);


//	int adc_result;
#ifdef DOUBLE_STEPS
	int prev_result = 0;
	int flip = 0;
#endif


	while(1){ //ADC loop
//		int channel = (PINA >> 4) & 2;		//two bits from PINA 4,5 select the channel
#ifdef NOISE_REDUCTION
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
        cli();
		adc_result = (ADCH / scale_factor);	//only want the MS 8 bits
#else
		ADCSRA |= BIT(ADSC); //(1<<ADSC);	//start a conversion
		while(!(ADCSRA & BIT(ADIF))) {};	//wait for the conversion to complete
		adc_result = (ADCH / scale_factor);	//only want the MS 8 bits
#endif

#ifdef DOUBLE_STEPS
		if(adc_result & 1){		//ODD
			if(prev_result == adc_result){	//twice or more in a row
				if(flip++){
					LedBar(adc_result-1);
				}
				else{
					LedBar(adc_result);
				}
				if(flip > 200)flip=0;	//flip flip
			}else{
				LedBar(adc_result);
				flip = 0;
			}
		}else{ //-------------------NOT ODD-------------------------
			LedBar(adc_result);//display the result
			flip = 0;
		}
		prev_result=adc_result;	//save new result
#endif
		LedBar(adc_result);//display the result
	}//end while loop
}


