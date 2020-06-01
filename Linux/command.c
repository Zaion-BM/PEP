#include "command.h"

#define CFGSTR_SIZE 64

struct BNUM_speed g_speed[] = {
	{ 50 , B50 },
    { 75 , B75 },
	{ 110 , B110 },
	{ 134 , B134 },
	{ 150 , B150 },
	{ 200 , B200 },
	{ 300 , B300 },
	{ 600 , B600 },
	{ 1200 , B1200 },
	{ 1800 , B1800 },
	{ 2400 , B2400 },
	{ 4800 , B4800 },
	{ 9600 , B9600 },
	{ 19200 , B19200 },
	{ 38400 , B38400 },
	{ 57600 , B57600 },
	{ 115200, B115200 },
	{ 230400 , B230400 },
	{ 460800 , B460800 },
	{ 500000 , B500000 },
	{ 576000 , B576000 },
	{ 921600 , B921600 },
	{ 1000000 , B1000000 },
	{ 1152000 , B1152000 },
	{ 1500000 , B1500000 },
	{ 2000000 , B2000000 },
	{ 2500000 , B2500000 },
	{ 3000000 , B3000000 },
	{ 3500000 , B3500000 },
	{ 4000000 , B4000000 },
	{ 0 , 0 } // Null termination, keep as the last element...
};

void command_interpreter(int argc, char*const* argv, uint32_t* baudrate, char *portname) {

    int opt = 0; //cmd line variable
    int speed; //UART speed

    while((opt = getopt(argc, argv, "hs:p:")) != -1) { //Get cmd line argument and handle it
		switch(opt) {
			case 'h': //print help and exit
				print_help(); 
				exit(EXIT_SUCCESS);
				break;
			
			case 's': //configure baudrate
				    speed = CheckSpeed(atoi(optarg));
				    if(speed == 0) {
				        printf("ERROR: Specified serial speed is not supported by termios! \n");
				        exit(EXIT_FAILURE);
					}
					else {
					    printf("Serial port speed: %d\n", atoi(optarg));
					}
					
					*baudrate = speed;
				    break;			
			
			case 'p': //configure port name
			    if((int)strlen(optarg) > CFGSTR_SIZE) {
			        printf("port: %s, long: %d\n", optarg, (int)strlen(optarg));
			        printf("ERROR: Specified serial port name is too long!	\n"	);
			        exit(EXIT_FAILURE);
			    }
			    else {
			        strcpy(portname, optarg);
			        printf("Serial port name: %s \n", portname);
			    }
			    break;
	    }
    }
}
	
	
int64_t CheckSpeed(unsigned speed) { //Check cmd line argument for valid UART speed
    int i;
    
    for (i = 0; g_speed[i].speed != speed; i++) {
        if (g_speed[i].speed == 0) {
            return 0;
        }
    }
    
    return g_speed[i].bnum;
}

void print_help(void) { //Help function

    printf("Banana application help: \n");
    printf("-h: Printig application help. \n");
    printf("-s <baudrate>: Setting serial port baudrate (speed). \n");
    printf("-p <port name>: Setting serial port port name. \n\n");
    printf("Game starts by pressing 's'.\n");
    printf("To navigate the basket use the 'd' for right direction and 'a' for left direction.\n"); 
    
}
