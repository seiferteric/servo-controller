#define SERVO_MENU	1
#define SERVO_CMD	2
#define CMD_FAIL	0
#define CMD_SUCCESS	1

int wait_for_input(void);
void servo_menu(void);
void command_mode(void);
