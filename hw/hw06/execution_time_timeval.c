#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    if(argc == 1){
        printf("Usage: %s <delay_in_sec>\n", argv[0]);
        return 0;
    }
    
    float delay = atof(argv[1]);
    
    struct timeval stime, etime;
    
    gettimeofday(&stime, NULL);   // get time before before the code
    
    // replace usleep() with the code to measure execution time
    usleep((int)(delay * 1000000)); // wait for a while to simulate the code
    
    
    
    gettimeofday(&etime, NULL);   // get time after executing the code
    
    float time_in_sec = etime.tv_sec - stime.tv_sec + (etime.tv_usec - stime.tv_usec) / 1000000.F;
    
    printf("elapsed time = %f\n", time_in_sec);
    
    return 0;
}

