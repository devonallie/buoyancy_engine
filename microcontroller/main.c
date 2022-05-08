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
#define ASCII_RC 13
#define DEPTH_LSB 5
#define DEPTH_MSB 4
#define STRING_MAX 80

/*CONFIGURE*/
#define DATA_SIZE 6
#define SERVO_ACTIVATION_MAX 10
#define SERVO_ACTIVATION_MIN 5
#define SERVO_DELAY_MS 100

void modules_init (void);

uint8_t user_data[DATA_SIZE];
enum battery_state{ALIVE, DEAD};

void get_dive_schedule (void);
void get_user_input (void);

uint16_t target_depth = 0;

int main (void)
{
	modules_init ();	
	get_dive_schedule ();

	while (get_pressure() < target_depth) {
		while (battery_state () == DEAD);
		servo_run (SERVO_ACTIVATION_MAX);
		_delay_ms (SERVO_DELAY_MS);
		servo_run (SERVO_ACTIVATION_MIN);
		_delay_ms (SERVO_DELAY_MS);
	}
	return EXIT_SUCCESS;
}
void get_dive_schedule (void)
{
	for (int i = 0; i < DATA_SIZE; i++) {
		user_data[i] = usart_read ();
	}
	target_depth  = user_data[DEPTH_LSB];
	target_depth |= ((uint16_t) user_data[DEPTH_MSB] << 8);
}

void get_user_input (void)
{
	uint8_t buff = 0;
	char str[STRING_MAX], target_str[STRING_MAX];
	while (((buff = usart_read ())) != ASCII_d) {
		if ((buff != ASCII_RC)) {
			sprintf (str, "%c", buff);
			usart_write (str);
			strcat (target_str, str);
			
		} else {
			usart_write ("\n\r");
			if ((uint16_t)atoi (target_str) > target_depth) {
				target_depth = (uint16_t)atoi (target_str);
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

