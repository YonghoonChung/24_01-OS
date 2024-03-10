#include <stdio.h>

int main(){
	int a = 0, b = 0, sum = 0;
	printf("Input two integers: ");
	scanf("%d %d",&a, &b);
	sum = a+b;
	printf("%d + %d = %d\n", a,b,sum);

	return 0;
}
