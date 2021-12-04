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
#include <avr/io.h>
#include <stdio.h>

#define PWM_OC1A 		 _BV (DDB1)
#define SERVO_BIAS_PIN		 _BV (DDB0)
#define SET_OC1A_ON_DOWN_COUNT	 _BV (COM1A1)
#define WAVE_GEN_MODE_8		 _BV (WGM13) //Phase correct & frequency correct PWM

#define N_1024			(_BV (CS12) | _BV (CS10))
#define N_256			 _BV (CS12)
#define N_64			(_BV (CS11) | _BV (CS10))
#define N_8			 _BV (CS11)
#define N_1			 _BV (CS10)

/* CONFIGURATION */
#define CLK_PRESCALE N_1024
#define F_PWM 50

void servo_init (void);
void servo_run (float duty_cycle);
void servo_enable (void);
void servo_disable (void);

void servo_init (void)
{
	DDRB   |= PWM_OC1A		| SERVO_BIAS_PIN;
	TCCR1A  = SET_OC1A_ON_DOWN_COUNT;
	TCCR1B  = WAVE_GEN_MODE_8	| CLK_PRESCALE;
}
	
void servo_run (float duty_cycle)
{
	PRR   &= ~_BV (PRTIM1); 
	PORTD |= SERVO_BIAS_PIN;
	ICR1   = F_CPU/F_PWM/(1 << (CLK_PRESCALE + 6));
	OCR1A  = (ICR1*duty_cycle)/100;	
}

void servo_enable (void)
{
	PRR &= ~_BV (PRTIM1);
}

void servo_disable (void)
{
	PRR |= _BV (PRTIM1);
}

