// Daniel Maida
// CS 360

#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#define NUM_PHILO 5


int randomGaussian_r(int mean, int stddev, unsigned int* state) {
	double mu = 0.5 + (double) mean;
	double sigma = fabs((double) stddev);
	double f1 = sqrt(-2.0 * log((double) rand_r(state) / (double) RAND_MAX));
	double f2 = 2.0 * 3.14159265359 * (double) rand_r(state) / (double) RAND_MAX;
	if (rand_r(state) & (1 << 5))
		return (int) floor(mu + sigma * cos(f2) * f1);
	else
		return (int) floor(mu + sigma * sin(f2) * f1);
}
void *EatOrThink(void * threadid) {
  int tid;
  tid = (long)threadid;
  unsigned int state = (unsigned int)tid;
  //int time_eating = 0;
  int time_thinking = 0;

  while (time_thinking < 50) {
    int time_to_think = randomGaussian_r(11,7,&state);
    if (time_to_think < 0 ) {
      time_to_think = 0;
    }
    printf("Philosopher %i thinking for %i seconds (total %i)\n", tid,time_to_think, time_thinking);
    sleep(time_to_think);
    time_thinking += time_to_think;
  }

  printf("Philosopher %i is leaving the table\n", tid);
  pthread_exit(NULL);
}

void *PrintHello(void *threadid) {
  long tid;
  tid = (long)threadid;
  printf("Hello World! It's me, thread #%ld!\n", tid);
  pthread_exit(NULL);
}

void philo(){

 pthread_t threads[NUM_PHILO];
 int rc;
 long t;
 for(t=0;t<NUM_PHILO;t++){
   printf("In main: creating thread %ld\n", t);
   rc = pthread_create(&threads[t], NULL, EatOrThink, (void *)t);
   if (rc != 0){
     printf("ERROR; return code from pthread_create() is %d\n", rc);
     exit(-1);
     }
   }
 /* Last thing that main() should do */
 pthread_exit(NULL);

}

int main(int argc, char* argv[]){
    philo();
    return 0;
}
