/* Author:    Devon Allie, Logan Crooks, Ben Anderson, and Andrew Mercer
 * 
 * Purpose:   This programme will manipulate the motor of a buoyancy engine
 *            through a serial connection to the ATMegaET128 microcontroller.
 * 
 * Ownership: This programme is written for ULTRA Electronics Marine Division.
 *            All software within belongs to them.
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

#define AVCC _BV (REFS0)
#define ADC_ON _BV (ADEN)
#define ADC_START_CONVERSION _BV (ADSC)
#define CONVERSION_IN_PROGRESS (ADCSRA & _BV (ADSC))

/*CONFIG*/

#define ADC_VREF AVCC
#define ADC_PIN 0 
#define ADC_PRESCALE 128 //2~128, by 2s

void battery_init (void);
uint16_t battery_read (void);

void battery_init (void)
{
	ADMUX  |= ADC_VREF | ADC_PIN; //ADC0, external voltage refence
	ADCSRA |= ADC_ON | ADC_PRESCALE; // sets ADC clock to 125 kHz which is within range of defined behaviour and max resolution 
	DIDR0  |= ADC_PIN; //disable digital pin function, PIN7, and PIN7 not possible
}

uint16_t battery_read (void)
{
	ADCSRA |= ADC_START_CONVERSION;
	while (CONVERSION_IN_PROGRESS);
	return ADCW;
}

//this wont work properly because there are some things i'm not seeing. more at 11'
