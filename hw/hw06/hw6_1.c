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
	struct timespec stime2, etime2;
	count = 0;
	pthread_t tid2[no_thread];

	printf("Each of %d threads increased 10000 times using a mutex.\n", no_thread);	
	clock_gettime(CLOCK_MONOTONIC, &stime2);
	for(int t = 0; t<no_thread; t++){
		pthread_create(&tid2[t], NULL, ThreadFn2, &count);
	}
	for(int t = 0; t<no_thread; t++){
		pthread_join(tid2[t], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &etime2);
	time_in_sec = etime2.tv_sec - stime2.tv_sec + (etime2.tv_nsec - stime2.tv_nsec)/1000000000.F;
	printf("count = %d, elapsed time = %f sec\n", count, time_in_sec);
	
	pthread_mutex_destroy(&mutex);
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
	for(int i = 0; i<10000; i++){
		pthread_mutex_lock(&mutex);
		(*p)++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
