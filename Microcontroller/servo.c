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

#define OC2B DDD2

void servo_init (void)
{
	DDRD |= _BV (OC2B) | _BV (PORTD2);
	TCCR2A = _BV (COM2B1) | _BV (COM2B0) | _BV (WGM20);
	TCCR2B = _BV (WGM22) | _BV (CS20);
}

void run_servo (uint8_t duty_cycle)
{
	PRR &= ~_BV (PRTIM2); // clear powersaving bit that disable timer2
	servo_inti ();	
	PORTD |= _BV (PORTD2);	// registers must be reinit after powering on according to datasheet
	OCR2A = duty_cycle*255;		// some forumla based on OCR2A and the input of pwm_freq
}

void stop_servo (void)
{
	PRR |= _BV (PRTIM2);
}

//this wont work properly because there are some things i'm not seeing. more at 11'
