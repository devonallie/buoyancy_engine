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
#include <util/twi.h>
#include <stdbool.h>
#include "usart.c"

#define TWINT_FLAG_LOW (!(TWCR & _BV (TWINT)))
#define TWSTO_FLAG_LOW (!(TWCR & _BV (TWSTO)))

/*CONFIG*/
#define MS5387 0x76
#define TWDR_SLAVE_ADDRESS (MS5387 << 1)
#define SCL_FREQ 400000
#define SCL_PRESCALE 1
#define PROM_SIZE 7

/*CONFIG-DEPENDANT*/
#define TWI_BAUD  ((F_CPU/SCL_FREQ) - 16)/2/SCL_PRESCALE

typedef enum {
	START 		= 0b10100100,
	SEND_SLA 	= 0b11000100,
	SEND_COMMAND 	= 0b11000100,
	RECEIVE_DATA 	= 0b11000100,
	RECEIVE_DATA_N 	= 0b10000100,
	STOP 		= 0b10010100,
} action_code;

typedef enum {
	NIL = 0,
	SLA_W = TWDR_SLAVE_ADDRESS | TW_WRITE,
	SLA_R = TWDR_SLAVE_ADDRESS | TW_READ,
	RESET = 0x1E,
	READ_PROM = 0xA0,
	READ_ADC = 0x00,
	TEMPERATURE_CONVERSION = 0x58,
	PRESSURE_CONVERSION = 0x48,
} command;

typedef enum {
	C0,
	C1,
	C2,
	C3,
	C4,
	C5,
	C6,
} prom_address;

void twi_init (void);
void twi_disable (void);
void twi_enable (void);
void twi (action_code ac, command cmd, uint8_t *data_buffer, uint8_t status);
int32_t get_pressure (void);

void twi_init (void)
{	
	PRR   &= ~_BV (PRTWI); 
	TWBR   =   TWI_BAUD;
	TWSR  &= ~_BV (TWPS1) | ~_BV (TWPS0);	
}

void twi_enable (void)
{
	PRR &= ~_BV (PRTWI);
}

void twi_disable (void)
{
	PRR |= _BV (PRTWI);
}


void twi (action_code ac, command cmd, uint8_t *data_buffer, uint8_t status)
{
	TWDR = cmd;
	TWCR = ac;
	if (status == TW_NO_INFO) {
		while (TWSTO_FLAG_LOW);
	} else {
		while (TWINT_FLAG_LOW);
	}
	*data_buffer = TWDR;
	if (TWSR == status) {
	}
	char str[80];
	sprintf (str, "TWSR: %02X\n\r", TWSR);
//	usart_write (str);
}

