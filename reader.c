#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/ipc.h> 
#include <sys/shm.h>

int main(){

	//get key using ftok()
	int key = ftok("fileloc", 1);
	//
	if ((shmPtr = shmat (key, 0, 0)) == (void*) -1) { 
      		perror ("can't attach\n"); 
      		exit (1); 
   	}

	//get struct pointer from shared memory
	// struct = (struct*) shmPtr;
	while(1){
	
		//while(struct.iswaiting[i] or exit);
		//	if exit:
		//		exit(0)
		//	else:
		//		print string
		//		set iswaiting[i] to true
	}

}
