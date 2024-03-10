#include <stdio.h>

int main(){

	int score[10];

	for(int i =0; i<10;i++){
		printf("Input score of student %d :",i);
		scanf("%d",&score[i]);
	}
	for(int i=0; i<10;i++){
		printf("student%d's score : %d\n", i,score[i]);
	}
	return 0;
}
