//My previous solution was perfect. However, I changed few lines to make the code neat and tidy.
/*
 * 2024년 3월 25일
 * HW 02 assignment by prof.Injung Kim
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

// My code was relatively perfect. I made function to make the code neat and tidy. Also I have made the same format for the 
// Comment by 22100584 Changgun Lee :
// 		- The code was good! They were very similar to mine. 
// 		- Why don't you remove the line that you commented it?
//		- Just remove the line from 91 ~ 97
// Comment by 22000374 Minyeong Song:
// 		- They seems good for me
// Comment by 22100428 Seoyoung Ahn:
// 		- I don't think you have anything to change. I should try in this way.
// 		- She referenced my code to change the main function and made it shorter main function.
void copyAttributes(char *srcPath, char *destPath);

int main (int argc, char *argv[]){
	if(argc != 3){
		printf("Usage: %s <src_path> <dest_path>\n", argv[0]);
		return 0;
	}
	char *srcPath = argv[1];
	char *destPath = argv[2];

	if (strcmp(srcPath, destPath) == FALSE) {
		printf("\'%s\' and \'%s\' are the same file.\n", srcPath, destPath);
		return 0;
	}
	copyAttributes(srcPath, destPath);
	return 0;
}

void copyAttributes(char *srcPath, char *destPath){
	struct stat statBuff;
	struct tm *timeInfo;
	struct utimbuf timeBuff;
	
	int source = open(srcPath, O_RDONLY);
	int destination = open(destPath, O_WRONLY | O_CREAT,0600);

	if(source<0){
		printf("\'%s\' does not exist\n", srcPath);
		return;
	}

	if (destination == -1) {
		printf("Error Occured: %s\n", strerror(errno));
	}


	//Loading the status of srcPath
	int statSuccess = stat(srcPath, &statBuff);
	if(statSuccess < 0){
		printf("Error Occurred while loading stat information\n");
		return;
	}
	
	// Copying the Time (srcPath -> destPath)
	timeBuff.actime = statBuff.st_atime;
	timeBuff.modtime = statBuff.st_mtime;

	int utimeSuccess = utime(destPath, &timeBuff);
	if(utimeSuccess < 0){
		printf("Copying time info occurred error\n");
		return;
	}

	// Copying the Mode
	int chmodSuccess = chmod(destPath, statBuff.st_mode);
	if(chmodSuccess < 0){
		printf("Copying mod info occurred error\n");
		return;
	}
	
	// Copying the uid, gid
	int chownSuccess = chown(destPath, statBuff.st_uid, statBuff.st_gid);
	if(chownSuccess< 0){
		printf("Copying owner info occurred error\n");
		return;
	}
	// Previous code
	// Displaying modified time(in Linux timestamp) to YYYY/MM/DD HH:MM:SS
	//statSuccess = stat(destPath, &statBuff);
	//if(statSuccess < 0){
	//	printf("Error Occurred while loading stat information\n");
	//	return;
	//}
	//
	// Updated code (just removed them)

	timeInfo = localtime(&statBuff.st_mtime);
	
	printf("attributes of file \"%s\"\n", srcPath);
	printf("\tst_dev = %ld\n",statBuff.st_dev);
	printf("\tst_mode = %o\n",statBuff.st_mode);
	printf("\tst_uid = %d\n", statBuff.st_uid);
	printf("\tst_gid = %d\n", statBuff.st_gid);
	printf("\tst_size = %ld\n", statBuff.st_size);
	printf("\tst_mtime = %ld\n", statBuff.st_mtime);
	printf("\tmodified time = %02d/%02d/%02d %02d:%02d:%02d\n", timeInfo->tm_year+1900,timeInfo->tm_mon+1,timeInfo->tm_mday,timeInfo->tm_hour,timeInfo->tm_min, timeInfo->tm_sec);
	printf("File attribures were successfully copied.\n");

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
