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

#define MS5387 0x76
#define TWDR_SLAVE_ADDRESS (MS5387 << 1)
#define SDA DDC4
#define SCL DDC5

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
bool twi_recieve_adc (void);
void twi_send_stop (void);
bool twi_validate_status (uint8_t status);



void twi_init (void)
{
	TWBR   =  0x48;	 // pg 221 gves the formula for this, 100 kHz supposedly
	TWSR  &= ~_BV (TWPS1) | ~_BV (TWPS0); //prescale of 1
	TWCR  |=  _BV (TWEA)  |  _BV (TWEN);
	DDRC  |=  _BV (SDA)   |  _BV (SCL); //set sda and scl pins to write
	TIMSK2 =  _BV (OCIE2B); //output compare interrupt enabled
	TCCR2A =  _BV (WGM21); //clear TCNT2 on compare match
	OCR2A  =   255; //random, configure for sensor polling rate
}
/*
 * starts the twi line
 */
bool twi_send_start (void)
{
	TWCR |= _BV (TWINT) | _BV (TWSTA);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status(TW_START);
}
/*
 * alerts slave that we interested in it
 */
bool twi_send_addr_r (void)
{
	TWDR  = TWDR_SLAVE_ADDRESS | TW_READ;
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status(TW_MR_SLA_ACK);
}

bool twi_send_addr_w (void)
{
	TWDR  = TWDR_SLAVE_ADDRESS | TW_WRITE;
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status(TW_MT_SLA_ACK);
}
bool twi_send_command (uint8_t command)
{
	TWDR  = command;
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status(TW_MT_DATA_ACK);
}

bool twi_recieve_adc (void)
{
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return twi_validate_status(TW_MR_DATA_ACK);
}
void twi_send_stop (void)
{
	TWCR = _BV (TWINT) | _BV (TWEN) | _BV (TWSTO);
	while (!(TWCR & _BV (TWINT)));
	//no stop code for MR in twi.h for some reason
}

bool twi_validate_status (uint8_t status)
{
	if (TW_STATUS != status) { //checks TWSR 7:3
		return false;
	} else {
		return true;
	}
}

