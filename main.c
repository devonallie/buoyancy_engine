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

#define SURFACE 0
#define MAX_DEPTH 800

bool target_reached = false;
bool calendar_set = false; // this won't work because the calendar is lost on boot.
			   // we don't have non-volatile mem anyway so maybe it's fine?
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
//void configure_network(void);

int main(int argc, char** argv){
	char option;

	while((option = getopt(argc, argv, "cu:n")) != -1){
		switch(option){
			case 'c':
				command_type = CALENDAR;
				configure_calendar();
			break;
			case 'u':
				command_type = USER;
			break;
			case 'n':
				command_type = NETWORK;
				//configure_network();
			break;
		}
	}
	interrupt_init();
	return EXIT_SUCCESS;
}

void configure_calendar(void)
{
	if(!calendar_set)
	{
		printf("No calendar detected, provide path to CSV\n");
		load_csv();
	}else
	{
		printf("Do you want to update the calendar? (Y/N)\n");
		while(true)
		{
			char[30] response;
			fscanf(fstream, "%s", response);
			switch(response)
			{
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
	while(fscanf(fstream, //https://ideone.com/08zlcB see here for better version
	{
	calendar_set = true;
	return;
}

/*
 * void configure_network(void){
 * This can be used to configure network details from a terminal or have I/O piped through a GUI for config
 * }
 */

void interrupt_init(void) {
	//Set timer for polling battery 
	//DDR#X = POLLING CIRCUIT SWITCH (OUT)
	//DDR#X = ADC PIN (IN)
	//Battery ISR NEEDS max priority
	//
	//Set timer for tracking the calendar
	//
	//Set timer for polling depth sensor
	//
	//Set ISR for instrument loss;
	//
	//Set TWI interface
	sei();
}
/*
 * ISR (vector for battery timer) {
 * 	BATTER_PIN = HIGH;
 * 	ADCH or ADCL, these have the results of ADC, a few registers need to be configured to set this up on the right pin
 *	if(BATTER_DIGITAL_VALUE(taken from ADCH/ADCL) < BATTERY_THRESHOLD)
 *	{
 *		command_process(SURFACE, LOW_BATTERY);
 * 	}
 * }
