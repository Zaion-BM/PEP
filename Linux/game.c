#include "game.h"
#include "highscores.h"
#include "mapgen.h"

extern int gameIsRunning; //Boolean like variable
int difficulty_level; // 1,2,3 or 0 if no parameter was given from STDIN
int score = 0; //Player score during game

extern int ser_fd; //Externally initiated UART handle
extern double avg_low,avg_mid,avg_high; // 0 at first attempt
extern int high_score_low, high_score_mid, high_score_high; // 0 at first attempt

void gameOver() {
    open_file();        //Get all data from database
    gameIsRunning = 0;  //Stop game running in other tasks

    /*According to the chosen level, set avg and high score and print result to the player*/ 
    switch(difficulty_level) {
        case(1):
            if(high_score_low <= score) {
            printf("Congrats! New High Score!!!!44!\n");
            high_score_low = score;
            }
            avg_low = (avg_low+score)/2;
            
            printf("Score: %d\nAverage Score: %f\nHigh Score: %d\n\n", score, avg_low, high_score_low);
            write_new_result();
            break;

        case(2):
            if(high_score_mid <= score) {
            printf("Congrats! New High Score!!!!44!\n");
            high_score_mid = score;
            } 
            avg_mid = (avg_mid+score)/2;
            
            printf("Score: %d\nAverage Score: %f\nHigh Score: %d\n\n", score, avg_mid, high_score_mid);
            write_new_result();
            break;

        case(3):
            if(high_score_high <= score) {
            printf("Congrats! New High Score!!!!44!\n");
            high_score_high = score;
            }
            avg_high = (avg_high+score)/2;
            
            printf("Score: %d\nAverage Score: %f\nHigh Score: %d\n\n", score, avg_high, high_score_high);
            write_new_result();
            break;
        
        default: write_new_result();
    }
   
   /*Print first message to player, the next game starts here*/
    printf("Please select game difficulty: \n"
    "\t\t\t Easy   - press e\n"
    "\t\t\t Normal - press n\n"
    "\t\t\t Hard   - press h\n");    
    
    //Set game variables to default
    score = 0;
    difficulty_level = 0;
    init_map(); // get a new map
}

void gameTask() {
	//Control variables
	char c = 0;
	struct termios input;
	
	// Disabling canonical mode and echo        
    tcgetattr(STDIN_FILENO, &input);
        
    input.c_lflag &= ~ICANON;
    input.c_lflag &= ~ECHO;
	        
    if(tcsetattr(STDIN_FILENO, TCSANOW, &input) == -1) {
        perror("ERROR");
    }
    
    //Start the game and print first message
    printf("Please select game difficulty: \n"
    "\t\t\t Easy   - press e\n"
    "\t\t\t Normal - press n\n"
    "\t\t\t Hard   - press h\n");  
	
	while(1) {
        /*Setting up select function to get user input*/
        int selrval;
        fd_set rfds;
        struct timeval tv;
        
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(ser_fd, &rfds);
        
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        selrval = select(ser_fd+1, &rfds, NULL, NULL, &tv); 
        
        if(selrval < 0) {
            perror("Select failed");
        }
        
        else if(selrval == 0) {
            //printf("Select timeout...\n");
        }
        
        else {
            if(FD_ISSET(STDIN_FILENO, &rfds)) { //Handle if a character is read from STDIN
                read(STDIN_FILENO, &c, 1);
                switch (c) {
                    case ('e'):
                        difficulty_level = 1;
                        printf("Difficulty level Easy selected.\nPress 's' to start.\n");
                        break;
                        
                    case ('n'):
                        difficulty_level = 2;
                        printf("Difficulty level Normal selected.\nPress 's' to start.\n");
                        break;
                        
                    case ('h'):
                        difficulty_level = 3;
                        printf("Difficulty level Hard selected.\nPress 's' to start.\n");
                        break;
                        
                    case ('s'):
                        write(ser_fd,&c,1);
                        gameIsRunning = 1;
                        break;
                    
                    default: 
                        write(ser_fd,&c,1);
                        break;
                }
            }
            
            
            if(FD_ISSET(ser_fd, & rfds)) { //Handle if a character is read from UART
                read(ser_fd, &c, 1);
                
                if(score == 0) { //set score
                    score = atoi(&c);
                }
                else {
                    score *= 10; 
                    score += atoi(&c);
                    gameOver(); //Game is over here
                }
            }
	    }
	}
	
	close(ser_fd); //Close serialport
	
    // Re-enabling canonical mode and echo
    input.c_lflag |= ICANON;
    input.c_lflag |= ECHO;

    if(tcsetattr(STDIN_FILENO, TCSANOW, &input) == -1) {
        perror("ERROR");
    }
    
    exit(EXIT_SUCCESS);
}
