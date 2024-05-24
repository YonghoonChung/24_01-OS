#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("Usage: %s <delay_in_sec>\n", argv[0]);
		return 0;
	}
	float delay = atof(argv[1]);

	struct timespec stime, etime;
	clock_gettime(CLOCK_MONOTONIC, &stime);

	usleep((int)(delay*1000000));

	clock_gettime(CLOCK_MONOTONIC, &etime);
	float time_in_sec = etime.tv_sec - stime.tv_sec + (etime.tv_nsec - stime.tv_nsec)/1000000000.F;
	printf("elapsed time = %f\n", time_in_sec);

	return 0;
}
