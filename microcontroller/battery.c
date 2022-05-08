/* Author:    Devon Allie, Logan Crooks, Ben Anderson, and Andrew Mercer
 * 
 * Purpose:   This programme will manipulate the motor of a buoyancy engine
 *            through a serial connection to the ATMegaET128 microcontroller.
 * 
 * Ownership: This programme is written for ULTRA Electronics Marine Division.
 *            All software within belongs to me.
 *
 */

/*
 * All defined variables and data structures will be moved to their own header
 * file(s) eventually.
 */
#include <avr/interrupt.h> 
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdio.h>
#include <usart.c>

#define AVCC _BV (REFS0)
#define ADC_ON _BV (ADEN)
#define ADC_START_CONVERSION _BV (ADSC)
#define CONVERSION_IN_PROGRESS (ADCSRA & _BV (ADSC))
#define ADC_PRESCALE (_BV (ADPS2) | _BV (ADPS1) | _BV (ADPS0))

/*CONFIG*/

#define ADC_VREF AVCC
#define ADC_READ_PIN 0
#define ADC_BIAS_PIN 1
#define BLADDER_DRAIN_PIN 2
#define LOW_BATTERY_THRESHOLD 20
#define STRING_MAX 80
#define ADC_BIT_DEPTH 1024
#define PERCENT 100

void battery_init (void);
bool battery_state (void);
void battery_enable (void);
void battery_disable (void);

void battery_init (void)
{

	DDRC   |= _BV (ADC_BIAS_PIN);
	DDRB   |= _BV (BLADDER_DRAIN_PIN);
	PORTC  |= _BV (ADC_BIAS_PIN);
	ADMUX  |= ADC_VREF | ADC_READ_PIN; 
	ADCSRA |= ADC_ON   | ADC_PRESCALE | ADC_START_CONVERSION; 
	DIDR0  |= ADC_READ_PIN;
}

void battery_enable (void)
{
	PRR &= ~_BV (PRADC);
}

void battery_disable (void)
{
	PRR |= _BV (PRADC);
}

bool battery_state (void)
{
	PORTC &= ~_BV (ADC_BIAS_PIN);
	ADCSRA |= ADC_START_CONVERSION;
	while (CONVERSION_IN_PROGRESS);
	char str[STRING_MAX];
	sprintf (str, "BATTERY PERCENT: %u \n\r", ADCW*ADC_BIT_DEPTH/PERCENT);
	usart_write (str);
	if (ADCW <= LOW_BATTERY_THRESHOLD) {
		PORTB &= ~_BV (BLADDER_DRAIN_PIN);
		return true;
	}			
	PORTC |= _BV (PORTC1);
	PORTB |= _BV (BLADDER_DRAIN_PIN);
	return false;
}

