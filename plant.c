#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h> 
#include <unistd.h>
#include <fcntl.h>

#define MAX_BLUE 15
#define MAX_RED 10  

int main(){


FILE *railway;
FILE *blueDelivery;
FILE *redDelivery;

char *railFile="railwayCars.txt";
char *blueFile="BlueDelivery.txt";
char *redFile="RedDelivery.txt";

 if((railway=fopen(railFile,"r"))==NULL){
    perror(Filename);
    exit(1);
 }

 if(blueDelivery=fopen(blueFile,"w")==NULL){
    perror(blueFile);
    exit(1);

 }

if(redDelivery=fopen(redFile,"w")==NULL){
    perror(redFile);
    exit(1);
}



int partsforA(int num){
if(num>=1 && num<=12){
    return 1;
}
return 0;


}



int partsforB(int num){
    if(num>=13 && num<=25){

        return 1;
    }
    return 0;
    
}





int pid=fork();


if(pid==0){
    int threadX=1
    int threadY=2

int bufferBlue[15];
int bufferRed[10];

int Blue_fill_ptr = 0;
int Blue_use_ptr = 0;
int Blue_count = 0;

int Red_fill_ptr = 0;
int Red_use_ptr = 0;
int Red_count = 0;


int sequence=0;
int part;

void put(int value) {

   if((collectedPart=fscanf(railway, "%d ", &part)) !=EOF){
    

    if(partsforA){
        bufferBlue[Blue_fill_ptr]=collectedPart;
        Blue_fill_ptr=(Blue_fill_ptr+1)%MAX_BLUE;
    


    }


    else if( partsforB){

        bufferRed[Blue_fill_ptr]=collectedPart;
        Red_fill_ptr=(Red_fill_ptr+1)%MAX_RED;


    }    

    else{

        printf("%s","wrong parts");
        exit(1);
    } 






   }



buffer[fill_ptr] = value;
fill_ptr = (fill_ptr + 1) % MAX;
count++;
}

int getBlue(int isthread) {

    if(isthread==1){

    int Bluevalue=bufferBlue[Blue_use_ptr];    
    fprintf(blueFile,"%d ", Bluevalue);
    use_ptr = (Blue_use_pt + 1) % MAX_BLUE;
    printf("GET BLUE VALUE: %d", Bluevalue);

    }

    else if(isthread==2){

    int Redvalue=bufferBlue[Red_use_ptr];    
    fprintf(redFile,"%d ", Redvalue);
    use_ptr = (Red_use_ptr + 1) % MAX_RED;
    printf("GET RED VALUE: %d", Redvalue);    


    }

}




int getRed(){




}









//child process



}


else if(pid>0){

//parent



}




else{

perror();

}














    return 0;
}