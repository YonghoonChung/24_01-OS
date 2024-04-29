/*
	Conway's Game of Life
	See https://namu.wiki/w/%EC%BD%98%EC%9B%A8%EC%9D%B4%EC%9D%98%20%EC%83%9D%EB%AA%85%20%EA%B2%8C%EC%9E%84
*/

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <pthread.h>

#include "Console.h"

#define TRUE 1
#define FALSE 0

#define MAX(x,y) ((x)>=(y) ? (x) : (y))
#define MIN(x,y) ((x)<=(y) ? (x) : (y))

#define ESC 27

#define MAX_WIDTH 1024
#define MAX_HEIGHT 1024

char environ[2][MAX_HEIGHT][MAX_WIDTH] = { 0 };

void ClearMargin(int width, int height, char env[MAX_HEIGHT][MAX_WIDTH]);
void InitializeEnv(int width, int height, char env[2][MAX_HEIGHT][MAX_WIDTH]);
void UpdateEnv(int width, int height, char env[2][MAX_HEIGHT][MAX_WIDTH], int cur);
int CountNeighbor(int x, int y, char env[MAX_HEIGHT][MAX_WIDTH]);
void DisplayEnv(int width, int height, char env[MAX_HEIGHT][MAX_WIDTH]);

#define	MULTITHREAD
//#define DEBUG


#ifdef	MULTITHREAD
#define MAX_THREAD 64

typedef struct {
	int idx;
	int no_thread;
	int width, height;				// the width and height of the screen
	char (*env)[MAX_HEIGHT][MAX_WIDTH];
	int cur;						// the index of the current environment (0 or 1)
} ThreadParam; 

void UpdateEnv_MT(int width, int height, char env[2][MAX_HEIGHT][MAX_WIDTH], int cur, int no_thread);
void* ThreadFn(void *vparam);
#ifdef DEBUG
int test = 0;
#endif 
#endif	//	MULTITHREAD

int main(int argc, char *argv[])
{
#ifdef	MULTITHREAD
	if(argc == 1){
		printf("Usage: %s <no_thread>\n", argv[0]);
		return 0;
	}
	int no_thread = atoi(argv[1]);
	if (no_thread>MAX_THREAD){
		printf("Use the number of thread lower than %d\n", MAX_THREAD);
		return 0;
	}
#endif	//	MULTITHREAD

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	clrscr();
	gotoxy(1, 1);
	printf("Screen size = %d x %d\n", screen_width, screen_height);

	InitializeEnv(screen_width, screen_height, environ); // waiting until ESC
	gotoxy(1, screen_height + 1);
	printf("Press ESC to stop.\n");

	int cur = 0;
	EnableCursor(FALSE);
	while(1){
		DisplayEnv(screen_width, screen_height, environ[cur]);
		usleep(100000);

		if(kbhit()){
			if(getch() == ESC){
				break;
			}
		}

#ifdef	MULTITHREAD
		UpdateEnv_MT(screen_width, screen_height, environ, cur, no_thread);
#else	//	MULTITHREAD
		UpdateEnv(screen_width, screen_height, environ, cur);
#endif	//	MULTITHREAD
		cur = 1 - cur;
	}
	EnableCursor(TRUE);
	gotoxy(1, screen_height + 1);
	printf("Bye!                    \n");

	return 0;
}

void InitializeEnv(int width, int height, char env[2][MAX_HEIGHT][MAX_WIDTH])
{
	int x = 0, y = 0;
	for(y = 0; y <= height + 1; y++){
		for(x = 0; x <= width + 1; x++){
			env[0][y][x] = env[1][y][x] = ' ';
		}
	}

	char (*env0)[MAX_WIDTH] = env[0];
	x = width / 2;
	y = height / 2;

	while(1){
		int oldx = x;
		int oldy = y;

		gotoxy(x, y);
		putchar(env0[y][x]);

		int key = getch();
		switch(key){
		case 'h':
			x = MAX(x - 1, 1);
			break;
		case 'j':
			y = MIN(y + 1, height);
			break;
		case 'k':
			y = MAX(y - 1, 1);
			break;
		case 'l':
			x = MIN(x + 1, width);
			break;
		case ' ':
			if(env0[y][x] == '*')
				env0[y][x] = ' ';
			else
				env0[y][x] = '*';
			break;
		}

		if(key == ESC)
			break;
	}
}

void UpdateEnv(int width, int height, char env[2][MAX_HEIGHT][MAX_WIDTH], int cur)
{
	int next = 1 - cur; // 실질적인 cur은 바뀌지 않는다.
	for(int y = 1; y <= height; y++){
		for(int x = 1; x <= width; x++){
			int count = CountNeighbor(x, y, env[cur]);
			switch(count){
			case 2:
				env[next][y][x] = env[cur][y][x]; // no change
				break;

			case 3:
				env[next][y][x] = '*'; // add a star
				break;

			default:
				env[next][y][x] = ' '; // kill
				break;
			}
		}
	}
}

int CountNeighbor(int x, int y, char env[MAX_HEIGHT][MAX_WIDTH])
{
	int n = 0;
	for(int i = y - 1; i <= y + 1; i++){
		for(int j = x - 1; j <= x + 1; j++){
			if(env[i][j] == '*')
				n++;
		}
	}

	if(n > 0 && env[y][x] == '*')
		n--;

	return n;
}

void DisplayEnv(int width, int height, char env[MAX_HEIGHT][MAX_WIDTH])
{
	LockDisplay();

	EnableCursor(FALSE);

	for(int y = 1; y <= height; y++){
		gotoxy(1, y);
		for(int x = 1; x <= width; x++)
			putchar(env[y][x]);
	}
	fflush(stdout);

	//EnableCursor(TRUE);

	UnlockDisplay();
}

#ifdef	MULTITHREAD

void UpdateEnv_MT(int width, int height, char env[2][MAX_HEIGHT][MAX_WIDTH], int cur, int no_thread)
{
	// TO DO: implement this function.
	
	pthread_t tid[no_thread];
	ThreadParam param[no_thread];
	for(int t = 0; t<no_thread; t++){
		param[t].idx = t;
		param[t].no_thread = no_thread;
		param[t].width = width;
		param[t].height = height;
		param[t].env = env;
		param[t].cur = cur;

		if(pthread_create(&tid[t], NULL, ThreadFn, &param[t])){
			printf("Error occured while creating thread\n");
			return;
		}
	}

	for(int t = 0; t<no_thread; t++){
		pthread_join(tid[t], NULL);
	}
#ifdef DEBUG
	test++;
#endif
}

void* ThreadFn(void *vparam)
{
	// TO DO: implement this function.

	ThreadParam *param = (ThreadParam*)vparam;
	int next = 1 - param->cur;
	for(int y = 1 + param->idx; y<= param->height; y+= param->no_thread){
		for(int x = 1; x <= param->width; x++){
			int count = CountNeighbor(x, y, param->env[param->cur]);
			switch(count){
			case 2:
				param->env[next][y][x] = param->env[param->cur][y][x]; // no change
				break;

			case 3:
				param->env[next][y][x] = '*'; // add a star
				break;

			default:
				param->env[next][y][x] = ' '; // kill
				break;
			}
		}
#ifdef DEBUG
	printf("%d) test %d ", param->idx, test);
#endif
	}
	return NULL;
}

#endif	//	MULTITHREAD
