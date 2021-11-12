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

#define F_CPU 16000000UL
#define USART_BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUD * 16UL))) - 1)
#define MAX_STRING_SIZE 1024

void usart_init (void);
void usart_print (char str[MAX_STRING_SIZE]);


void usart_init (void)
{
	UBRR0H = BAUD_PRESCALE >> 8;
	UBRR0L = BAUD_PRESCALE;

	UCSR0C = _BV (UCSZ01)| _BV (UCSZ00);
	UCSR0B = _BV (RXEN0) | _BV (TXEN0);
}

void usart_print (char str[MAX_STRING_SIZE])
{
	while (*str) {
		while ((UCSR0A & _BV (UDRE0)) == 0); // Do nothing until UDR is ready
		UDR0 = *str++;
	}
}



//this wont work properly because there are some things i'm not seeing. more at 11'
