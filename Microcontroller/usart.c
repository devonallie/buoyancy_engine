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
#ifndef USART_C
#define USART_C
#include <avr/io.h>
#include <stdio.h>

#define F_CPU 16000000UL
#define USART_BAUD 9600
#define UBRR (((F_CPU / (USART_BAUD * 16UL))) - 1)
#define MAX_STRING_SIZE 1024
#define BAUD_HIGH_BYTE (UBRR >> 8)
#define BAUD_LOW_BYTE UBRR
#define DATA_FRAME_SIZE_8 (_BV (UCSZ01) | _BV (UCSZ00))
#define TRANSMIT_ENABLE _BV (TXEN0)
#define RECEIVE_ENABLE _BV (RXEN0)

void usart_init (void);
void usart_write (char str[MAX_STRING_SIZE - 1]);
uint8_t usart_read (void);

void usart_init (void)
{
	UBRR0H = BAUD_HIGH_BYTE;
	UBRR0L = BAUD_LOW_BYTE;
	
	UCSR0C = DATA_FRAME_SIZE_8; 
	UCSR0B = _BV (RXCIE0) | TRANSMIT_ENABLE | RECEIVE_ENABLE;
}

void usart_write (char str[MAX_STRING_SIZE])
{
	while (*str) {
		while (!(UCSR0A & _BV (UDRE0))); // Do nothing until UDR is ready
	UDR0 = *str++;
	}
}

uint8_t usart_read (void)
{
	while (!(UCSR0A & _BV (UDRE0)));
	return UDR0;
}

//this wont work properly because there are some things i'm not seeing. more at 11
//
#endif
