/*
 * 2024년 3월 25일
 * HW 02 assignment by prof.Injung Kim
 *
 * Problem : Write a program that copies following attributes of a file into another.
 * using open, close, read, write
 * 
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <utime.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 512 

void copyFile(char *srcPath, char *destPath);
void copyAttributes(char *rcPath, char *destPath);

int main (int argc, char *argv[]){
	//for(int i = 0; i <= argc; i++){
	//	printf("argv[%d] = \"%s\"\n",i,argv[i]);
	//}

	if(argc < 3){
		printf("Usage: %s <src_path> <dest_path>\n", argv[0]);
		return 0;
	}
	char *srcPath = argv[1];
	char *destPath = argv[2];
	int choice;
	if (strcmp(srcPath, destPath) == FALSE) {
		printf("You have called the source path and destination path the same.\n");
		printf("If you continue, the message in the source path will be removed.\n");
		printf("Do you still want it? (Yes : 1, No : 0) ");
		scanf("%d", &choice);

		if (choice == FALSE) return 0;
		
	}
	copyAttributes(srcPath, destPath);
	return 0;
}

void copyAttributes(char *srcPath, char *destPath){
	// Copy the Attributes
	struct stat statBuff;
	struct tm *timeInfo;
	//struct utimbuf timeBuff;

	int source = stat(srcPath, &statBuff);
	if(source <0){
		printf("Error Occurred while loading stat information\n");
		return;
	}
	//int time = utime(srcPath, &timeBuff);
	//if(time < 0){
	//	printf("Error Occurred while loading time information\n");
	//	return;
	//}
	
	time_t modiTime = statBuff.st_mtime;
	//timeInfo = localtime(&modiTime);
	timeInfo = localtime(&modiTime);
	
	printf("attributes of file \"%s\"\n", srcPath);
	printf("\tst_dev = %ld\n",statBuff.st_dev);
	printf("\tst_mode = %d\n",statBuff.st_mode);
	printf("\tst_uid = %d\n", statBuff.st_uid);
	printf("\tst_gid = %d\n", statBuff.st_gid);
	printf("\tst_size = %ld\n", statBuff.st_size);
	printf("\tst_mtime = %ld\n", statBuff.st_mtime);
	printf("\t%02d/%02d/%02d %02d:%02d:%02d\n", timeInfo->tm_year+1900,timeInfo->tm_mon+1,timeInfo->tm_mday,timeInfo->tm_hour,timeInfo->tm_min, timeInfo->tm_sec);
	printf("File attribures were successfully copied.\n");
	return;
}
