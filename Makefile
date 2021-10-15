MCU=m328p
PROGRAMMER=avr109
F_CPU=20000000L
BAUD=9600
PORT=/dev/ttyACM0
CC=avr-g++
OBJCOPY=avr-objcopy
CFLAGS=-Wall -Wextra -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=main
SRCS=main.c


all:
	${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:	
	avrdude -v -p ${MCU} -c ${PROGRAMMER} -U flash:w:${TARGET}.hex:i -P ${PORT} -D

clean:
	rm -f *.bin *.hex *.o
