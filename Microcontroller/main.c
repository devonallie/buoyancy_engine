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
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <avr/interrupt.h> 

#define PORT1A 0b00000010

#define SURFACE 0
#define MAX_DEPTH 800

typedef enum command_type{
	UNSET;
	CALENDAR;
	USER;
	NETWORK;
	LOW_BATTERY
}Command;

Command command_type = UNSET;

void interrupt_init(void);
void configure_calendar(void);
void configure_network(void);

int main(int argc, char** argv)
{
	char option;

	while((option = getopt(argc, argv, "cu:n")) != -1) {
		switch(option) {
		case 'c':
			command_type = CALENDAR;
			configure_calendar();
		break;
		case 'u':
			command_type = USER;
		break;
		case 'n':
			command_type = NETWORK;
			configure_network();
		break;
		}
	}
	interrupt_init();
	while(true) {
		//stuff goes here of course
	}
	return EXIT_SUCCESS;
}

void configure_calendar(void)
{
	if(!calendar_set) {
		printf("No calendar detected, provide path to CSV\n");
		load_csv();
	}else {
		printf("Do you want to update the calendar? (Y/N)\n");
		while(true) {
			char[30] response;
			fscanf(fstream, "%s", response);
			switch(response) {
			case "Y\n":
				load_csv();
				return;
			case "N\n":
				return;
			default:
				printf("Invalid response, idiot.\n");
				break;
			}
		}

	}
	return;
}

void load_csv(fstream path)
{
	int i = 0;
	/*
	while(fscanf(fstream, //https://ideone.com/08zlcB see here for better version
	{
	calendar_set = true;
	*/
	return;
}


void configure_network(void)
{
}

void interrupt_init(void)
{
	DDR1A |= PORT1A;
	TIMKS0 = TOIE0;
	sei();
}

ISR (TIMER0_OVF_vect)
{
	PORTA1 ~= PORTA1;
}
