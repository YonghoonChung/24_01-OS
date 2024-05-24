#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc == 1){
        printf("Usage: %s <delay_in_sec>\n", argv[0]);
        return 0;
    }
    
    float delay = atof(argv[1]);
    
    struct timespec stime, etime;
    
    clock_gettime(CLOCK_MONOTONIC, &stime);   // get time before before the code
    
    // replace usleep() with the code to measure execution time
    usleep((int)(delay * 1000000)); // wait for a while to simulate the code
    
    
    clock_gettime(CLOCK_MONOTONIC, &etime);   // get time after executing the code
    
    float time_in_sec = etime.tv_sec - stime.tv_sec + (etime.tv_nsec - stime.tv_nsec) / 1000000000.F;
    
    printf("elapsed time = %f\n", time_in_sec);
    
    return 0;
}

