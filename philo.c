//Daniel Maida
//CS 360
//Dining Philosophers Problem with threads.

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
#include "random_r.h"

#define NUM_PHILO 5
#define TIME_TO_EAT 100

pthread_mutex_t mutex;
int chopsticks[NUM_PHILO];

void getChopSticks(int threadID, int left, int right){

	printf("Philosopher %i is waiting for chopsticks %i and %i\n", threadID, left, right);
	while(true){
		if(pthread_mutex_lock(&mutex) != 0){
			fprintf(stderr, "Error locking the mutex\n");
			exit(1);
		}
		if (chopsticks[left]==-1 && chopsticks[right]==-1){
			chopsticks[left] = chopsticks[right] = threadID;
			if(pthread_mutex_unlock(&mutex) != 0){
				fprintf(stderr, "Error unlocking mutex after picking up chopsticks\n");
				exit(1);
			}
			return;
		}
		if(pthread_mutex_unlock(&mutex) != 0){
			fprintf(stderr, "Error unlocking mutex after not being able to pick up both chopsticks\n");
			exit(1);
		}
		sleep(1);
	}
}

void* eatOrThink(void * threadid) {
	int threadID = *((int *)threadid);
	unsigned int state = (unsigned int)threadID;
	int time_eating = 0;
	int time_thinking = 0;

	while (time_eating < TIME_TO_EAT) {
		int time_to_think = randomGaussian_r(11,7,&state);
		if (time_to_think < 0 ) {
			time_to_think = 0;
		}
		printf("Philosopher %i thinking for %i seconds (total %i)\n", threadID,time_to_think, time_thinking);
		sleep(time_to_think);
		time_thinking += time_to_think;

		int left = threadID;
		int right = (threadID + 1) % NUM_PHILO;

		int time_to_eat = randomGaussian_r(9,3,&state);
		if (time_to_eat < 0){
			time_to_eat = 0;
		}

		getChopSticks(threadID, left, right);

		printf("Philosopher %i eating for %i seconds (total %i)\n", threadID, time_to_eat, time_eating);
		sleep(time_to_eat);
		time_eating += time_to_eat;
		chopsticks[left] = chopsticks[right] = -1;
		printf("Philosopher %i is putting down %i and %i\n", threadID, left, right);
	}
	printf("Philosopher %i is leaving the table (Total Eating: %i) (Total Thinking: %i)\n", threadID, time_eating, time_thinking);
	free(threadid);
	return 0;

}

void philo(){
	pthread_t threads[NUM_PHILO];

	for (int i = 0; i < NUM_PHILO; i++) {
		chopsticks[i] = -1;
	}

	for(int i = 0; i < NUM_PHILO; i++){
		int *pt = malloc(sizeof(*pt));
		*pt = i;
		if(pthread_create(&threads[i], NULL, eatOrThink, pt)){
			fprintf(stderr, "ERROR; return code from pthread_create() \n");
			exit(1);
		}
	}
	for (int i = 0; i < NUM_PHILO; i++){
		if(pthread_join(threads[i], NULL) != 0){
			fprintf(stderr, "Failed to join thread: %i\n", i);
			exit(1);
		}
	}
}

int main(int argc, char* argv[]){
	philo();
	return 0;
}
