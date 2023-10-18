# Project-2-F23  
## Project 2 Exploring Synchronization
 
### There are several goals for this assignment:  
1.     Learn and experience multi-process development.  
2.     Learn and experience multi-thread development.  
3.     Introduction to the use of locks for mutual exclusion and critical section control  
4.     Introduction to the use of condition variables for synchronization control in concurrent execution.  
5.     Use of file I/O shared among processes and threads.  
  
### The Basic Problem:  
You are to develop an application that simulates part of an assembly line in a fabrication plant. The application is implemented as a process with a child process. The primary process manages the fabrication plant facilities for the arrival of parts to the plant and the distribution of assembled parts from the plant. The child process uses threads to move new parts (items) into the assembly lines. The child process also contains threads that consume parts from the assembly line and produce modified parts for distribution from the plant.  
   
The project makes use of process and thread creation and concurrency. Safety is introduced into the concurrency issues using locks and condition variables.  
 
### Activities in the Fabrication Plant  
The receiving dock of the fabrication plant retrieves parts from a railway car that arrives at the plant. There are 25 different types of parts that are delivered by the railway car. The railway car is simulated by a linear data file where each record in the file represents 1 part. These parts are simply numbered as 1 to 25 and each part is represented by a value (1 to 25) stored in a data record in the file. Unfortunately, the parts are stored randomly in the railway car ( i.e. in the data file). There are hundreds of parts in each railway car.  
   
Within the fabrication plant, the parts are modified by two different groups of workers. Thus, the parts that are taken from the railway car must be sorted onto two conveyer belts leading to the workstations of the two different groups of workers. Each conveyer belt is simulated by a memory buffer implemented as a FIFO queue.  
   
### The Fabrication Plant Manager  
The main process is the fabrication plant manager. This process is responsible for giving access to railway cars that arrive and for providing access to the truck ramps and trucks receiving assembled parts. As noted, the railway car is represented by a data file. Each of the trucks carrying away assembled parts is represented by a data file.  
   
The plant manager opens the data file representing the railway car and opens the data files that represent the trucks carrying away assembled parts. The plant manager then creates the child process that manages the assembly activities. The child is called the “assembly manager process”. The plant manager then waits for the child to complete and then shuts down the application. The shutdown mechanism for the assembly manager process is described in the section on Program Termination.  
 
### The Assembly Manager  
The assembly manager process has two work areas, the setup area, and the assembly area.   
   
The setup area is comprised of two worker threads (ThreadL and ThreadR). The job of each thread is to read data records (parts) from the data file representing the railway car. Each thread sorts the parts by their identifier or part number (ID, values of 1 -25) into two groups. One group (Group A) has parts numbered 1 -12 and the second group (Group B) has parts numbered 13 – 25.  
   
Group A parts are destined for conveyer belt BLUE, Group B parts are destined for conveyer belt RED.  
   
As parts are removed from the railway car data file, they are given a sequence number. The sequence number is a unique value assigned to a part entering the plant. The sequence number is stored in a shared memory cell that is accessed by both ThreadL and ThreadR. The sequence number is initially 0. In order to get a part from the railway car, a thread increments the sequence number, reads a part number from the data file, and then stores the sequence number with the part number on the designated conveyor belt.  
 
   
**ThreadL Operation**  
ThreadL reads 25 records (parts) from the railway car datafile and stores parts that are in Group A onto conveyor belt BLUE and the parts in Group B onto conveyor belt RED. Once all 25 parts have been moved to the conveyor belts, ThreadL loops back to read the next 25 parts from the railway car datafile. One condition is that it takes .25 seconds to complete the movement of each part onto a conveyor belt. (This means a delay of .25 seconds between deposits into the memory buffer).  
 
   
**ThreadR Operation**  
ThreadR reads 15 records (parts) from the railway car datafile and places parts that are in Group A onto conveyor belt BLUE and the parts in Group B onto conveyor belt RED. Once all 15 parts have been moved to the conveyor belts, ThreadR loops back to read the next 15 parts from the railway car datafile. One condition is that it takes .50 seconds to complete the movement of each part onto a conveyor belt. (This means a delay of .50 seconds between deposits into the memory buffer). This conveyor belt is older and slower.  
   
