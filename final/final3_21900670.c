#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

#include "Console.h"

#define	MULTI_THREAD		// uncomment this line to solve the problem

#define MIN(X,Y) ((X) <= (Y) ? (X) : (Y))

#define NO_THREAD 2
#define BUFFER_SIZE 256

void Tab2Space(char *src, int max_len, int tab_size);
void DisplaySubscreen(char *buffer, int sx, int width);

#ifdef	MULTI_THREAD

typedef struct {
	char *filename;
	int sx;
	int width;
	char buffer[BUFFER_SIZE];
} ThreadParam;

void* ThreadFn(void *vparam);
char buffer[BUFFER_SIZE] = "";

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#else	//	MULTI_THREAD

char buffer[BUFFER_SIZE] = "";
#endif	//	MULTI_THREAD

int main(int argc, char *argv[])
{
	if(argc < 3){
		printf("Usage: %s <file1> <file2>\n", argv[0]);
		return 0;
	}

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;


	buffer[BUFFER_SIZE - 1] = 0;		// for safety
#ifdef	MULTI_THREAD

	char *filename1 = argv[1];
	char *filename2 = argv[2];
	pthread_t tid[NO_THREAD];
	ThreadParam param[NO_THREAD];

	for(int t =0; t < NO_THREAD; t++){
		param[t].filename = argv[t+1];
		param[t].sx = (t==0)? 0 : screen_width/2;
		param[t].width = (t==0) ? screen_width/2 : screen_width;
		param[t].buffer[BUFFER_SIZE] = 0;

		pthread_create(&tid[t], NULL, ThreadFn, &param[t]);
	}
	for(int t = 0; t<NO_THREAD; t++){
		pthread_join(tid[t], NULL);
	}
#else	//	MULTI_THREAD

	char *filename1 = argv[1];

	FILE *fp = fopen(filename1, "r");
	if(fp == NULL){
		printf("Failed to open file %s\n", filename1);
		exit(-1);
	}
	int count = 0;
	while(fgets(buffer, BUFFER_SIZE - 1, fp2)){
		Tab2Space(buffer, BUFFER_SIZE, 4);
		DisplaySubscreen(buffer, screen_width / 4, screen_width / 2);

		// to make each thread display 5 lines at each tune alternately
		count++;
		if(count % 5 == 0)
			usleep(1);
	}


	fclose(fp);
#endif	//	MULTI_THREAD

	printf("Bye!\n");

	return 0;
}


#ifdef	MULTI_THREAD

void* ThreadFn(void *vparam)
{
	ThreadParam *param = (ThreadParam *)vparam;
	char *filename = param->filename;
	int sx = param -> sx;
	int width = param -> width;
	char buffer[BUFFER_SIZE];
	FILE *fp = fopen(filename, "r");
	if(fp == NULL){
		printf("Failed to open file %s\n", filename);
		pthread_exit(NULL);
		exit(-1);
	}
	int count = 0;	
	while(fgets(buffer,BUFFER_SIZE -1, fp)){

		pthread_mutex_lock(&mutex);
		Tab2Space(buffer, BUFFER_SIZE, 4);
		DisplaySubscreen(buffer,sx,width);

		pthread_mutex_unlock(&mutex);
		count ++;
		if(count %5 == 0)
			usleep(1);
	}
	fclose(fp);
	pthread_exit(NULL);
}

#endif	//	MULTI_THREAD


#ifndef	MULTITHREAD					// DO NOT modify this line

void Tab2Space(char *str, int max_len, int tab_size)
// in-place algorithm to replace tab characters with spaces
{
	int no_tab = 0;
	int src_len = 0;
	for(src_len = 0; str[src_len] != 0; src_len++)
		if(str[src_len] == '\t')
			no_tab++;

	if(no_tab == 0)			// nothing to do
		return;

	int dest_len = src_len + no_tab * (tab_size - 1);
	if(max_len < dest_len + 1){
		printf("max_len (%d) is too short (%d + %d * %d = %d)\n", max_len, src_len, no_tab, tab_size, dest_len);
		return;
	}

	int p1 = src_len - 1;
	int p2 = dest_len - 1;
	while(p1 >= 0){
		if(str[p1] == '\t'){
			for(int i = 0; i < tab_size; i++)
				str[p2--] = ' ';
		} else 
			str[p2--] = str[p1];
		p1--;
	} 

	str[dest_len] = 0;
}

#endif	//	MULTITHREAD				// DO NOT modify this line

#ifndef	MULTITHREAD					// DO NOT modify this line

void DisplaySubscreen(char *buffer, int sx, int width)
// Currently, this function was not implemented for multi-threading environments
// If a thread is switched while printing a line, characters can be print in wrong location.
//
// TO DO: make this function thread-safe (see the pdf file)
{
	int len = strlen(buffer);

	for(int pos = 0; pos < len; ){
		int limit = MIN(len, pos + width);

		for(int i = 0; i < sx; i++)
			putchar(' ');
		fflush(stdout);

		while(pos < limit)
			putchar(buffer[pos++]);
		fflush(stdout);

		if(pos > 0 && pos < len && buffer[pos - 1] != '\n')
			putchar('\n');
		fflush(stdout);
	}
}

#endif	//	MULTITHREAD				// DO NOT modify this line
