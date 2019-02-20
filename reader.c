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

int main(){
    int shmId;
    char *shmPtr;
    int i;
    
    struct shm_echo {
        bool isWaiting[3];
        int numConnected;
        char strToSend[500];
    };
    
	//get key using ftok()
	key_t key = ftok("writer.c", 1);
    printf("Key: %d\n",key);

	//
    if((shmId = shmget(key, MEM_SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror("Error accessing shared memory");
        exit(1);
    }
	if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      		perror ("can't attach\n"); 
      		exit (1); 
   	}
    
	//get struct pointer from shared memory
    struct shm_echo *msgReceived = (struct shm_echo*) shmPtr;
    msgReceived->numConnected += 1;
    if (msgReceived->numConnected > 2) {
        msgReceived->numConnected = 2;
    }
    i = msgReceived->numConnected;


	while(1){
        if (msgReceived->isWaiting[0] && !msgReceived->isWaiting[i]) {
            printf("%s\n",msgReceived->strToSend);
            msgReceived->isWaiting[i] = true;
        } else {
            printf("else is reached\n");
            //msgReceived->isWaiting[i] = false;
        }
	
		//while(struct.iswaiting[i] or exit);
		//	if exit:
		//		exit(0)
		//	else:
		//		print string
		//		set iswaiting[i] to true
	}

}