For both threads, the data stored on the conveyor belt for each part is the part’s sequence number and the part number.  
   
### Conveyor Belt Implementation  
The two memory buffers that simulate conveyor belt BLUE and conveyor belt RED are each a FIFO queue and require synchronization to ensure that the threads,  ThreadR and ThreadL, do not try to store in a full buffer and that threads ThreadX and ThreadY (see below) do not try to retrieve from an empty buffer.  
   
Because of space limitations in the fabrication plant, conveyor belt BLUE is only able to hold a maximum of 15 parts at any time (buffer size 15). Conveyor belt RED is only able to hold a maximum of 10 parts at any time (buffer size 10).  
   
The movement of a part from either of the conveyor belts to the processing thread requires 0.20 seconds to complete (this is a delay of 0.20 seconds between each fetch from the buffer).  
   
### The Consumer Part of the Assembly Manager  
The component of the assembly manager that operates on parts **retrieved from** conveyor belt BLUE and conveyor Belt RED is referred to as the “consumer activity”.  
   
The consumer activity is comprised of 2 threads, ThreadX and ThreadY. ThreadX retrieves parts from conveyer belt BLUE and writes them to a data file that represents a delivery truck, BLUE_Delivery, carrying the Group A parts away from the fabrication plant. ThreadY retrieves parts from conveyer belt RED and writes them to a data file that represents a truck, RED_Delivery, carrying Group B parts away from the plant. These are two separate data files.  
   
ThreadX and ThreadY write both the sequence number and part number retrieved from their conveyor to their corresponding delivery truck (output data file).   
   
### Synchronization  
Access to the shared sequence number generator is to use a lock to provide mutual exclusion.  
Access to each of the conveyor belts (BLUE and RED) is to be synchronized using condition variables including associated locks.  
   
### Program Termination  
The main thread of the assembly manager will maintain a count of the completion of the two threads, ThreadX and ThreadY, protected by a lock. The main thread will wait for the completion of both threads and then exit the child process. Exiting the child process will result in completing the ‘wait’ in the fabrication manager process.  
   
When either ThreadL or ThreadR read an End-of-File (EOF) from the railway car data file, the application is to be shut down. This is to be accomplished by having the thread that reads the EOF placing a sequence number of -1 and part number of -1 in both the BLUE and RED conveyor belts (data buffers). The EOF reading thread will then terminate.  
   
When ThreadX and ThreadY extract from their conveyor belt a part with part number -1, they increment the shared thread completion count. Then the thread is to exit.  
   
### Suggestions and Hints:  
·      This project is described using a real-life model/application. We are often asked to build simulations or games or models using software. In this project the key is to understand the flow of data and their processing by threads. The original source of data is a file shared among the processes and threads ThreadL and ThreadR. The data flows through queues to thread ThreadX and ThreadY. Then the data flow to output files shared by the processes and used by ThreadX and ThreadY.  
·      A single executable can be developed for the fabrication manager process.  
·      A single executable can be developed for the assembly manager process.  
·      ThreadL and ThreadR perform the same functionality, but can be started with different parameters.  
·      ThreadX and ThreadY perform the same functionality, but can be started with different parameters.  
·      The previous two hints mean that you do not write duplicate code, but use parameters to specify differences in threads, where possible.  
·      Consider how file descriptors are part of the PCB and are copied to child processes.  
·      You should design the output file records so that you can use them for debugging as well as program output.  
   
   
### Deliverables:  
The first weekly deliverable includes: the creation of the processes; the design of the two conveyor belt storage buffers; read and write (get/put) functions for the two buffers;   
   
Second weekly deliverable includes development of the threads in the assembly manager, design of the parts data files; creation of the railway car data file with stored parts;  
   
Final program delivery: implementation of the file read and write functions; sample data runs with output files. Description of the implementation  
  
