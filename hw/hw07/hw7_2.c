/*
	Compilation: gcc hw7_2_problem.c Console.c StringQueue.c

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>

#include <semaphore.h>

#include "Console.h"
#include "StringQueue.h"

//#define	_DEBUG

#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#define MIN(x, y) ((x) <= (y) ? (x) : (y))

#define TRUE 1
#define FALSE 0

void Parent(StringQueue *q);
void Child(StringQueue *q);

void CheckSemaphore(const char *mesg);

int screen_width = 0;
int screen_height = 0;
int sx = 0, sy = 0;

sem_t *full = NULL;
sem_t *empty = NULL;
sem_t *mutex = NULL;

int main(int argc, char *argv[])
{
	screen_width = getWindowWidth();
	screen_height = getWindowHeight() - 3;

	sx = (screen_width - MAX_LEN - 4) / 2;
	sy = MAX(1, (screen_height - QUEUE_SIZE) / 2);

	clrscr();


	StringQueue *q = NULL;
	// TO DO: allocate q as a POSIX shared memory block and initialize it using StringQueue_Init()
	char shm_name[512] = "abcd";
	int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
	if (ftruncate(shm_fd, sizeof(StringQueue)) == -1) {
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}
	q = mmap(NULL, sizeof(StringQueue), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (q == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	StringQueue_Init(q);

	// TO DO: create named semaphores full, empty, and mutex
	// 		on failure, dispaly an error message and quit
	full = sem_open("/sem_full", O_CREAT, 0666, 0);
	if (full == SEM_FAILED) {
		perror("sem_open full");
		exit(EXIT_FAILURE);
	}	
	empty = sem_open("/sem_empty", O_CREAT, 0666, QUEUE_SIZE);
	if (empty == SEM_FAILED) {
		perror("sem_open empty");
		exit(EXIT_FAILURE);
	}
	mutex = sem_open("/sem_mutex", O_CREAT, 0666, 1);
	if (mutex == SEM_FAILED) {
		perror("sem_open mutex");
		exit(EXIT_FAILURE);
	}


#ifdef	_DEBUG
	CheckSemaphore("initial values of semaphores");
#endif	//	_DEBUG


	// TO DO: create a child process
	//			the child calls Child() and exit(0)
	//			the parent calls Parent() and wait(NULL)
	pid_t pid = fork();
	if(pid == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0){
		Child(q);
		exit(0);
	} else {
		Parent(q);
		wait(NULL);
	}

	// TO DO: deallocate the semaphores and reset the pointers to NULL
	sem_close(full);
	sem_close(empty);
	sem_close(mutex);
	sem_unlink("/sem_full");
	sem_unlink("/sem_empty");
	sem_unlink("/sem_mutex");
	full = NULL;
	empty = NULL;
	mutex = NULL;

	// TO DO: deallocate the shared memory block for queue and reset q to NULL
	munmap(q, sizeof(StringQueue));
	shm_unlink(shm_name);
	q = NULL;

	gotoxy(1, screen_height);
	printf("Bye!\n");

	return 0;
}

void Parent(StringQueue *q)
{
	char input[MAX_LEN] = "";

	StringQueue_Display(q, sx, sy);

	while(1){
		gotoxy(1, screen_height);
		for(int i = 0; i < screen_width; i++)
			putchar(' ');

		gotoxy(1, screen_height);
		int ret = scanf("%s", input);

		// TO DO: implement entry section using the semaphores
		sem_wait(empty);
		sem_wait(mutex);

		StringQueue_Insert(q, input);

		// TO DO: implement exit section using the semaphores
		sem_post(mutex);
		sem_post(full);

		if(strcmp(input, "exit") == 0)
			break;

		StringQueue_Display(q, sx, sy);
	}

	PrintXY(1, 1, "Terminating Parent()\n");
}

void Child(StringQueue *q)
{
	char word[MAX_LEN] = "";

	while(full == NULL);

	while(1){
		// TO DO: implement entry section using the semaphores
		sem_wait(full);
		sem_wait(mutex);

		StringQueue_Delete(q, word);	

		// TO DO: implement exit section using the semaphores
		sem_post(mutex);
		sem_post(empty);

		if(strcmp(word, "exit") == 0)
			break;

		StringQueue_Display(q, sx, sy);

		PrintXY(1, 2, "deleted word = %-32s", word);
		gotoxy(1, screen_height);
		fflush(stdout);

		sleep(2);
	}

	PrintXY(1, 2, "Terminating Child()\n");
}

void CheckSemaphore(const char *mesg)
{
	int full_val = 0;
	int empty_val = 0;
	int mutex_val = 0;

	sem_getvalue(full, &full_val);
	sem_getvalue(empty, &empty_val);
	sem_getvalue(mutex, &mutex_val);
	if(mesg)
		printf("[%s] ", mesg);
	printf("full = %d, empty = %d, mutex = %d\n", full_val, empty_val, mutex_val);
	fflush(stdout);
}
