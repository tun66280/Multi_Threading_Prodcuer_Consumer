# Project-2-F23  
## Project 2 Exploring Synchronization
 

# Section 1: Comple and Run
To compile and run the program simply use the make command. If you run make with no arguments then it will create an excecutable file called Main from the object files created. Now, if you wish the run the program you must specify the "run" target after entering the make command.The makefile has 4 targets: all, Main, assemblymanager, and clean. make all is the defualt target which will simply compile the Fabrication plant manager program and create an executable called Main. Performing make Run will compile and run the Main program. lastly, running make clean, will delete the executable main, hence the target name, "clean".



# Section 2: Design and Implementation

I desgined my programs piece by piece. I started by implementing my AssemlyManager.c file, creating my buffers, then my get() and put() functions, my producer() and consumer() functions, etc. Then I created my FabrticationPlantManager.c file which creates a new child process and allows it to inherit its files and etc. The understanding of threads and how they function was critical in my implementation. Understanding that each thread has its own thread local storage, within the address space, the workings of locks and condition variables, and the problems that come with concurrency were crucial to the design and implementation of my work.



 # Section 2.1: FabricationPlantManager.c
FabricationPlantManager.c was used to open the Fabrication Plant! It opened three files: railwayCars.txt, BlueDelivery.txt, and RedDelivery.txt. It then duplicates the file descriptors from which I pass as arguments to mt execv call after forking. Yes! That is, FabticationPlantmanager.c is responsible for "initiating" the AssemblyManager.c file which is the heart of the producer consumer operation! The "Fabrication Plant" waits for the "Assembly" to be complete and afterwards closes the plant!

# Section 2.2: AssemblyManager.c
the AssemblyManage.c is the heart of the producer-consumer operation. It reads from railway.txt file and puts the values into two buffers(this is the production) and it reads the data from these buffers and writes the data to two output files(this is the consumption).

# Section 2.2.1: partsforA()

The int partsforA(int num) function takes an integer as parameter and sees if the integer falls between the range [1,12]. if it does, then the function returns 1. If it doesnt then  it returns 0; this function is used to ensure the correct part is going to its designateed buffer(buffer Blue).

# Section 2.1.2: partsforB()


The int partsforB(int num) function takes an integer num as parameter and sees if the integer falls between the range [13,25]. If it does, then the function returns 2. If it doesnt then  it returns 0; this function is used to ensure the correct part is going to its designateed buffer(buffer Red).


# Section 2.1.3:  put()

The int put(int part) function takes an integer part as parameter. Its first condtion is to check if the EOF has been reach for the railway.txt file that contains all the parts. Its has a subset of condition from which it calls partsforA to check if the part is belongs to the blue buffer and partsforB to check if the aprt belongs to the red buffer. if the end EOF is reached it inserts -1 to both buffers and returns 1;

#  Section 2.1.4: get()

the int get(int isthread) takes as parameter a thread signature- ishtread- to identify which thread is performing the operation. Said operation is the retrieval of parts from either buffer blue or buffer red. This signature is necessary as threadX "consumes" buffer blue while threadY "consumes" buffer red. the function returns 0 in all cases except from when -1 is retrieved from the buffers. In this case the function returns 1.


# Section 2.1.5: Pthread_create, Pthread_lock, Pthread_unlock
These are all wrapper functions to ensure that the calls to pthread_create, pthread_mutex_lock, pthraed_mutex_unlock, that ensure there was no issue in the Posix library functions.


# Section 2.1.6 Producer()

The void *Producer(void *args) function is a function that threadL and threadR will call to place parts into designated buffers. It takes as parameter a pointer to a void args. This is a thread signature that will ensure threads L and thread R sleep when they are supposed to(after producing a certain amount of parts). This function places parts into their designateed buffer while ensuring mutual exclusion through locks, and ensure ordering through condition variables(when to wait for items to be placed in buffer, items to be removed from buffers, and  signaling that conditions have been satisfied). Each thread termiantes upon the condition that -1 has been placed into the buffer. this is known by seeing the return value of puts(described in puts section) and also by a shutdown function called threadfinished.


# Section 2.1.7 Consumer()

The void *Consumer(void *args) function is a function that threadX and threadY will call to retrieve parts from their assiociated buffers. It takes as parameter a pointer to a void args. This is a thread signature that will ensure threads X and thread Y sleep when they are supposed to(after consuming a certain amount of parts) as will as ensure that the threads  are consuming from their associated buffer(threadX consuming from blue and thread Y consuming from red) . This function retreives parts from their associated buffer while ensuring mutual exclusion through locks, and ensure ordering through condition variables(when to wait for items to be placed in buffer, items to be removed from buffers, and  signaling that conditions have been satisfied). Each thread termiantes upon the condition that -1 has been read from the buffer. this is known by seeing the return value of gets(described in gets section) and also by a shutdown function called threadfinished.


# Section 3: Testing

I tested my program through having printt statments for whenever put and get functions were called, as well as print statments for when a thread went to sleep and when it woke up. The main bugs I had were deadlocks. Some deadlocks were caused by not passing pointers to specific shared variables used as conditions in  conditin variables.Thus when the "state of the world changed" i didnt pass a pointer so my variable never actually changed.This would cause my threads to sleep forever. My other deadlock was also caused through mishandling of conditions for condition variables. One prominent issue was sending a thread to sleep when its buffer was full. Originally I had an || statement that checked whether one buffer was at it max capacity or if the other was. This lead to inconsistent sleeping and wake ups, that proved fatal for the functioning of my program. I fixed this deadlock by writing conditions to know which part was about to be read and by knowing beforehand, I could check if that associated buffer was full, rather than checking if both were full.  