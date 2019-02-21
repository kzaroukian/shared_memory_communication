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

// Lab 5 Writer by Marc Chesebro and Kaylin Zaroukian

void interruptHandler(int sigNum);


int shmId;
char *shmPtr;

int main() {
   char userInput[500];
   struct shm_echo {
       bool isWaiting[3];
       int numConnected;
       int numPrinted;
       char strToSend[500];
    };
    
   signal(SIGINT, interruptHandler);

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
    
   struct shm_echo *msgToSnd = (struct shm_echo*) shmPtr;

    msgToSnd->isWaiting[0] = true;
    msgToSnd->isWaiting[1] = true;
    msgToSnd->isWaiting[2] = true;
    
    msgToSnd->numConnected = 0;

   while(1) {
      // check if we can send the next message
        printf("Enter a line: ");
        fgets(userInput, 500, stdin);
        memcpy(msgToSnd->strToSend, userInput, 500);
        msgToSnd->isWaiting[1] = true;
        msgToSnd->isWaiting[2] = true;
        msgToSnd->numPrinted = 0;

    }
    
   // when we want to detacth
   if (shmdt(shmPtr) < 0) {
      perror("Error detatching");
      exit(1);
   }
      
   return 0;
}

void interruptHandler(int sigNum) {
    if (shmdt(shmPtr) < 0) {
        perror("Error detatching");
        exit(1);
    }

    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit(1);
    }
	printf("Exiting...\n");
	exit(0);
}
