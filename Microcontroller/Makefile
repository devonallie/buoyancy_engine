MCU=atmega328p
PROGRAMMER=arduino
BAUD=57600
F_CPU=16000000UL
PORT=/dev/cu.usbserial-110
CC=avr-g++
OBJCOPY=avr-objcopy
CFLAGS=-Wall -Wextra -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main
SRCS=main.c


all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:	
	avrdude -v -p ${MCU} -c ${PROGRAMMER} -b ${BAUD} -U flash:w:${TARGET}.hex:i -P ${PORT} -D

clean:
	rm -f *.bin *.hex *.o
