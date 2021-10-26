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
//#include "RTClib.h"

//RTC_DS1307 rtc; //proposed model for the rtc?

#define LOW_BATTERY_THRESHOLD 10;
#define ADC_BIT_WIDTH 1024 // 2^10, 10 bits

typedef union {
	struct {
		uint8_t ADCL : 8;
		uint8_t ADCH  : 2;
	}
	uint16_t jerry;	
} ADC_val;

int counter = 0;

void pressure_sensor_init (void);
void battery_init (void);

int main (void)
{
	battery_init ();
	sei();
	while (true) {
	}
	return 0;
}

void battery_init (void)
{
	DDRD   |=  _BV (OC0A); // output compare pin set to write
	TCCR0A |= (_BV (COM0A1)) | (_BV (COM0A1)); //sets PD6 HIGH on OCR0A match
	TCCR0B |=  _BV (CS00) // no prescaler
	OCR0A   =   255; //random number for now, to be updated with formula based on F_CPU and polling rate for batter

	ADMUX   =  _BV (MUX1); //ADC2
	ADCSRA  = (_BV (ADEN))   | (_BV (ADIE) | (_BV (ADATE); //adc interrupt on trigger
	ADCSRB  = (_BV (ADTS1))  | (_BV (ADTS0));	//adc trigger being OCR0A match
}

void pressure_sensor_init (void)
{
	TWAR = 0x76 << 1; // address of the MS5837 shifted one because TWGCE is lsb 
	TWBR = 2// pg 221 gves the formula for this but with 16 mHz, no pre-scale, we get 0.8 mHz
	TWCR = (_BV (TWIE) | (_BV (TWEN)); // enables TWI interrupts and TWI
}

ISR (ADC_vect) //triggers when ADC conversion is complete which starts when compare match occurs on OCR0A
{
	ADV_val levels;
	levels.ADCL = ADCL;
	levels.ADCH = ADCH;

	if (levels.jerry/ADC_BIT_WIDTH < LOW_BATTERY_THRESHOLD) {
		//interact with motor
	}
}
