//Welcome to the Assembly center! This .c file assembles the parts and ships them off
//to their affiliated delivery trucks!


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


#define MAX_BLUE 15 //max buffer size for  blue 
#define MAX_RED 10  // max buffer size for red 





FILE *railway;
FILE *blueDelivery;
FILE *redDelivery;






pthread_t ThreadL,ThreadR,ThreadX,ThreadY;
pthread_mutex_t mutex;
pthread_cond_t empty,full;

int partsforA(int num){ // partsforA
if(num>=1 && num<=12){
    return 1;
}
return 0;


}



int partsforB(int num){ //partsforB
    if(num>=13 && num<=25){

        return 2;
    }
    return 0;
    
}




    
  

    int threadX=1;
    int threadY=2;
    int threadL=3;
    int threadR=4;

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
    
   

int put(int part) { //puts part into buffer based on part type

   if(collectedPart!=EOF){ // if we havent reached EOF
    
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

    else { // if weve reached EOF place -1 in both buffers

        
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
    
return 5;

}

int get(int isthread) { //retrieves part from specified buffer

    if(isthread==1){ // thread X identifier

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

    else if(isthread==2){ ////thread Y identifier

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








void *Producer(void *args){ //thread producer function 
    printf("HELLO WORLD\n");
    int *count=0;
    int partType;
    int MaxbufferElement=100;
    int threadFinished=0;
    int *isthread=(int *)args;


    while(1){ //prevent race condiition
    

        Pthread_lock(&mutex);


        collectedPart=fscanf(railway, "%d ",&part);
        


        if( partsforA(part)==1){ // if a blue part initialze vars
             partType=part;
            count=&count1;
            MaxbufferElement=MAX_BLUE;
        }

        else {  //red part, initialize vars
             partType=part;
            count=&count2;
            MaxbufferElement=MAX_RED;
        }



       

    

    while(*count==MaxbufferElement){
        printf("\n%s\n","Producer is SLEEPING");
        pthread_cond_wait(&empty,&mutex);
        printf("\n%s\n","Producer WAKES up ");



    }
    if( put(partType)==1){
        threadFinished=1;

    }


     if(*isthread==threadL){

            sleep(.25);
            
        


    }

    if(*isthread==threadR){
    
        sleep(.5);
            
        }
    
    
        sequence++;
    pthread_cond_signal(&full);
     //end critical section
   if(threadFinished==1){
        Pthread_unlock(&mutex); //end critical section
        return NULL;
    
   }
   Pthread_unlock(&mutex);

}
 printf("\nProducer Exit\n");
}






void *Consumer(void *args){ // Consumer method 
    int *count;

    int *isthread=(int *)args; //get thread signature 
    int threadFinished=0; // checks if thread has terminated


    if(*isthread==1){ // thread X
         count=&count1;
         threadFinished=0;
    }

    else{ // thread Y
        threadFinished=0;
         count=&count2;
    }

    printf("(thread %d\n)",*isthread);

    while(1){ // while thread hasnt terminated. Used to protect critical section

    Pthread_lock(&mutex); // lock
    printf("\ndebug\n");




    while(*count==0){ // sleep if buffer is full
        printf("(thread %d), %s\n",*isthread,"Consumer is SLEEPING");
        pthread_cond_wait(&full,&mutex);
        printf("(thread %d), %s\n",*isthread,"Consumer WAKES up");


    }

    if(get(*isthread)==1){

        threadFinished=1;

    }

    sleep(.2);

    pthread_cond_signal(&empty); //signal that buffer isnt full





if(threadFinished==1){
    Pthread_unlock(&mutex); // unlock

    return NULL;
}

    Pthread_unlock(&mutex); // unlock


}

printf("%s","thread exits");
    }






int main(int argc,char *argv[]){

 int railway_fd = atoi(argv[0]);
int blueDelivery_fd = atoi(argv[1]);
int redDelivery_fd = atoi(argv[2]);



 railway = fdopen(railway_fd, "r");
 blueDelivery = fdopen(blueDelivery_fd, "w");
 redDelivery=fdopen(redDelivery_fd,"w");

int rc=pthread_mutex_init(&mutex,NULL);
assert(rc==0);
int erc=pthread_cond_init(&empty,NULL);
assert(erc==0);
int crc=pthread_cond_init(&full,NULL);
assert(crc==0);



    

struct BufferItem bufferBlue[15]={0};
struct BufferItem bufferRed[10]={0};

    

    




printf("%s\n\n","ERIIIEEEEEE TIII DEEEE");
printf("Sequence is: %d\n", sequence);
printf("%s\n\n","WORTH THE WAIT");


Pthread_create(&ThreadL,NULL,Producer,&threadL);
Pthread_create(&ThreadR,NULL,Producer,&threadR);

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



