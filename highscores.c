#include "highscores.h"

//Global constants 
FILE* fp;
char temp[16];
char data_low[16];  //"low,00.00,00"
char data_mid[16];  //"mid,00.00,00"
char data_high[16]; //"hig,00.00,00"
char toplist[10][16];  //"lvl,00,name"

double avg_low,avg_mid,avg_high; // 0 at first attempt
int high_score_low, high_score_mid, high_score_high; // 0 at first attempt

void parse_file(FILE* fp) {
            int i = 0;
            while(fgets(temp, 16, fp)) {
                if(0==i) {strcpy(data_low,temp);}
                if(1==i) {strcpy(data_mid,temp);}
                if(2==i) {strcpy(data_high,temp);}
                if( (i>2) && (i<10) ) {strcpy(toplist[i],temp);}
                i++;
            }
            
            //Get avg_low
            memset(temp, '\0', sizeof(temp));
            strncpy(temp,data_low+4,5);
            avg_low = atof(temp);
            
            //Get avg_mid
            memset(temp, '\0', sizeof(temp));
            strncpy(temp,data_mid+4,5);
            avg_mid = atof(temp);
            
            //Get avg_high
            memset(temp, '\0', sizeof(temp));
            strncpy(temp,data_high+4,5);
            avg_high = atof(temp);
            
            //Get hs_low
            strncpy(temp,data_low+10,2);
            high_score_low = atoi(temp);
            
            //Get hs_mid
            strncpy(temp,data_mid+10,2);
            high_score_mid = atoi(temp);
            
            //Get hs_high
            strncpy(temp,data_high+10,2);
            high_score_high = atoi(temp);
}

void write_new_result(){
    fp = fopen("datafile.txt", "w"); //Open database
    if (fp == NULL) { //Handle error
        printf("Please call open_file() API function first.\n"); 
        fclose(fp);
        }
    else{ 
        //Write new data_low to file
        strcpy(data_low,"low,");
        sprintf(temp,"%05.02f",avg_low);
        strcat(data_low,temp);
        sprintf(temp, ",%02d", high_score_low);
        strcat(data_low,temp);
        strcat(data_low,"\n");
        fputs(data_low,fp);
        
        //Write new data_mid to file
        strcpy(data_mid,"mid,");
        sprintf(temp,"%05.02f",avg_mid);
        strcat(data_mid,temp);
        sprintf(temp, ",%02d", high_score_mid);
        strcat(data_mid,temp);
        strcat(data_mid,"\n");
        fputs(data_mid,fp);
        
        //Write new data_low to file
        strcpy(data_high,"hig,");
        sprintf(temp,"%05.02f",avg_high);
        strcat(data_high,temp);
        sprintf(temp, ",%02d", high_score_high);
        strcat(data_high,temp);
        strcat(data_high,"\n");
        fputs(data_high,fp);
        
        for(int i = 0; i<10;i++){
            strcpy(toplist[i],"lvl,00,name\n"); //Create toplist element in memory
            fputs(toplist[i],fp);               //Write toplist element to file
            }
         fclose(fp);
    }
    
}

void open_file(){
    fp = fopen("datafile.txt", "r");
        if (fp != NULL){
                parse_file(fp);
                fclose(fp);
            }
        else{
            fp = fopen("datafile.txt", "w"); //Open empty file
            
            strcpy(data_low,"low,00.00,00\n"); //Create data_low in memory
            fputs(data_low,fp);              //Write data_low to file
            
            strcpy(data_mid,"mid,00.00,00\n"); //Create data_mid in memory
            fputs(data_mid,fp);              //Write data_mid to file
            
            strcpy(data_high,"high,00.00,00\n"); //Create data_high in memory
            fputs(data_high,fp);             //Write data_high to file
            
            for(int i = 0; i<10;i++){
                strcpy(toplist[i],"lvl,00,name\n"); //Create toplist element in memory
                fputs(toplist[i],fp);             //Write toplist element to file
            }
            
            /*Give initial value to variables in memory*/
            avg_low = 0;
            avg_mid = 0;
            avg_high = 0;
            high_score_low = 0;
            high_score_mid = 0;
            high_score_high = 0;
            
            fclose(fp);
        }

}
