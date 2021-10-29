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

#define MS5387_addr 0x76

#define OC0A DDD6
#define LOW_BATTERY_THRESHOLD 10
#define ADC_BIT_WIDTH 1024 // 2^10, 10 bits
#define SDA DDC4
#define SCL DDC5
typedef union ADC_val {
	struct {
		uint8_t low  : 8;
		uint8_t high : 2;
	} byte;
	uint16_t jerry;	
} ADC_val;

typedef union sensor_data {
	struct {
		uint8_t low  : 8;
		uint8_t mid  : 8;
		uint8_t high : 8;
	} byte;
	uint16_t mason;	
} sensor_data;

void pressure_sensor_init (void);
void battery_init (void);

bool twi_send_start (void);
bool twi_send_addr_r (void);
bool twi_send_addr_w (void);
bool twi_send_command (uint8_t command);
bool twi_recieve_adc (void);
void twi_send_stop (void);
bool validate_status (uint8_t status);

int main (void)
{
	battery_init ();
	pressure_sensor_init ();
	sei ();
	while (true) {
	}
	return 0;
}

void battery_init (void)
{
	DDRD   |= _BV (OC0A); // output compare pin set to write
	TIMSK0 |= _BV (OCIE0A);
	TCCR0A  = _BV (COM0A1) | _BV (COM0A0) | _BV (WGM01); //sets PD6 HIGH on OCR0A match
	TCCR0B  = _BV (CS00); // no prescaler
	OCR0A   = 255; //random number for now, to be updated with formula based on F_CPU and polling rate for batter

	ADMUX   = _BV (MUX1); //ADC2
	ADCSRA  = _BV (ADEN)  | _BV (ADIE) | _BV (ADATE); //adc interrupt on trigger
	ADCSRB  = _BV (ADTS1) | _BV (ADTS0);	//adc trigger being OCR0A match
}

void pressure_sensor_init (void)
{
	TWBR   =  0x48;	 // pg 221 gves the formula for this, 100 kHz supposedly
	TWSR  &= ~_BV (TWPS1) | ~_BV (TWPS0); //prescale of 1
	TWCR  |=  _BV (TWEA)  |  _BV (TWEN);
	DDRC  |=  _BV (SDA)   |  _BV (SCL); //set sda and scl pins to write
	TIMSK2 =  _BV (OCIE2A); //output compare interrupt enabled
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
	return validate_status(TW_START);
}
/*
 * alerts slave that we interested in it
 */
bool twi_send_addr_r (void)
{
	TWDR  = (MS5387_addr << 1) | TW_READ;
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return validate_status(TW_MR_SLA_ACK);
}

bool twi_send_addr_w (void)
{
	TWDR  = (MS5387_addr << 1) | TW_WRITE;
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return validate_status(TW_MT_SLA_ACK);
}
bool twi_send_command (uint8_t command)
{
	TWDR  = command;
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return validate_status(TW_MT_DATA_ACK);
}

bool twi_recieve_adc (void)
{
	TWCR |= _BV (TWINT);
	while (!(TWCR & _BV (TWINT)));
	return validate_status(TW_MR_DATA_ACK);
}
void twi_send_stop (void)
{
	TWCR = _BV (TWINT) | _BV (TWEN) | _BV (TWSTO);
	while (!(TWCR & _BV (TWINT)));
	//no stop code for MR in twi.h for some reason
}

bool validate_status (uint8_t status)
{
	if (TW_STATUS != status) { //checks TWSR 7:3
		return false;
	} else {
		return true;
	}
}

ISR (ADC_vect) //triggers when ADC conversion is complete which starts when compare match occurs on OCR0A
{
	ADC_val levels;
	levels.byte.low  = ADCL;
	levels.byte.high = ADCH;
	if (levels.jerry <= LOW_BATTERY_THRESHOLD) {
		//tell motor to surface
		//maybe set LEDs (red, yellow, green) for status?
		//trap programme in forever loop so nothing else happens?
		while(true);

	}
}


//this wont work properly because there are some things i'm not seeing. more at 11'
