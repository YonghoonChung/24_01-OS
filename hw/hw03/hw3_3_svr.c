#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_LEN 256
#define SIZE 512
#define TRUE 1
#define FALSE 0

int splitString(char* cmd, char* argv[]);

int main(int argc, char* argv[]){
	char cmd[SIZE] = "";
	char shm_name[SIZE] = "";
	// Block the unwanted format
	if(argc != 2){
		printf("Usage: %s <shard_mem_name>\n", argv[0]);
		return 0;
	}
	strcpy(shm_name, argv[1]);
	// printf("shared Memory name = %s\n", argv[1]);
	int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	if(shm_fd == -1){
		printf("Failed to open shared memory\n");
		return 0;
	}
	if(ftruncate(shm_fd, SIZE)==-1){
		printf("Failed to configuring the size of the shared memory\n");
		return 0;
	}
	void* shm = mmap(0,SIZE, PROT_READ|PROT_WRITE , MAP_SHARED, shm_fd, 0);
	if(shm == MAP_FAILED){
		printf("mmap Error\n");
		return 0;
	}

	// Initialize the shared memory.
	strcpy((char*)shm, "");

	// printf("shm = %s\n", (char *)shm);
	while(1){
		printf("Waiting for a command...\n");

		while(strcmp((char*)shm, "")==FALSE){ // repeating until shm[0] != 0
			if(usleep(100000)==-1){
				printf("usleep() error\n");
				return 0;
			}
		}
		
		strcpy(cmd, (char*)shm); // Copy the command in shm to a local variable(cmd) 
		strcpy((char*)shm, "");

		// Code from hw3_2 start
		argc = splitString(cmd, argv);

//		printf("argc = %d\n", argc);
//		for(int i = 0; i<=argc; i++){
//			printf("\targv[%d] = \"%s\"\n", i, argv[i]);
//		}
		if(strcmp(argv[0], "exit_svr")==FALSE) break;// If the command is "exit_svr" then break;
		
		if(strcmp(argv[0],"cd")==FALSE){
			char* path = argv[1];
			if(strcmp(argv[1], "~")==FALSE){
				printf("Passed\n");
				path = "/home/yonghoon";
			}
			int ch = chdir(path);

			if(ch == 0) printf("Success\n");
			else printf("Could not find the Directory.\n");

			continue;
		}
		// start forking.
		int child = fork();
		if(child > 0){
			wait(NULL);
		}else if(child == 0){
			if(execvp(argv[0], argv)==-1){
				printf("ERROR! Failed to Load %s\n", argv[0]);
				return 0;
			}else exit(-1);
		}else{
			printf("ERROR While Creating the Child\n");
			return 0;
		}
		// Code from hw3_2 end

	}
	
	// unmap shared memory
	if(munmap(shm, SIZE)==-1){
		printf("munmap() error\n");
		return 0;
	}
	if(shm_unlink(shm_name)==-1) {
		printf("shm_unlink Error\n");
		return 0; 
	}

	printf("Bye!\n");

	return 0;
}

int splitString(char* cmd, char* argv[]){
	int argc = 0;
	int len = strlen(cmd);
	int inWord = 0;

	argv[argc] = &cmd[0];

	for (int i = 0; i < len; i++) {
        if (!isspace(cmd[i]) && !inWord) {
            inWord = 1;
            argv[argc++] = &cmd[i];
        } else if (isspace(cmd[i])) {
            inWord = 0;
            cmd[i] = '\0';
        }
    }
	argv[argc] = NULL;
	return argc;
}
