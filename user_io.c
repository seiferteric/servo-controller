#include <stdio.h>
#include "user_io.h"
#include "serial_io.h"
#include "servo.h"

void set_servo_menu(void);
void get_servo_menu(void);
void reset_servos(void);
void set_all_servos_menu(void);
void send_response(int);

// Waits for specific input, "Enter" for user menu mode, or ">" for 
// command mode.
int wait_for_input(void)
{
	while(1) {
		char in = fgetc(stdin);
		if(in == '\n' || in == '\r')
			return SERVO_MENU;
		if(in == '>')
			return SERVO_CMD;
	}
	return 0;
}

// Display the menu asking what the user would like to do and do it
void servo_menu(void)
{
	int choice = 0;

	while(!choice) {
		printf("\n\n~~SERVO CONTROLLER MENU~~\n\n1) Set Servo Position\n2) Set All Servos\n3) Reset All Servos\n4) Read Current Positions\n5) Exit\n\nEnter Choice: ");
		read_int(1, &choice);
		if(choice > 5 || choice < 1)
			choice = 0;
	}
	switch(choice) {
		case 1:
			set_servo_menu();
			break;
		case 2:
			set_all_servos_menu();
			break;
		case 3:
			reset_servos();
			break;
		case 4:
			get_servo_menu();
			break;
		default:
			break;
	}
	fputc('\n', stdout);
}

// Set individual servo PWM Duty Cycle
void set_servo_menu(void)
{
	int choice = 0;
	int servo_value = 0;
	while(!choice) {
		printf("\n\nWhat Servo whould you like to set?\n\n1) Servo 1\n2) Servo 2\n3) Servo 3\n4) Servo 4\n5) Servo 5\n6) Servo 6\n7) Servo 7\n8) Servo 8\n\nEnter Choice: ");
		read_int(1, &choice);
		if(choice > 8 || choice < 1)
			choice = 0;
	}

	while(!servo_value) {
		printf("\nSet servo value (%d-%d in 0.4us increments)\n\nValue: ", PWM_MIN, PWM_MAX);
		read_int(4, &servo_value);
		if(servo_value > PWM_MAX || servo_value < PWM_MIN)
			servo_value = 0;
	}
	
	set_servo(choice - 1, servo_value);
}

// Sets all servos to a common value
void set_all_servos_menu(void)
{
	int servo_value = 0;
	while(!servo_value) {
		printf("\nSet servo value (%d-%d in 0.4us increments)\n\nValue: ", PWM_MIN, PWM_MAX);
		read_int(4, &servo_value);
		if(servo_value > PWM_MAX || servo_value < PWM_MIN)
			servo_value = 0;
	}
	
	
	for(int i=0;i<8;i++) {
		set_servo(i, servo_value);
	}
}

// Read servos current pwm duty cycle and print them
void get_servo_menu(void)
{
	printf("\n\nCurrent Servo Duty Cycle:\n\n");
	for(int i=0;i<8;i++) {
		printf("%d) %d\n", i+1, get_servo(i));
	}

}

// Sets all servos to the default value
void reset_servos(void)
{
	for(int i=0;i<8;i++) {
		set_servo(i, PWM_DEFAULT);
	}
}

// Command mode is usefull if sending commands from a PC
// fewer characters need to be sent. Format is this:
// >(command number)[servo number][value 1]...[value 8]
// Can get and set values.
void command_mode(void)
{
	
	int command	= read_char(stdin);
	int servo	= 0;
	char svalue[5]	= {'\0','\0','\0','\0', '\0'};
	int value	= 0;


	switch(command) {
		case 'A':		//Set Servo Command
			servo = read_char(stdin);
			servo -= '1';
			for(int i=0;i<4;i++)	
				svalue[i] = read_char(stdin);
			value = atoi(svalue);
			if(!set_servo(servo, value)){
				send_response(CMD_FAIL);
			}else
				send_response(CMD_SUCCESS);
			break;		
		case 'B':		//Get Servo Command
			servo = read_char(stdin);
			servo -= '1';
			value = get_servo(servo);				
			if(value == -1) {
				send_response(CMD_FAIL);
			} else {
				printf("%d\n", value);
				send_response(CMD_SUCCESS);
			}
			break;
		case 'C':		//Set ALL Servos Command
			for(int i=0;i<4;i++)	
				svalue[i] = read_char(stdin);
			value = atoi(svalue);
			for(int i=0;i<8;i++) {
				if(!set_servo(i, value)){
					send_response(CMD_FAIL);
					break;
				}
				if(i==3)
					send_response(CMD_SUCCESS);
			}
			break;
		case 'D':		//Get ALL Servos
			for(int i=0;i<8;i++) {
				value = get_servo(i);
				printf("%d\n", value);
			}
			send_response(CMD_SUCCESS);
			break;
		case 'E':		//Reset All Servos
			for(int i=0;i<8;i++) {
				set_servo(i, PWM_DEFAULT);
			}
			send_response(CMD_SUCCESS);
			break;
		default:
			send_response(CMD_FAIL);
			break;	
	}

}

void send_response(int response)
{
	fputc(response, stdout);
}
