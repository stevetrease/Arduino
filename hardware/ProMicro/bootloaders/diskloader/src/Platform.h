/*
	Changes for the SparkFun Pro Micro Bootloader
	* Changed PID and VID values and macro names:
		#define USB_PID_PROMICRO16 0x0002
		#define USB_PID_PROMICRO8 0x0003
		#define USB_VID 0x1B4F
	* Changed OEM_NAME macro
		#define OEM_NAME		'p','r','o','m','i','c','r','o'					// 8 chars
		
- Jim Lindblom; January 24, 2012
*/
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;


#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))
#define DISABLE_JTAG()  MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD); MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);

#define USB_PID_PROMICRO16 0x0002
#define USB_PID_PROMICRO8 0x0003
#define USB_VID 0x1B4F 	// SparkFun Electronics VID
#define USB_PID ARDUINO_MODEL_PID	// passed in by Makefile - 0x0002 for 16MHz Pro Micro, 0x0003 for 8MHz

#define USB_SERIAL_STRING	'0','0','0','0','0','0','0','0','1','7','0','1'

#define OEM_NAME		'p','r','o','m','i','c','r','o'					// 8 chars
#define BOARD_INIT()	DDRC |= (1<<7); DDRB |= (1<<0); DDRD |= (1<<5); CPU_PRESCALE(0); DISABLE_JTAG();
#define LED0			PORTC &= ~(1<<7)
#define LED1			PORTC |= (1<<7)
#define TXLED0			PORTD |= (1<<5)
#define TXLED1			PORTD &= ~(1<<5)
#define RXLED0			PORTB |= (1<<0)
#define RXLED1			PORTB &= ~(1<<0)

#define TRANSFER_PGM		0x80
#define TRANSFER_RELEASE	0x40
#define TRANSFER_ZERO		0x20

void Transfer(u8 ep, const u8* data, int len);
void Recv(u8 ep, u8* dst, u8 len);
void Program(u8 ep, u16 page, u8 count);

#define CDC_ENABLED

#include "USBCore.h"
#include "USBDesc.h"


