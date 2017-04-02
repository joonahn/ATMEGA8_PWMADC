/*
* servo_linear_control.c
*
* Created: 2016-09-08 오후 4:08:01
* Author : HyunSung
*/
#define F_CPU 8000000UL
#define OC1A_PWM_SET 1
#define OC1B_PWM_SET 2
#define OC1AB_PWM_SET 3
#define ubrr 51
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void pwmSet(int channel, double width);
void pwmInit(int channel);
int adcRead(int pinNum);

int main()
{
	int data = 0;
	double angle = 0;

	pwmInit(OC1A_PWM_SET);
	
	while(1)
	{
		data = adcRead(0);
		angle = 1.6*data/1024 + 0.7; //(2.3-0.7)*(data/1024) + 0.7
		pwmSet(OC1A_PWM_SET, angle);
	}
}

void pwmSet(int channel, double width)
{
	switch(channel)
	{
		case 1: OCR1A = (ICR1 / 20) * width; break;
		case 2: OCR1B = (ICR1 / 20) * width; break;
		case 3: OCR1A = (ICR1 / 20) * width;
		OCR1B = (ICR1 / 20) * width; break;
	}
}

void pwmInit(int channel)
{
	ICR1 = 19999;
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

	switch(channel)
	{
		case 1: TCCR1A = (1 << COM1A1) | (1 << WGM11); DDRB |= (1<<DDB1); break;
		case 2: TCCR1A = (1 << COM1B1) | (1 << WGM11); DDRB |= (1<<DDB2); break;
		case 3: TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); DDRB = DDRB | 1<<DDB1 | 1<<DDB2; break;
	}
}

int adcRead(int channel)
{
	int analog;

	/* select channel */
	switch (channel) {
		case 0:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 0 << MUX1 | 0 << MUX0;
		break;
		case 1:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 0 << MUX1 | 1 << MUX0;
		break;
		case 2:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 1 << MUX1 | 0 << MUX0;
		break;
		case 3:
		ADMUX = 1 << REFS0 | 0 << MUX2 | 1 << MUX1 | 1 << MUX0;
		break;
		case 4:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 0 << MUX1 | 0 << MUX0;
		break;
		case 5:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 0 << MUX1 | 1 << MUX0;
		break;
		case 6:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 1 << MUX1 | 0 << MUX0;
		break;
		case 7:
		ADMUX = 1 << REFS0 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0;
		break;
	}

	ADCSRA = 1 << ADEN | 1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2;

	/*   read analog value */
	ADCSRA = 1 << ADSC | ADCSRA;
	while (ADCSRA & (1 << ADSC))
	;
	analog = ADCW;

	_delay_ms(100);
	return analog;
}