/*
 * On May 11th, we had an online meeting to discuss the fifth assignment. 
 * For question 1, we found that my code had a critical mistake. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>


#include "Console.h"


#define DEBUG			// comment out this line after completion


#define TRUE 1
#define FALSE 0

/*
 * Comment from Minyoung on the main function:
 * 		The algorithm of the code does not work in this way. The processor does not wait until the one is finished. It prints while falling.
 *
 */

void DropWord(char *word, int idx, int no_word, int width, int height);
void Erase(int sx, int sy, int len);

int main(int argc, char *argv[])
{
	if(argc == 1){
		printf("Usage: %s <word1> <word2> ...\n", argv[0]);
		return 0;
	}
	char* words[argc];
	clrscr();

	int screen_width = getWindowWidth();
	int screen_height = getWindowHeight() - 3;

	gotoxy(1, screen_height + 1);
	printf("screen size = %dx%d.\n", screen_width, screen_height);

#ifndef	DEBUG
	EnableCursor(FALSE);
#endif	//	DEBUG

	// write your code here and do not modify other parts
 
	// Previous code
	
//(old code)	for(int i = 1; i<argc;i++){
//(old code)		words[i] = argv[i]; 
//(old code)	}
//(old code)	int no_word = argc -1;
//(old code)	for(int i = 0; i<argc-1;i++){
//(old code)		int child = fork();
//(old code)		if(child > 0){
//(old code)			wait(NULL);
//(old code)		}else if(child == 0){
//(old code)			DropWord(words[i+1],i+1,no_word, screen_width, screen_height);
//(old code)			exit(-1);
//(old code)		}else{
//(old code)			printf("Error in fork()\n");
//(old code)			return 0;
//(old code)		}
//(old code)	}

	//New code
	
	pid_t pid;
    for (int i = 1; i < argc; i++){
        pid = fork();
        if (pid == 0){
            DropWord(argv[i], i, argc - 1, screen_width, screen_height + 1);
            exit(0);
        }
        else if (pid < 0){
            fprintf(stderr, "Failed to fork\n");
            exit(1);
        }
        usleep(100000);
    }


    while (wait(NULL) > 0);


	gotoxy(1, screen_height + 2);
	printf("Bye!\n");
//	for(int i =1; i<argc;i++){
//		printf("%s\n", words[i]);
//	}
#ifndef	DEBUG
	EnableCursor(TRUE);
#endif	//	DEBUG

	return 0;
}

void DropWord(char *word, int idx, int no_word, int width, int height)
// drops a word from top to bottom.
// read the code to understand the parameters
{
	int len = strlen(word);
	int x = width * idx / (no_word + 1);
	for(int y = 1; y < height; y++){

		LockDisplay();

		if(y > 1)
			Erase(x, y - 1, len);

		gotoxy(x, y);
		printf("%s", word);

		fflush(stdout);

		UnlockDisplay();

		usleep(100000);
	}
}

void Erase(int sx, int sy, int len)
{
	gotoxy(sx, sy);
	for(int i = 0; i < len; i++)
		putchar(' ');

