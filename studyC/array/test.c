#include <stdio.h>

int main(){

	int arraySize = 4;
	int test[arraySize];
	
	printf("%ld\n", sizeof(test)/sizeof(test[0]));

	for (int i = 0; i<4;i++){
		printf("%d\n", test[i]);
	}
	printf("***********************\n");

	printf("%p %p\n\n\n\n\n",test,&test[0]);

	for(int j = 0; j<10;j++){
		printf("a + %d = %p\t",j, test+j);
		printf("*(a+%d)=%d,a[%d]=%d\n", j, *(test+j),j,test[j]);
	}

	return 0;

}
