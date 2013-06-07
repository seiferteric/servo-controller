

/*
*
*	Serial #defines, structures and function declarations
*
*/

//Serial IO Definitions
#define ENABLE_TX		0x08
#define ENABLE_RX		0x10
#define SERIAL_MODE_8_BIT	0x06
#define F_CPU 			20000000UL
#define 			BAUD 115200


#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <util/setbaud.h>


//Serial IO Function Declarations
int print_char(char, FILE*);
int read_char(FILE*);
void init_all(void);
int read_int(int, int*);
int init_serial(void);
