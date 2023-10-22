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
    int Red_fill_ptr = 0;
    int Red_use_ptr = 0;

    int count=0;
    int sequence=0;
    int part;

void put() {

   if((collectedPart=fscanf(railway, "%d ", &part)) !=EOF){
    

    if(partsforA){
        bufferBlue[Blue_fill_ptr]=collectedPart;
        Blue_fill_ptr=(Blue_fill_ptr+1)%MAX_BLUE;
        count++;
        


    }


    else if( partsforB){

        bufferRed[Blue_fill_ptr]=collectedPart;
        Red_fill_ptr=(Red_fill_ptr+1)%MAX_RED;
        count++;


    }    

    else{

        printf("%s","wrong parts");
        exit(1);
    } 






   }

   bufferBlue[Blue_fill_ptr]=-1;
   bufferRed[Red_fill_ptr]=-1;



}

int get(int isthread) {

    if(isthread==1){

    int Bluevalue=bufferBlue[Blue_use_ptr];    
    fprintf(blueFile,"%d", Bluevalue);
    fprintf(blueFile,"%d", sequence);
    printf("GET BLUE VALUE: %d  SEQUENCE #: %d\n\n", Bluevalue,sequence);
    use_ptr = (Blue_use_pt + 1) % MAX_BLUE;
    count--;
    

    }

    else if(isthread==2){

    int Redvalue=bufferRed[Red_use_ptr];    
    fprintf(redFile,"%d", Redvalue);
    fprintf(redFile,"%d", sequence);
    printf("GET RED VALUE: %d  SEQUENCE #: %d\n\n", Redvalue,sequence);
    use_ptr = (Red_use_ptr + 1) % MAX_RED;
    count--;

    }

}


pthread_t mutex;
pthread_cond_t empty,full;

int rc=pthread_mutex_init(&mutex,NULL);
assert(rc==-0);

int erc=pthread_cond_init(&empty,NULL);
assert(erc==0);
int crc=pthread_cond_init(&full,NULL);
assert(crc==0);



void Pthread_lock( pthread_t *lock){
    int rc= pthread_lock(lock);
    assert(rc==0);


}

void Pthread_unlock(pthread_t *lock){
    int rc=pthread_unlock(pthread_t lock);
    assert(rc==0);


}



void *Producer(void *args){

    int MaxbufferElement;


    if( *isthread==threadX){

        fill_ptr=Blue_fill_ptr;
        MaxbufferElement=MAX_BLUE-1;



    }

    if(*isthread==threadY){

        fill_ptr=Red_fill_ptr;
        MaxbufferElement=MAX_RED-1;

    }


    Pthread_lock(&mutex);

    while((count==MAX_BLUE-1)||(count==MAX_RED-1)){
        phtread_cond_wait(&empty,&mutex);

    }
    put(*isthread);
    sequence++;
    pthread_cond_signal(&full,&mutex);
    Pthread_unlock(&mutex);





}










void *Consumer(void *args){
    int *isthread;
    int fill_ptr;
    int MaxbufferElement;


    if( *isthread==threadX){
        isthread=(int *) args;
        



    }

    if(*isthread==threadY){
        isthread=(int *) args;

    }


    Pthread_lock(&mutex);

    while(count==MaxbufferElement){
        phtread_cond_wait(&full,&mutex);

    }
    get(*isthread);
    pthread_cond_signal(&empty,&mutex);

    Pthread_unlock(&mutex);





}





void Producer(void *args){




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