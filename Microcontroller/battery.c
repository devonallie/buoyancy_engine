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

#define OC0A DDD6
#define LOW_BATTERY_THRESHOLD 512
#define ADC_BIT_WIDTH 1024 
#define OCR0A_MAX 255

typedef union adc_val {
	struct {
		uint8_t low  : 8;
		uint8_t high : 2;
	} byte;
	uint16_t jerry;	
} adc_val;


void battery_init (void);


void battery_init (void)
{

	DDRB   |= _BV (OC0A); // output compare pin set to write
	TIMSK0  = _BV (OCIE0A);
	TCCR0A  = _BV (COM0A0) | _BV (WGM01); //toggles PD6 on OCR0A match
	TCCR0B  = _BV (CS02)   | _BV (CS00); // div 1024
	OCR0A   = OCR0A_MAX;
	ADMUX   = _BV (MUX0) | _BV (REFS0); //ADC0, external voltage refence
	ADCSRA  = _BV (ADEN) | _BV (ADATE) | _BV (ADIE) | _BV (ADPS2) | _BV (ADPS1) | _BV (ADPS0); //adc interrupt on trigger and prescale sets ADC clock to 125 kHz which is within range of defined behaviour and max resolution
	ADCSRB  = _BV (ADTS1)| _BV (ADTS0); //trigger adc ISR on OCR0A match 
	DIDR0   = _BV (ADC0D); //disable digital pin function
}

//this wont work properly because there are some things i'm not seeing. more at 11'
