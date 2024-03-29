// The code were relatively perfect, but I added few comment to make the code more neat.
/*
1. Reads a text line.
2. mark the start and end indices of each word.
3. Similar to the hw1_2
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 256

typedef struct{
	int start,end;
	char *string;
}Word;


// previous code
//Word word[10]; // We thought that words might now be enough, therefore change it to 50.

// updated code
Word word[50];

// previous code
//int noWords = 0; // rather than making the global variable, use the local variable

// void words(char text[], int size);
int words(char text[], int size);

int main(){
	int start = 0, end = 0; 
	char text[MAX_LEN]="";
	printf("Input a text line : ");
	fgets(text, MAX_LEN, stdin);			// When we use fgets, all character including the space gets saved and the end of the string it leaves the '\0'.
	text[strlen(text)-1]=0;
	
	printf("input text = %s\n", text);
	
	// previous code
	// words(text, strlen(text));
	
	// updated code
	int noWords = words(text,strlen(text));
	printf("             ");
	
	//updated code 
	for (int i = 0; i<word[0].start;i++){ 	// It is not critical but added for the space in the beginning
		printf(" ");
	}
	for (int i = 0; i<noWords; i++){
		printf("[");
		for(int j = 0; j<word[i].end-word[i].start-1;j++){
			printf(" ");
		}
		printf("]");
		for(int k =0; k<word[i+1].start-word[i].end-1;k++){
			printf(" ");
		}
	}
	printf("\n");
	
	for(int i = 0; i<noWords; i++){
		printf("words[%d] = (%d, %d, %s)\n", i, word[i].start,word[i].end,word[i].string);
	}

	for(int i =0; i<noWords;i++){
		free(word[i].string);
	}

	return 0;
}
//previous code
//void words(char text[], int size){

//updated code
int words(char text[], int size){
	// previous code
	// int first = 0;
	 
	//Updated code
	int first = 0, noWords = 0;	  	// first = starting character of the line
	int start = 0, end = 0;       	// start = start of the each word, end = end of the each word

	//printf("%s %d", text, size);
	
	if(text[0] != ' '){
		start = 1;
		if(text[1] == ' ')
			end = 1;
	}

	for(int i = 0; i<size;i++){
		if(i != size-1){
			//printf("%c\n", text[i]);
			if(text[i]==' '&& text[i+1] ==' '){
				start = 0;
				end = 0;
			}
			else if (text[i] == ' '&& text[i+1]!= ' '){
				start = 1;
				first = i+1;
			}
			else if (text[i] != ' ' && text[i+1] == ' '){
				end = 1;
			}
			
			if(start && end){
				start = 0;
				end = 0;
				word[noWords].start = first;
				word[noWords].end = i+1;
				word[noWords].string = malloc((i+1-first+1)*sizeof(char));
				strncpy(word[noWords].string, text + first, i+1- first);
				word[noWords].string[i+1 - first] = '\0';
				noWords++;
			}	
		}
		else{
			if(!start) continue;
			word[noWords].start = first;
			word[noWords].end = i+1;
			word[noWords].string = malloc((i+1-first+1)*sizeof(char));
			strncpy(word[noWords].string, text + first, i+1- first + 1);
			word[noWords].string[i+1 - first + 1] = '\0';
			noWords++;


		}
	}
	// previous code
	// return;

	// updated code
	return noWords;
}
