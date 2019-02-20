#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#define MEM_SIZE 4096

void interruptHandler(int sigNum);

int main() {
   int shmId;
   char *shmPtr;
   char* filePath;
   filePath = malloc(500);
   char* path = "writer.c";
   char userInput[500];
   struct shm_echo {
      bool isWaiting[3];
      int numConnected;
      char strToSend[500];
   };
    
    signal(SIGINT, interruptHandler);

   // add signal handler for ctrl c to let readers know they have exited

   //key_t key = ftok(getcwd("writer.c",8), 1600);
   key_t key = ftok("writer.c",1);
   printf("Key: %d\n",key);

   if((shmId = shmget(key, MEM_SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror("Error accessing shared memory");
      exit(1);
   }
   
   printf("ShmID: %d\n",shmId);
   
   if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
      perror("Error attaching shared memory");
      exit(1);
   }
   //struct shm_echo *msgToSnd = (struct shm_echo*)malloc(sizeof(struct shm_echo));
   struct shm_echo *msgToSnd = (struct shm_echo*) shmPtr;

    // initial array values
    // writer
    // initially nothing has been written
    msgToSnd->isWaiting[0] = false;
    // readers
    // initally ready to receive and write to the readers
    msgToSnd->isWaiting[1] = true;
    msgToSnd->isWaiting[2] = true;
    
    msgToSnd->numConnected = 0;
   // while
   // true means displayed - and waiting
   // write to shared memory
   while(true) {
      // check if we can send the next message
       if(!msgToSnd->isWaiting[0] && msgToSnd->isWaiting[1] && msgToSnd->isWaiting[2]) {
           printf("Enter a line: ");
           fgets(userInput, 500, stdin);
           memcpy(msgToSnd->strToSend, userInput, 500);
           // means we have updated with a new message
           msgToSnd->isWaiting[0] = true;
           
           if(strncmp(userInput, "exit", 4) == 0) {
               exit(0);
           }
       } else {
           // no message to send
           //printf("else is reached\n");
           msgToSnd->isWaiting[0] = false;
       }
   }
    
   // when we want to detacth
   if (shmdt(shmPtr) < 0) {
      perror("Error detatching");
      exit(1);
   }
   
      
   return 0;
}

void interruptHandler(int sigNum) {
    printf("Exiting...\n");
    exit(0);
}
