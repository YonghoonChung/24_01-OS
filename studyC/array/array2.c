#include <stdio.h>

int main(){
	int a[10];
	int i = 0;
	for (i = 0; i<10;i++){
		a[i] = i*i;
		printf("a[%d] = %d\n",i,a[i]);
	}
}
