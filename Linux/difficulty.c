#include "difficulty.h"

extern int gameIsRunning;
extern int difficulty_level;
extern int ser_fd;

void difficultyTask() {
    
    /*Send '+' on UART periodicly according to difficulty*/
    while(1) {
        if(gameIsRunning) {
            switch(difficulty_level) {
                case(1):
                    sleep(1);
                    break;
            
                case(2):
                    write(ser_fd,"+",1);
                    sleep(30);
                    break;
                
                case(3):
                    write(ser_fd,"+",1);
                    sleep(10);
                    break;
                
                default:
                    sleep(1);
                    break;
            }
        }
        else {sleep(1);}
    }
}
