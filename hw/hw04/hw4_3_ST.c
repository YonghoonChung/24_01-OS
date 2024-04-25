#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <pthread.h>

#include "Console.h"

#define TRUE 1
#define FALSE 0

#define ESC 27

#define	MULTITHREAD
#ifdef	MULTITHREAD

#define MAX_THREAD 16

typedef struct {
	int idx;
	int no_thread;

	int width;
	int cy;
	int bar_len;
	int delay;
} ThreadParam;

void* ThreadFn(void *vparam);

int cont = TRUE;		// use this variable to terminate threads

#endif	//	MULTITHREAD

int DrawBar(int sx, int sy, int len, char c);

int main(int argc, char *argv[])
{
#ifdef	MULTITHREAD
	if(argc < 2){
		printf("Usage: %s <no_thread>\n", argv[0]);
		return 0;
	}
	int no_thread = atoi(argv[1]);
#endif	//	MULTITHREAD

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	clrscr();
	gotoxy(1, 1);
	printf("screen size = %d x %d\n", screen_width, screen_height);

	gotoxy(1, screen_height);
	printf("Press ESC to quit.\n");

//	EnableCursor(FALSE);

//	Convert the following code to animate bouncing bars using multiple threads

	int bar_len = 7;
	int x = (screen_width - bar_len) / 2;
	int dx = 1;
#ifdef MULTITHREAD
	int cy = screen_height / 2;
#else
	int cy = screen_height / 2;
#endif
	
	while(1){
		int prevx = x;

		if(x + dx < 1 || x + dx + bar_len - 1 > screen_width) // bouncing algorithm
			dx = -dx;
		x += dx;

		if(dx > 0)
			gotoxy(prevx, cy);
		else
			gotoxy(prevx + bar_len - 1, cy);
		putchar(' ');
		DrawBar(x, cy, bar_len,'=');
		
		gotoxy(1, screen_height);
		fflush(stdout);

		usleep(10000);

		if(kbhit() && getch() == ESC)	// breaks if the user presses ESC
			break;
	}
// Up to here

//	EnableCursor(TRUE);
	gotoxy(1, screen_height);
	printf("Bye!                             \n");

	return 0;
}

int DrawBar(int sx, int sy, int len, char c)
{
	gotoxy(sx, sy);
	for(int i = 0; i < len; i++)
		putchar(c);
}

#ifdef	MULTITHREAD
void* ThreadFn(void *vparam)
{
	ThreadParam *param = (ThreadParam*) vparam;

	// TO DO: implement this function.

}
#endif	//	MULTITHREAD

