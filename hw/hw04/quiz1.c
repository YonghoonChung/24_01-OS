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
    char text[MAX_TEXT]="";
    printf("Input text : ");
    fgets(text,MAX_TEXT,stdin);
    int len= strlen(text)-1;
	text[len] = 0;

    for (int i=0; i<len; i++){
        for(;i<len && isspace(text[i]); i++);
        if(i==MAX_LEN) break;

        int j=0, k=0;
        for(j=i+1;j<len &&!isspace(text[j]);j++);

        words[no_word].start = i;
        words[no_word].end = j;
        for(k=0;k<j-i;k++)
            words[no_word].substr[k]=text[i+k];
        if(++no_word >= len) break;
        i=j;
    }
    printf("Totally, %d words were extracted from text \"%s\".\n", no_word, text);
    for(int i=0;i<no_word; i++){
    printf("words[%d]=(%d,%d,%s)\n",i,words[i].start, words[i].end, words[i].substr);}
    return 0;
}
