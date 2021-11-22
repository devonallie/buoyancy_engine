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

#define MS5387 0x76
#define TWDR_SLAVE_ADDRESS (MS5387 << 1)
#define SCL_FREQ_MAX 400000
#define SCL_FREQ SCL_FREQ_MAX
#define F_CPU 16000000UL
#define SCL_PRESCALE 1
#define TWI_BAUD  ((F_CPU/SCL_FREQ) - 16)/2/SCL_PRESCALE
#define OCR2A_MAX 255

	char str_test[80];
typedef union twi_data {
	struct {
		uint8_t first_byte  : 8;
		uint8_t second_byte : 8;
		uint8_t third_byte  : 8;
	} bytewise;
	uint32_t contiguous;	
} twi_data;


bool twi_send_start (void);
bool twi_send_addr_r (void);
bool twi_send_addr_w (void);
bool twi_send_command (uint8_t command);
bool twi_recieve_adc (uint32_t *adc_buffer);
bool twi_send_stop (void);
bool twi_receive_prom (uint16_t *prom_buffer);
bool twi_validate_status (uint8_t status);
void twi_disable (void);



void twi_init (void)
{	
	PRR   &= ~_BV (PRTWI); //powers on twi
	TWBR   =   TWI_BAUD;	 // pg 221 gves the formula for this, 100 kHz supposedly
	TWSR  &= ~_BV (TWPS1) | ~_BV (TWPS0); //prescale of 1
	TWCR  |=  _BV (TWEA)  |  _BV (TWEN);//enables twi, send ACKs
	//15ms max for adc conversion
}
/*
 * starts the twi line
 */
bool twi_send_start (void)
{
	TWCR |= _BV (TWINT) | _BV (TWSTA);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status (TW_START);
}
/*
 * alerts slave that we interested in it
 */
bool twi_send_addr_r (void)
{
	TWDR  = TWDR_SLAVE_ADDRESS | TW_READ;
	TWCR &= ~_BV (TWSTA);
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status (TW_MR_SLA_ACK);
}

bool twi_send_addr_w (void)
{
	TWDR  = TWDR_SLAVE_ADDRESS | TW_WRITE;
	TWCR &= ~_BV (TWSTA);
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status (TW_MT_SLA_ACK);
}
bool twi_send_command (uint8_t command)
{
	TWDR  = command;
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status (TW_MT_DATA_ACK);
}

bool twi_receive_adc (uint32_t *adc_buffer)
{
	TWCR &= ~_BV (TWSTA);
	TWCR |= _BV (TWINT) | _BV (TWEA);
	while (!(TWCR & _BV (TWINT)));
	*adc_buffer |= (uint32_t) TWDR << 16;
	
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	*adc_buffer |= (uint32_t) TWDR << 8;

	TWCR &= ~_BV (TWEA);
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	*adc_buffer |= (uint32_t) TWDR;

	return twi_validate_status (TW_MR_DATA_NACK);
}
bool twi_receive_prom (uint16_t *prom_buffer)
{
	TWCR &= ~_BV (TWSTA);
	TWCR |= _BV (TWINT) | _BV (TWEA);
	while (!(TWCR & _BV (TWINT)));
	*prom_buffer |= (uint16_t) TWDR << 8;
	TWCR &= ~_BV (TWEA);
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	*prom_buffer |= (uint16_t) TWDR;

	return twi_validate_status (TW_MR_DATA_NACK);
}


bool twi_send_stop (void)
{
	TWCR |= _BV (TWINT) | _BV (TWSTO);
	while (!(TWCR & _BV (TWSTO)));
	return twi_validate_status (TW_NO_INFO); //*shug emoji*
}

bool twi_validate_status (uint8_t status)
{
	if (TW_STATUS != status) { //checks TWSR 7:3
		return false;
	} else {
		return true;
	}
}

void twi_disable (void)
{
	PRR |= PRTWI;
}

