/*
 * 2024년 3월 24일
 * HW 02 assignment by prof.Injung Kim
 *
 * Problem : Write a program that copies a file into another.
 * using open, close, read, write
 */
#include <stdio.h>

int main (int argc, char *argv[]){
	for(int i = 0; i <= argc; i++){
		printf("argv[%d] = \"%s\"\n",i,argv[i]);
	}
	return 0;
}

