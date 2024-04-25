#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT 256
#define MAX_LEN 128
#define SIZE 512

typedef struct{
    int start, end;
    char substr[MAX_LEN];
}Substring;

Substring words[SIZE];
int no_word = 0;

int main(){
    char text[MAX_TEXT]=""; // 
    printf("Input text : ");
    fgets(text,MAX_TEXT,stdin);
    int len= strlen(text)-1;
	text[len] = 0;

	for (int i=0; i<len; i++){ // 0 to len
        for(;i<len && isspace(text[i]); i++); // increase i until space is found //여기서 i는 글자가 시작하는 곳에서 멈춘다 
		if(i==MAX_LEN) break; // 여기는 언제 올까?

        int j=0, k=0;
        for(j=i+1;j<len &&!isspace(text[j]);j++); // 다음 공백이 나올 때까지 j를 증가한다.

        words[no_word].start = i;
        words[no_word].end = j;
        for(k=0;k<j-i;k++)
            words[no_word].substr[k]=text[i+k];
        if(++no_word >= MAX_LEN) break;
        i=j;
    }
    printf("Totally, %d words were extracted from text \"%s\".\n", no_word, text);
    for(int i=0;i<no_word; i++){
   		printf("words[%d]=(%d,%d,%s)\n",i,words[i].start, words[i].end, words[i].substr);
	}
    return 0;
}
