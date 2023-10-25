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
#include <pthread.h>
#include <assert.h>

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
    perror(railFile);
    exit(1);
 }

 if((blueDelivery=fopen(blueFile,"w"))==NULL){
    perror(blueFile);
    exit(1);

 }

if((redDelivery=fopen(redFile,"w"))==NULL){
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
    
  printf("%s\n\n","child process is running...");
  printf("%s","child has finished...");

    int threadX=1;
    int threadY=2;

    int bufferBlue[15];
    int bufferRed[10];

    int Blue_fill_ptr = 0;
    int Blue_use_ptr = 0;
    int Red_fill_ptr = 0;
    int Red_use_ptr = 0;

    int collectedPart;
    int count=0;
    int sequence=0;
    int part;
    int done=0;
    int terminateProducers=0;
    int terminateConsumers=0;

void put() {


   if(( collectedPart=fscanf(railway, "%d ", &part)) !=EOF || done==0){
    

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

    else {

        bufferBlue[Blue_fill_ptr]=-1;
        Blue_fill_ptr=(Blue_fill_ptr+1)%MAX_BLUE;



        bufferRed[Blue_fill_ptr]=-1;
        Red_fill_ptr=(Red_fill_ptr+1)%MAX_RED;
        
        count++;
        terminateProducers++;

    } 

    //else{

        
    //}






   }

   //bufferBlue[Blue_fill_ptr]=-1;
   //bufferRed[Red_fill_ptr]=-1;



}

int get(int isthread) {

    if(isthread==1){

    int Bluevalue=bufferBlue[Blue_use_ptr];    
    fprintf(blueDelivery,"assembly part:%d,", Bluevalue);
    fprintf(blueDelivery,"sequence #:%d  ", sequence);
    printf("GET BLUE VALUE: %d  SEQUENCE #: %d\n\n", Bluevalue,sequence);
    Blue_use_ptr = (Blue_use_ptr + 1) % MAX_BLUE;
    count--;
    

    }

    else if(isthread==2){

    int Redvalue=bufferRed[Red_use_ptr];    
    fprintf(redDelivery,"assembly part:%d,", Redvalue);
    fprintf(redDelivery,"sequence #:%d  ", sequence);
    printf("GET RED VALUE: %d  SEQUENCE #: %d\n\n", Redvalue,sequence);
    Red_use_ptr = (Red_use_ptr + 1) % MAX_RED;
    count--;

    }

}

pthread_t ThreadL,ThreadR,ThreadX,ThreadY;
pthread_mutex_t mutex;
pthread_cond_t empty,full;

int rc=pthread_mutex_init(&mutex,NULL);
assert(rc==0);

int erc=pthread_cond_init(&empty,NULL);
assert(erc==0);
int crc=pthread_cond_init(&full,NULL);
assert(crc==0);



void Pthread_lock( pthread_mutex_t *lock){
    int rc= pthread_mutex_lock(lock);
    assert(rc==0);


}

void Pthread_unlock(pthread_mutex_t *lock){
    int rc=pthread_mutex_unlock( lock);
    assert(rc==0);


}



void *Producer(void *args){
    while(terminateProducers!=1){

    Pthread_lock(&mutex);

    while((count==MAX_BLUE-1)||(count==MAX_RED-1)){
        pthread_cond_wait(&empty,&mutex);

    }
    put(*isthread);
    sequence++;
    pthread_cond_signal(&full);
    Pthread_unlock(&mutex);


}
}









void *Consumer(void *args){

    while(terminateConsumers!=1){

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

    while(count==0){
        phtread_cond_wait(&full,&mutex);

    }
    get(*isthread);
    pthread_cond_signal(&empty);

    Pthread_unlock(&mutex);





}
}

void Pthread_create(pthread_t *p, const pthread_attr_t  *attr, void * (threadFunction) (void *), void *arg ){
    int rc=pthread_create(p,attr,threadFunction,arg);
    assert(rc==0);

}



Pthread_create(ThreadL,NULL,Producer,NULL);
Pthread_create(ThreadR,NULL,Producer,NULL);

Pthread_create(ThreadX,NULL,Producer,threadX);
Pthread_create(ThreadY,NULL,Producer,threadY);


pthread_join(ThreadL,NULL);
pthread_join(ThreadR,NULL);
pthread_join(ThreadX,NULL);
pthread_join(ThreadY,NULL);
printf("%s","End Child...");




//child process



}


else if(pid>0){

    printf("%s\n\n","parent process waiting...");
    wait(NULL);
    printf("%s","parent has finished...");
    return 0;
//parent



}




else{

perror("fork failed");

}














    return 0;
}



//notes
//can a process be terminated before threads complete (view book)?