#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <termios.h>

#include "command.h"
#include "game.h"
#include "highscores.h"
#include "mapgen.h"
#include "difficulty.h"

#define CMDLINE_DBG true

#define CFGSTR_SIZE 64

// Global variables
int gameIsRunning = 0;
int ser_fd;

int main(int argc, char *argv[]) {
    
    uint32_t baudrate = 0;
    char     portname [CFGSTR_SIZE+1] = "";
    
    command_interpreter(argc, argv, &baudrate, portname); //call for cmd line argument interpreter
   
   /*Initialize serial port and open it*/
    struct termios gecko_ser; 
   
    memset(&gecko_ser,0,sizeof(gecko_ser));
    
    gecko_ser.c_iflag = 0;
    gecko_ser.c_oflag = 0;
    gecko_ser.c_cflag = CS8|CREAD|CLOCAL;
    gecko_ser.c_lflag = 0;
    gecko_ser.c_cc[VMIN] = 1;
    gecko_ser.c_cc[VTIME] = 0;
    
    ser_fd = open(portname,O_RDWR);
 
    if(ser_fd < 0 ) {
        perror("Serial device open failed!");
        exit(EXIT_FAILURE);
    }
    
    cfsetospeed(&gecko_ser, baudrate);
    cfsetispeed(&gecko_ser, baudrate);
	
	tcsetattr(ser_fd,TCSANOW,&gecko_ser);
    
    int status; //Return value for thread creation
    
    /*Thread ID variables*/
    pthread_t gameTask_ID;
    pthread_t mapgenTask_ID;
    pthread_t difficultyTask_ID;

    /*Create threads*/
    
    status = pthread_create(&gameTask_ID, NULL, (void*)gameTask, NULL);
    
    status = pthread_create(&mapgenTask_ID, NULL, (void*)mapgenTask, NULL);
    
    status = pthread_create(&difficultyTask_ID, NULL, (void*)difficultyTask, NULL);

    /*Terminate only if all threads have already terminated*/
    pthread_join(gameTask_ID, NULL);
    
    pthread_join(mapgenTask_ID, NULL);
    
    pthread_join(difficultyTask_ID, NULL);

	exit(EXIT_SUCCESS);
}
