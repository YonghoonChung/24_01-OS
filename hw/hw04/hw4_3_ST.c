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
int main(int argc, char *argv[]){
#ifdef	MULTITHREAD
	if(argc < 2){
		printf("Usage: %s <no_thread>\n", argv[0]);
		return 0;
	}
	int no_thread = atoi(argv[1]);
	if(no_thread > MAX_THREAD){
		printf("Use the number of thread lower than 16\n");
		return 0;
	}
#endif	//	MULTITHREAD

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;
	clrscr();
	gotoxy(1, 1);
	printf("screen size = %d x %d\n", screen_width, screen_height);

	gotoxy(1, screen_height);
	printf("Press ESC to quit.\n");

	EnableCursor(FALSE);

	//	Convert the following code to animate bouncing bars using multiple threads

	int bar_len = 7;
	int cy = screen_height / 2;

#ifdef MULTITHREAD
	pthread_t tid[no_thread];
	ThreadParam param[no_thread];
	for(int t = 0; t<no_thread; t++){
		param[t].idx = t;
		param[t].no_thread = no_thread;
		param[t].width = screen_width;
		param[t].cy = (screen_height * (t+1)) / (no_thread + 1);
		param[t].bar_len = bar_len ;
		param[t].delay = 3000 * (no_thread - t);

		pthread_create(&tid[t], NULL, ThreadFn, &param[t]);
	}

	while(cont){
		if(kbhit() && getch() == ESC){	// breaks if the user presses ESC
			cont = 0;
			//break;
		}
	}
	for(int t = 0; t<no_thread; t++){
		pthread_join(tid[t], NULL);
	}
#endif
	// Up to here
	EnableCursor(TRUE);
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
void* ThreadFn(void *vparam){
	ThreadParam *param = (ThreadParam*) vparam;

	// TO DO: implement this function.

	int screen_height = getWindowHeight() - 3;
	int x = (param->width - param->bar_len) / 2;
	int dx = 1;
	while(cont){
		int prevx = x;

		if(x + dx < 1 || x + dx + param->bar_len - 1 > param->width) // bouncing algorithm
			dx = -dx;
		x += dx;
		
		LockDisplay();
		if(dx > 0)
			gotoxy(prevx, param->cy);
		else
			gotoxy(prevx + param->bar_len - 1, param->cy);
		putchar(' ');
		DrawBar(x,param->cy, param->bar_len,'=');

		gotoxy(1, screen_height);
		fflush(stdout);
		
		UnlockDisplay();
		usleep(param->delay);

	}
}
#endif	//	MULTITHREAD

