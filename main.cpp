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

#define DEPTH_TOLERANCE 5;
#define MAINTAIN_DEPTH_HZ 50;
#define REACH_DEPTH_HZ 100;

using namespace std;

bool target_reached = false;
bool calendar_set = false; // this won't work because the calendar is lost on boot.
			   // we don't have non-volatile mem anyway so maybe it's fine?
typedef enum sources{
	CALENDAR;
	USER;
	NETWORK;
}Source;

void command_processing(Source input_source);
void configure_calendar(void);

int main(int argc, char** argv){
	char option;
	Source input_source;

	while((option = getopt(argc, argv, "cu:n")) != -1){
		switch(option){
			case 'c':
				input_source = CALENDAR;
				configure_calendar();
			break;
			case 'u':
				input_source = USER;
			break;
			case 'n':
				input_source = NETWORK;
			break;
		}
		command_processing(input_source)
	}
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
	//call some other function to digest the CSV into a database
	calendar_set = true;
	return;
}

void command_processing(Source input_source){
	int target_depth = atoi(optarg);
	while(target_reached = false){
		poll_rate = reach;
		if(current_depth() < target_depth - DEPTH_TOLERANCE){
			dive();
		}else if(current_depth() > target_depth + DEPTH_TOLERANCE){
			rise();
		}else
			target_reached = true;
	}
	poll_rate = maintain;
}
