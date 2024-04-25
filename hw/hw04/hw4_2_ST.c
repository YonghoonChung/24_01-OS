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

//#define	MULTITHREAD
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

#endif	//	MULTITHREAD

int main(int argc, char *argv[])
{
#ifdef	MULTITHREAD
	if(argc == 1){
		printf("Usage: %s <no_thread>\n", argv[0]);
		return 0;
	}
	int no_thread = atoi(argv[1]);
#endif	//	MULTITHREAD

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	clrscr();
	gotoxy(1, 1);
	printf("Screen size = %d x %d\n", screen_width, screen_height);

	InitializeEnv(screen_width, screen_height, environ);
	gotoxy(1, screen_height + 1);
	printf("Press ESC to stop.\n");

	int cur = 0;
	while(1){
		DisplayEnv(screen_width, screen_height, environ[cur]);
		usleep(100000);

		if(kbhit()){
			if(getch() == ESC)
				break;
		}

#ifdef	MULTITHREAD
		UpdateEnv(screen_width, screen_height, environ, cur, );
#else	//	MULTITHREAD
		UpdateEnv(screen_width, screen_height, environ, cur);
#endif	//	MULTITHREAD
		cur = 1 - cur;
	}

	gotoxy(1, screen_height + 1);
	printf("Bye!                     \n");

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
	int next = 1 - cur;
	for(int y = 1; y <= height; y++){
		for(int x = 1; x <= width; x++){
			int count = CountNeighbor(x, y, env[cur]);
			switch(count){
			case 2:
				env[next][y][x] = env[cur][y][x];
				break;

			case 3:
				env[next][y][x] = '*';
				break;

			default:
				env[next][y][x] = ' ';
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

	EnableCursor(TRUE);

	UnlockDisplay();
}

#ifdef	MULTITHREAD

void UpdateEnv_MT(int width, int height, char env[2][MAX_HEIGHT][MAX_WIDTH], int cur, int no_thread)
{
	// TO DO: implement this function.
}

void* ThreadFn(void *vparam)
{
	ThreadParam *param = (ThreadParam*)vparam;

	// TO DO: implement this function.

	return NULL;
}

#endif	//	MULTITHREAD
