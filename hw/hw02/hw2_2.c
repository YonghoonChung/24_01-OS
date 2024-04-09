/*
 * 2024년 3월 24일
 * HW 02 assignment by prof.Injung Kim
 *
 * Problem : Write a program that copies a file into another.
 * using open, close, read, write
 * 
 * mechanism
 * ------------------------
 * open
 * loop
 * 		read one sentence
 * 		write one sentence
 * 	close
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 512 

void copyFile(char *srcPath, char *destPath);

int main (int argc, char *argv[]){
	//for(int i = 0; i <= argc; i++){
	//	printf("argv[%d] = \"%s\"\n",i,argv[i]);
	//}

	if(argc != 3){
		printf("Usage: %s <src_path> <dest_path>\n", argv[0]);
		return 0;
	}
	char *srcPath = argv[1];
	char *destPath = argv[2];
	int choice;
	if (strcmp(srcPath, destPath) == FALSE) {
		printf("\'%s\' and \'%s\' are the same file\n", srcPath, destPath);
		return 0;
	}
	copyFile(srcPath, destPath);
	return 0;
}

void copyFile(char *srcPath, char *destPath){
	// Open file
	int source = open(srcPath, O_RDONLY);

	if(source<0){
		printf("Copying srcFile does not exist\n");
		return;
	}

	int destination = open(destPath, O_WRONLY | O_CREAT | O_TRUNC,0600);

	if (destination == -1) {
		printf("Error Occured: %s\n", strerror(errno));
		return;
	}
	
	// Read File and Write File
	static char buffer[BUFFER_SIZE];
	ssize_t readSize = 0, writtenSize = 0;
	ssize_t totalBytes = 0;
	while(TRUE){
		readSize = read(source, buffer,BUFFER_SIZE);
		if(readSize <0){
			printf("Failed to read \'%s\'\n", srcPath);
			break;
		}
		if(readSize == 0) break;

		writtenSize = write(destination, buffer, readSize);
		if(writtenSize <0){
			printf("Failed to write on %s\n", destPath);
			return;
		}
		//printf("Number of byte = %ld\n",readSize);
		totalBytes += readSize;
	}
	printf("Totally, %ld bytes were copied.\n", totalBytes);



	// Close File
	if(close(source)<0){
		printf("Failed to close srcPath \'%s\'.\n", srcPath );
		return;
	}
	
	if(close(destination)<0){
		printf("Failed to close destPath \'%s\'.\n", destPath );
	 	return;
	}
	return;
}
