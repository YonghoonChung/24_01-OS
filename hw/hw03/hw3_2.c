#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_LEN 256
#define TRUE 1
#define FALSE 0


int splitString(char* cmd, char* argv[]);
int emptyString(char* cmd);


int main(){
	char* argv[100];
	int argc = 0;
	char cmd[MAX_LEN] = "";

	printf("Welcome to MyShell!\n");
	
	//Like a shell, it loops.
	while(1){
		printf("$ ");
		fgets(cmd, MAX_LEN, stdin);	
		cmd[strlen(cmd) - 1] = '\0';
		// return the command line.
		printf("cmd = \"%s\"\n", cmd);
		
		//exit check and empty string check
		if(strcmp(cmd, "exit") == FALSE) break;
		else if (emptyString(cmd)) continue;		
		argc = splitString(cmd,argv);
			
		// printing the argument vectors
		printf("argc = %d\n", argc);
		for(int i = 0; i<=argc; i++){
			printf("\targv[%d] = \"%s\"\n", i, argv[i]);
		}

		// change directory. -> implemented cd ~ as well.
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
int emptyString(char* cmd){
	size_t len = strlen(cmd);

	for(size_t i=0; i<len; i++){
		if(!isspace(cmd[i])){
			return 0;
		}
	}

	return 1;
}
