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

#include "twi.c"
#include "usart.c"
#include "battery.c"
#include "servo.c"

#define PROM_SIZE 7
#define RESET_COMMAND 0x1E
#define PROM_READ_COMMAND 0xA0
#define ADC_READ_COMMAND 0x00

#define TEMPERATURE_4096 0x58
#define PRESSURE_4096 0x48

/*CONFIGURE*/
#define BATTERY_THRESHOLD 20 //%

enum prom_address{C0, C1, C2, C3, C4, C5, C6};

void all_modules_init (void);
void command_sequence (void);
void calculate_compensated_pressure (void);

uint32_t D1;
uint32_t D2;
uint16_t prom[PROM_SIZE];
int32_t pressure;

int main (void)
{
	cli ();
	all_modules_init ();
	sei ();

	usart_read ();

	while (true) {
		command_sequence ();
		calculate_compensated_pressure ();
		_delay_ms (1000);
		while ((battery_read ()*100)/1024 < BATTERY_THRESHOLD) {
			servo_start ();
			usart_write ("surfacing!\n\r");
		}

		_delay_ms (1000);
	}
	return 0;
}

void all_modules_init (void)
{
	battery_init ();
	twi_init ();
	servo_init ();
	usart_init ();
}

void command_sequence (void)
{
	
	//Reset comand sequence
	if (twi_send_start ()) {
		if (twi_send_addr_w ()) {
			if (twi_send_command (RESET_COMMAND)) {
				if (twi_send_stop ()) {
				}
			}
		}
	}

	_delay_ms (10);
	for (int i = 0; i < PROM_SIZE; i++) {
		//PROM read command sequence
		if (twi_send_start ()) {
			if (twi_send_addr_w ()) {
				if (twi_send_command (PROM_READ_COMMAND + 2*i)) {
					if (twi_send_stop ()) {
					}
				}
			}
		}
		//SLAVE response, read TWDR
		if (twi_send_start ()) {
			if (twi_send_addr_r ()) {
				if (twi_receive_prom (&prom[i])) {
					if (twi_send_stop ()) {
					}
				}
			}
		}
	}
	//D1, Pressure OSR-4096 command sequence

	if (twi_send_start ()) {
		if (twi_send_addr_w ()) {
			if (twi_send_command (PRESSURE_4096)) {
				if (twi_send_stop ()) {
				}
			}
		}
	}
	_delay_ms (10);// max conversion time is ~9ms, see datasheet

	//ADC READ command sequence
	if (twi_send_start ()) {
		if (twi_send_addr_w ()) {
			if (twi_send_command (ADC_READ_COMMAND)) {
				if (twi_send_stop ()) {
				}
			}
		}
	}

	//SLAVE response, read TWDR
	
	if (twi_send_start ()) {
		if (twi_send_addr_r ()) {
			if (twi_receive_adc (&D1)) {
				if (twi_send_stop ()) {
				}
			}
		}
	}
	//D2, Temperature OSR-4096 command sequence

	if (twi_send_start ()) {
		if (twi_send_addr_w ()) {
			if (twi_send_command (TEMPERATURE_4096)) {
				if (twi_send_stop ()) {
				}
			}
		}
	}
	_delay_ms (10);// max conversion time is ~9ms, see datasheet

	//ADC READ command sequence
	if (twi_send_start ()) {
		if (twi_send_addr_w ()) {
			if (twi_send_command (ADC_READ_COMMAND)) {
				if (twi_send_stop ()) {
				}
			}
		}
	}

	//SLAVE response, read TWDR
	
	if (twi_send_start ()) {
		if (twi_send_addr_r ()) {
			if (twi_receive_adc (&D2)) {
				if (twi_send_stop ()) {
				}
			}
		}
	}
	
}

void calculate_compensated_pressure (void) 
{
	//first order compensation
	char str[1024];
	int32_t dT = D2 - (uint32_t)prom[C5]*(1 << 8);
	int32_t TEMP = 2000l + (int64_t)dT*prom[C6]/((uint32_t)1 << 23);

	int64_t OFF =  (int64_t)prom[C2]*((int64_t)1 << 17)
		    +  (int64_t)prom[C4]*dT/((int64_t)1 << 6);
	
	int64_t SENS = (int64_t)prom[C1]*((int64_t)1 << 16)
		     + (int64_t)prom[C3]*dT/((int64_t)1 << 7);
	
	int32_t P = (D1*SENS/((int64_t)1 << 21) - OFF)/((int64_t)1 << 15);
	
	pressure = P;

	sprintf (str, "PRESSURE: %li \n\r", P);
	usart_write (str);
	sprintf (str, "TEMPERATURE: %li\n\r", TEMP);
	usart_write (str);
}

//this wont work properly because there are some things i'm not seeing. more at 11'
