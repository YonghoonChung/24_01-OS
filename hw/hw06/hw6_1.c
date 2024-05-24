#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>


void* ThreadFn1(void *vparam);
void* ThreadFn2(void *vparam);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Usage: %s <number_of_thread>\n", argv[0]);
		return 0;
	}
	int no_thread = atoi(argv[1]);
	int count = 0;	

	struct timespec stime, etime;

	// Without Mutex
	pthread_t tid[no_thread];
	printf("Each of %d threads increased 10000 times w/o mutex.\n", no_thread);	
	clock_gettime(CLOCK_MONOTONIC, &stime);
	for(int t = 0; t<no_thread; t++){
		pthread_create(&tid[t], NULL, ThreadFn1, &count);
	}
	for(int t = 0; t<no_thread; t++){
		pthread_join(tid[t], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &etime);
	float time_in_sec = etime.tv_sec - stime.tv_sec + (etime.tv_nsec - stime.tv_nsec)/1000000000.F;
	printf("count = %d, elapsed time = %f sec\n", count, time_in_sec);

	// With Mutex
	count = 0;
	printf("Each of %d threads increased 10000 times using a mutex.\n", no_thread);	
	clock_gettime(CLOCK_MONOTONIC, &stime);
	for(int t = 0; t<no_thread; t++){
		pthread_create(&tid[t], NULL, ThreadFn2, &count);
	}
	for(int t = 0; t<no_thread; t++){
		pthread_join(tid[t], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &etime);
	time_in_sec = etime.tv_sec - stime.tv_sec + (etime.tv_nsec - stime.tv_nsec)/1000000000.F;
	printf("count = %d, elapsed time = %f sec\n", count, time_in_sec);


	return 0;
}

void* ThreadFn1(void *vparam){
	int* p = (int*)vparam;
	for(int i = 0; i<10000; i++)
		(*p)++;
	return NULL;
}

void* ThreadFn2(void *vparam){
	int* p = (int*)vparam;
	pthread_mutex_lock(&mutex);
	for(int i = 0; i<10000; i++)
		(*p)++;
	pthread_mutex_unlock(&mutex);
	return NULL;
}
