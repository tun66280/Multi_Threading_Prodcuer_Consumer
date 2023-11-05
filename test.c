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
#include <sys/wait.h>


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

        return 2;
    }
    return 0;
    
}




    
  

    int threadX=1;
    int threadY=2;

    struct BufferItem {
    int part;
    int sequence;
    };

struct BufferItem bufferBlue[15]={0};
struct BufferItem bufferRed[10]={0};

    int Blue_fill_ptr = 0;
    int Blue_use_ptr = 0;
    int Red_fill_ptr = 0;
    int Red_use_ptr = 0;

    int collectedPart;
    int count1=0;
    int count2=0;
    int sequence=1;
    int part;
    int done=0;
    int terminateProducers=0;
    int terminateConsumers=0;

int put(int part) {

   if(collectedPart!=EOF){
    
    printf("%s\n","PUT METHOD IS INDEED CALLED");

    if(partsforA(part)){
        bufferBlue[Blue_fill_ptr].part=part;
        bufferBlue[Blue_fill_ptr].sequence=sequence;
        Blue_fill_ptr=(Blue_fill_ptr+1)%MAX_BLUE;
        //    printf("%s\n","increment count");
        count1++;
        return 0;    


    }
    else if( partsforB(part)){

        bufferRed[Red_fill_ptr].part=part;
        bufferRed[Red_fill_ptr].sequence=sequence;
        Red_fill_ptr=(Red_fill_ptr+1)%MAX_RED;
      //   printf("%s\n","increment count");

        count2++;

        return 0;

    }    

   
   }

    else {

        
        bufferBlue[Blue_fill_ptr].part=-1;
        bufferBlue[Blue_fill_ptr].sequence=sequence;
        Blue_fill_ptr=(Blue_fill_ptr+1)%MAX_BLUE;

        bufferRed[Red_fill_ptr].part=-1;
        bufferRed[Red_fill_ptr].sequence=sequence;
        Red_fill_ptr=(Red_fill_ptr+1)%MAX_RED;
        
        count1++;
        count2++;
        return 1;

    } 
    


}

int get(int isthread) {

    if(isthread==1){

    int Bluevalue=bufferBlue[Blue_use_ptr].part;
    int sequenceval=bufferBlue[Blue_use_ptr].sequence;
    Blue_use_ptr = (Blue_use_ptr + 1) % MAX_BLUE;
     //    printf("%s\n","decrement count");

    count1--;

    fprintf(blueDelivery,"assembly part:%d,", Bluevalue);
    fprintf(blueDelivery,"sequence #%d  ", sequenceval);
    printf("GET BLUE VALUE: %d  SEQUENCE #: %d\n\n", Bluevalue,sequenceval);

    
    if(Bluevalue==-1){
        return 1;
    }
    

    }

    else if(isthread==2){

    int Redvalue=bufferRed[Red_use_ptr].part;  
    int sequenceval=bufferRed[Red_use_ptr].sequence;  
    Red_use_ptr = (Red_use_ptr + 1) % MAX_RED;
   //  printf("%s\n","decrement count");

    count2--;     


    fprintf(redDelivery,"assembly part:%d,", Redvalue);
    fprintf(redDelivery,"sequence #:%d  ", sequenceval);
    printf("GET RED VALUE: %d  SEQUENCE #: %d\n\n", Redvalue,sequenceval);


    if(Redvalue==-1){
        return 1;
    }
    }


    else{
        printf("%s","what happned!!!?????");
    }


return 0;
}



void CompleteProduction(int (*putMethod)(int ), int *part){

if(putMethod(*part)==1){
    terminateProducers++;
}



}

void setCompletion(int (*getMethod)(int), int arg,int *p_arg){

if(getMethod(arg)==1){
        printf("value of threadfinish before  is: %d\n\n",*p_arg);

    terminateConsumers++;
    (*p_arg)++;
    printf("value of threadfinish after is is: %d\n\n",*p_arg);
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



void Pthread_create(pthread_t *p, const pthread_attr_t  *attr, void * (*threadFunction) (void *), void *arg ){
    int threadCreate=pthread_create(p,attr,threadFunction,arg);
    if(threadCreate!=0){
        perror("Pthread_Create");
    }

}



void Pthread_lock( pthread_mutex_t *lock){
    int threadLock= pthread_mutex_lock(lock);
    if(threadLock!=0){

        perror("Pthread_lock");
    }


}

void Pthread_unlock(pthread_mutex_t *lock){
    int threadUnlock=pthread_mutex_unlock( lock);
    if(threadUnlock!=0){
        perror("Pthread_unlock");
    }


}








void *Producer(void *args){
    printf("HELLO WORLD\n");
    int *count=0;
    int partType;
    int MaxbufferElement=100;
    while(terminateProducers<1){ //prevent race condiition
    

        Pthread_lock(&mutex);





    while(terminateProducers<1){

        collectedPart=fscanf(railway, "%d ",&part);
        


        if( partsforA(part)==1){
             partType=part;
            count=&count1;
            MaxbufferElement=MAX_BLUE;
        }

        else{
             partType=part;
            count=&count2;
            MaxbufferElement=MAX_RED;
        }
        


    

    while(*count==MaxbufferElement){
        printf("\n%s\n","Producer is SLEEPING");
        pthread_cond_wait(&empty,&mutex);
        printf("\n%s\n","Producer WAKES up ");



    }
     CompleteProduction(put,&partType);
    sequence++;
    pthread_cond_signal(&full);
    Pthread_unlock(&mutex);
    printf("\nProducer Exit\n");
    break;

}
    

}
}





void *Consumer(void *args){
    int *count;

    int *isthread=(int *)args;
    int fill_ptr;
    int MaxbufferElement;
    int threadFinished=0;


    if(*isthread==1){
         count=&count1;
         threadFinished=0;
    }

    else{
        threadFinished=0;
         count=&count2;
    }

    printf("(thread %d\n)",*isthread);

    while(threadFinished<1){

    Pthread_lock(&mutex);
    printf("\ndebug\n");
    while(terminateConsumers<2){
            if(threadFinished==1){break;}


    while(*count==0){
        printf("(thread %d), %s\n",*isthread,"Consumer is SLEEPING");
        pthread_cond_wait(&full,&mutex);
        printf("(thread %d), %s\n",*isthread,"Consumer WAKES up");


    }

    setCompletion(get,*isthread,&threadFinished);

    pthread_cond_signal(&empty);




    Pthread_unlock(&mutex);
    break;

}

printf("%s","thread exits");
    }

//printf("%s","thread exits");
}




printf("%s\n\n","ERIIIEEEEEE TIII DEEEE");
printf("Sequence is: %d\n", sequence);
printf("%s\n\n","WORTH THE WAIT");


Pthread_create(&ThreadL,NULL,Producer,NULL);
Pthread_create(&ThreadR,NULL,Producer,NULL);

Pthread_create(&ThreadX,NULL,Consumer,&threadX);
Pthread_create(&ThreadY,NULL,Consumer,&threadY);


pthread_join(ThreadL,NULL);
pthread_join(ThreadR,NULL);
pthread_join(ThreadX,NULL);
pthread_join(ThreadY,NULL);
printf("\n\n%s","End Child...");

printf("%s","print statement for Blue buffer contents\n\n");

for(int i=0; i<MAX_BLUE;i++){
    printf("%d ",bufferBlue[i].part);
}

printf("%s","\n\n");



printf("%s","print statement for Red buffer contents\n\n");

for(int i=0; i<MAX_RED;i++){

    printf("%d ",bufferRed[i].part);
}


    return 0;
}



