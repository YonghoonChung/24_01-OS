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

int emptyString(char* cmd);

int main(int argc, char* argv[]){
    char cmd[SIZE] = "";
    char shm_name[SIZE] = "";
    // Block the unwanted format
    if(argc != 2){
        printf("Usage: %s <shard_mem_name>\n", argv[0]);
        return 0;
    }

    printf("Welcome to my remote shell client!\n");

    strcpy(shm_name, argv[1]);
    // printf("shared Memory name = %s\n", argv[1]);
    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    if(shm_fd == -1){
        printf("Failed to open shared memory\n");
        return 0;
    }
    void* shm = mmap(0,SIZE, PROT_WRITE , MAP_SHARED, shm_fd, 0);
    if(shm == MAP_FAILED){
        printf("mmap Error\n");
        return 0;
    }

    printf("Connected to server via shared memory.\n");

    while(1){
        printf("$ ");
		
        fgets(cmd, MAX_LEN, stdin);
        cmd[strlen(cmd) - 1] = '\0';
		if(emptyString(cmd)) continue;
		
        while(strcmp((char*)shm, "")==TRUE){ // repeating until shm[0] == 0
            if(usleep(100000)==-1){
                printf("usleep() error\n");
                return 0;
            }
        }


        if(strcmp(cmd, "exit") == FALSE) break;

        strcpy((char*)shm, cmd);
        // printf("shm = %s\n", (char *)shm);
        if(strcmp(cmd, "exit_svr")==FALSE) break;
    }
    if(munmap(shm, SIZE)==-1){
        printf("munmap() error\n");
        return 0;
    }
    printf("Bye!\n");
    return 0;
}

int emptyString(char* cmd){
    size_t len = strlen(cmd);

    for(size_t i=0; i<len; i++){
        if(!isspace(cmd[i])){
            return 0;
        }
    }

    return 1;
}

