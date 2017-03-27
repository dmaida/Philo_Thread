// Daniel Maida
// CS 360

#define _POSIX_SOURCE
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
#include "random_r.h"

#define NUM_PHILO 5

pthread_mutex_t mutex;
int chopsticks[NUM_PHILO];

void* EatOrThink(void * threadid) {
  int tid;
  tid = (long)threadid;
  unsigned int state = (unsigned int)tid;
  int time_eating = 0;
  int time_thinking = 0;

  while (time_thinking < 15) {
    int time_to_think = randomGaussian_r(11,7,&state);
    if (time_to_think < 0 ) {
      time_to_think = 0;
    }
    printf("Philosopher %i thinking for %i seconds (total %i)\n", tid,time_to_think, time_thinking);
    sleep(time_to_think);
    time_thinking += time_to_think;

	  int left = tid;
	  int right = (tid + 1) % NUM_PHILO;
		int time_to_eat = randomGaussian_r(9,3,&state);
		if (time_to_eat < 0){
			time_to_eat = 0;
		}

		while (1)
		{
			pthread_mutex_lock(&mutex );
			if (chopsticks[left]==1 || chopsticks[right]==1) // if any of the 2 chopsticks is in use
			{
				pthread_mutex_unlock(&mutex);
				sleep(1);
			}
			else
				break;
		}

		chopsticks[left] = 1;
		chopsticks[right] = 1;  // Grab the chopsticks
		pthread_mutex_unlock(&mutex);                            // Release the access to chopsticks
		printf("Philosopher %i eating for %i seconds (total %i)\n", tid, time_to_eat, time_eating);
		sleep(time_to_eat);
		time_eating += time_to_eat;
		chopsticks[left] = 0;
		chopsticks[right] = 0;

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

	 for (int i = 0; i < NUM_PHILO; i++) {
	 	chopsticks[i] = 0;
	 }

	for(t=0;t<NUM_PHILO;t++){
   printf("In main: creating thread %ld\n", t);
   rc = pthread_create(&threads[t], NULL, EatOrThink, (void *)t);
   if (rc != 0){
     fprintf(stderr, "ERROR; return code from pthread_create() is %d\n",rc);
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
