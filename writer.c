#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <unistd.h>
#define MEM_SIZE 4096

int main() {
   int shmId;
   char *shmPtr;
   char* filePath;
   filePath = malloc(500);
   char* path = "writer.c";
   char userInput[500];
   struct echo {
      bool[3] isWaiting;
      char[500] strToSend;
   }

   // add signal handler for ctrl c to let readers know they have exited

   //key_t key = ftok(getcwd("writer.c",8), 1600);
   key_t key = ftok("writer.c",1600);
   printf("Key: %lu\n",key);
   struct echo *msgToSnd = (struct* echo);

   if((shmId = shmget(key, MEM_SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror("Error accessing shared memory");
      exit(1);
   }
   
   printf("ShmID: %lu\n",shmId);   
   
   if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
      perror("Error attaching shared memory");
      exit(1);
   }
   msgToSend->isWaiting
   // while
   // true means displayed - and waiting
   // write to shared memory
   while(true) {	   
      printf("Enter a line: ");
      fgets(userInput, 500, stdin);

   }


   // when we want to detacth
   if (shmdt(shmPtr) < 0) {
      perror("Error detatching");
      exit(1);
   }
   
      
   return 0;
}
