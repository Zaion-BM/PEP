#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <unistd.h>

#include "mapgen.h"

int map[99]; //Memory space for map
extern int ser_fd; //Externally initiated UART handle

void init_map(){

    srand(time(0)); //seed random time generator
    
    for(int i = 0;i<100;i++){   //fill map array
        map[i] = ( rand() % 4 );    //%4 in order to get 0||1||2||3 numbers only
    }
}

void mapgenTask() {
    
    init_map(); //Create unique map
    int index = 0;
    char ch;
    
    while(1) {
        if(index == 100) {index=0;} //Circular buffer
        ch = map[index]+0x30; //int to char conversion
        write(ser_fd,&ch,1); //Send 1 byte on UART
        index++; //Increment index
        usleep(100000); //Sleep for 100ms
    }
}
