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
#include <string.h>
#include <stdlib.h>

#include "twi.c"
#include "usart.c"
#include "battery.c"
#include "servo.c"

#define ASCII_d 100

/*CONFIGURE*/
#define DATA_SIZE 6

void modules_init (void);

uint8_t user_data[DATA_SIZE];
enum battery_state{ALIVE, DEAD};

int main (void)
{
	modules_init ();	
	get_dive_schedule ();

	while (get_pressure() < target_depth*100) {
		while (battery_state () == DEAD);
		servo_enable (10);
		_delay_ms (100);
		servo_enable (5);
		_delay_ms (100);
	}
	return EXIT_SUCCESS;
}
void get_dive_schedule (void)
{
	for (int i = 0; i < DATA_SIZE; i++) {
		user_data[i] = usart_read ();
	}
	target_depth  = user_data[DEPTH_LSB]
	target_depth |= ((uint16_t)user_data[DEPTH_MSB] << 8);
}

void get_user_input (void)
{
	while (((buff = usart_read ())) != ASCII_d) {
		if ((buff != 13)) {
			sprintf (str, "%c", buff);
			usart_write (str);
			strcat (target_str, str);
			
		} else {
			usart_write ("\n\r");
			if (atoi (target_str) > target_depth) {
				target_depth = atoi (target_str);
				memset (&target_str[0], 0, sizeof (target_str));
			}
		}
	}
}

void modules_init (void)
{
	battery_init ();
	twi_init ();
	servo_init ();
	usart_init ();
	sei ();
}

