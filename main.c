#define F_CPU 	20000000UL

#include "serial_io.h"

#include <avr/pgmspace.h>
#include <util/delay.h>
#include "servo.h"
#include "user_io.h"


int main(void)
{
	
	init_all();
	
	
	printf("\n\nSERVO CONTROLLER V1.0\n\n'Enter' for Menu\n");
		

	
	while(1) {
		
		int input = wait_for_input();
		if(input == SERVO_MENU)
			servo_menu();
		if(input == SERVO_CMD)
			command_mode();
	}

	return 0;
}

void init_all(void)
{
	init_serial();

	init_servos();
}