int32_t get_pressure (void)
{	
	char str[80];
//	usart_write ("//START OF RESET SENSOR SEQUENCE\n\r");
//	  usart_write ("//SEND RESET COMMAND\n\r");
	twi (START, NIL, NULL, TW_START);
	twi (SEND_SLA, SLA_W, NULL, TW_MT_SLA_ACK);
	twi (SEND_COMMAND, RESET, NULL, TW_MT_DATA_ACK);
	twi (STOP, NIL, NULL, TW_NO_INFO);

	_delay_ms (10);
//	usart_write ("//END OF RESET SENSOR SEQUENCE\n\r");
	

//	usart_write ("//START OF READ PROM SEQUENCE\n\r");
	uint16_t prom[PROM_SIZE];
	uint8_t data_buffer;
	for (int i = 0; i < PROM_SIZE; i++) {
//		usart_write ("//SEND READ COMMAND\n\r");
		twi (START, NIL, NULL, TW_START);
		twi (SEND_SLA, SLA_W, NULL, TW_MT_SLA_ACK);
		twi (SEND_COMMAND, READ_PROM + 2*i, NULL, TW_MT_DATA_ACK);
		twi (STOP, NIL, NULL, TW_NO_INFO);
//		usart_write ("//RECIEVE PROM DATA\n\r");
		twi (START, NIL, NULL, TW_START);
		twi (SEND_SLA, SLA_R, NULL, TW_MR_SLA_ACK);
		twi (RECEIVE_DATA, NIL, &data_buffer, TW_MR_DATA_ACK);
		prom[i] |= (uint16_t) data_buffer << 8;
		twi (RECEIVE_DATA_N, NIL, &data_buffer, TW_MR_DATA_NACK);
		prom[i] |= (uint16_t) data_buffer;
		twi (STOP, NIL, NULL, TW_NO_INFO);
		
		sprintf (str, "prom %d = %u\n\r", i, prom[i]);
//		usart_write (str);
	}
//	usart_write ("//END OF READ PROM SEQUENCE\n\r");
	

//	usart_write ("//START OF READ UNCOMPENSATED PRESSURE SEQUENCE\n\r");
//	 usart_write (" //SEND PRESSURE CONVERSION COMMAND\n\r");
	twi (START, NIL, NULL, TW_START);
	twi (SEND_SLA, SLA_W, NULL, TW_MT_SLA_ACK);
	twi (SEND_COMMAND, PRESSURE_CONVERSION, NULL, TW_MT_DATA_ACK);
	twi (STOP, NIL, NULL, TW_NO_INFO);
	_delay_ms (10);
//	  usart_write ("//SEND READ COMMAND\n\r");
	twi (START, NIL, NULL, TW_START);
	twi (SEND_SLA, SLA_W, NULL, TW_MT_SLA_ACK);
	twi (SEND_COMMAND, READ_ADC, NULL, TW_MT_DATA_ACK);
	twi (STOP, NIL, NULL, TW_NO_INFO);
//	  usart_write ("//RECIEVE UNCOMPENSATED PRESSURE\n\r"); 
	uint32_t D1 = 0;
	uint32_t D2 = 0;
	twi (START, NIL, NULL, TW_START);
	twi (SEND_SLA, SLA_R, NULL, TW_MR_SLA_ACK);
	twi (RECEIVE_DATA, NIL, &data_buffer, TW_MR_DATA_ACK);
	D1 |= (uint32_t) data_buffer << 16;
	twi (RECEIVE_DATA, NIL, &data_buffer, TW_MR_DATA_ACK);
	D1 |= (uint32_t) data_buffer << 8;
	twi (RECEIVE_DATA_N, NIL, &data_buffer, TW_MR_DATA_NACK);
	D1 |= (uint32_t) data_buffer;
	twi (STOP, NIL, NULL, TW_NO_INFO);
//	usart_write ("//END OF READ UNCOMPENSATED PRESSURE SEQUENCE\n\r");
	
//	usart_write ("//START OF READ UNCOMPENSATED TEMPERATURE SEQUENCE\n\r");
//	  usart_write ("//SEND TEMPERATURE CONVERSION COMMAND\n\r");
	twi (START, NIL, NULL, TW_START);
	twi (SEND_SLA, SLA_W, NULL, TW_MT_SLA_ACK);
	twi (SEND_COMMAND, TEMPERATURE_CONVERSION, NULL, TW_MT_DATA_ACK);
	twi (STOP, NIL, NULL, TW_NO_INFO);

	_delay_ms (10);

//	 usart_write (" //SEND READ COMMAND\n\r");
	twi (START, NIL, NULL, TW_START);
	twi (SEND_SLA, SLA_W, NULL, TW_MT_SLA_ACK);
	twi (SEND_COMMAND, READ_ADC, NULL, TW_MT_DATA_ACK);
	twi (STOP, NIL, NULL, TW_NO_INFO);
	
//	  usart_write ("//READ UNCOMPENSATED TEMPERATURE\n\r"); 
	twi (START, NIL, NULL, TW_START);
	twi (SEND_SLA, SLA_R, NULL, TW_MR_SLA_ACK);
	twi (RECEIVE_DATA, NIL, &data_buffer, TW_MR_DATA_ACK);
	D2  = (uint32_t) data_buffer << 16;
	twi (RECEIVE_DATA, NIL, &data_buffer, TW_MR_DATA_ACK);
	D2  = (uint32_t) data_buffer << 8;
	twi (RECEIVE_DATA_N, NIL, &data_buffer, TW_MR_DATA_NACK);
	D2  = (uint32_t) data_buffer;
	twi (STOP, NIL, NULL, TW_NO_INFO);
//	usart_write ("//END OF READ UNCOMPENSATED TEMPERATURE SEQUENCE\n\r");
	
//	usart_write ("//FIRST ORDER COMPENSATION SEQUENCE\n\r");
	int32_t dT = D2 - (uint32_t) prom[C5]*(1 << 8);
	int32_t TEMP = 2000l + (int64_t) dT*prom[C6]/((uint32_t) 1 << 23);
	int64_t OFF =  (int64_t) prom[C2]*((int64_t) 1 << 17)
		     + (int64_t) prom[C4]*dT/((int64_t) 1 << 6);
	int64_t SENS = (int64_t) prom[C1]*((int64_t) 1 << 16)
		     + (int64_t) prom[C3]*dT/((int64_t) 1 << 7);
	int32_t P = (D1*SENS/((int64_t) 1 << 21) - OFF)/((int64_t) 1 << 15);
	sprintf (str, "D1: %lu\n\r", D1);
	usart_write (str);
	sprintf (str, "D2: %lu\n\r", D2);
	usart_write (str);
	sprintf (str, "dT: %lu\n\r", dT);
	usart_write (str);
	sprintf (str, "TEMP: %lu\n\r", TEMP);
	usart_write (str);
	sprintf (str, "OFF: %lu\n\r", OFF);
	usart_write (str);
	sprintf (str, "SENS: %lu\n\r", SENS);
	usart_write (str);
	sprintf (str, "P: %lu\n\r", P);
	usart_write (str);
	return P;
}



