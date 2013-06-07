#include "serial_io.h"


int init_serial(void)
{
	//Init Serial. UBRRH/L Values are caluculated by setbaud.h
	// and the BAUD #defined at the top as well as the F_CPU
	// to give me the proper register values.
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	UCSR0B = ENABLE_TX | ENABLE_RX;
	UCSR0C = SERIAL_MODE_8_BIT;
	
	//Init Port D. First two pins for serial IO,
	// Second to pins for remaining two servo signals.
	DDRD = 0x0E;

	//This sets the stdout and stdin to my serial io functions
	// so I can use stdio functions such as printf.
	stdout = fdevopen(print_char, read_char);

	return 1;

}

//This is the output function for stdlib (stdout)
int print_char(char x, FILE* stream)
{
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = x;
	
	return 0;
}
//This is the input function for stdlib (stdin)
int read_char(FILE* stream)
{
	while(!(UCSR0A & (1<<RXC0)));
		return UDR0;
}

//This function uses fgetc and will only allow numeric digits as input
// up to length digits. Returns length of digits read. Sets value to 
// numeric value if input. (Up to 10 digits)
int read_int(int length, int* value) {
	
	char digits[11]	= {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
	int read	= 0;
	char in		= 0;
	if(length > 10) {
		length = 10;	
	}
	while(in != '\r' && in != '\n') {		
		in = fgetc(stdin);
		
		if(read < length && in >= '0' && in <= '9') {
			digits[read] = in;
			fputc(in, stdout);
			read++;
		}
		if(in == '\b' && read > 0) {
			read--;
			digits[read] = '\0';
			fputc('\b', stdout);
			fputc(' ', stdout);
			fputc('\b', stdout);
		
		}
	}

	*value = atoi(digits);
	return strlen(digits);
}



