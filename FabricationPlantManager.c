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


FILE *railway;
FILE *blueDelivery;
FILE *redDelivery;

char *railFile="railwayCars.txt";
char *blueFile="BlueDelivery.txt";
char *redFile="RedDelivery.txt";

int main(){




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

int new_railway_fd = dup(fileno(railway));
int new_blueDelivery_fd = dup(fileno(blueDelivery));
int new_redDelivery_fd=dup(fileno(redDelivery));

fclose(railway);
fclose(blueDelivery);
fclose(redDelivery);

char new_railway_fd_str[32];
char new_blueDelivery_fd_str[32];
char new_redDelivery_fd_str[32];



sprintf(new_railway_fd_str, "%d", new_railway_fd);
sprintf(new_blueDelivery_fd_str, "%d", new_blueDelivery_fd);
sprintf(new_redDelivery_fd_str, "%d", new_redDelivery_fd);











int Pid=fork();



if(Pid==0){


char *args[] = { new_railway_fd_str, new_blueDelivery_fd_str, new_redDelivery_fd_str, NULL};

execv("/home/TU/tun66280/3207 C Projects/project-2-f23-tun66280/assembly",args);
perror("execv");
return 1;






}




else{
printf("\n%s\n","parent is waiting ");


wait(NULL);

printf("\n%s\n","parent has finished");



}



    return 0;
}