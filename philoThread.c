/* * * * * * * * * * * * * * * * *
 * Vargha Hokmran  SID: 11543295 *
 * CS-360    Systems Programming *
 * Assignment -7         Threads *
 * Dining Philosophers'  Problem *
 * * * * * * * * * * * * * * * * */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>      // for the Gaussian function
# include <unistd.h>    // for sleep
# include <errno.h>
# include <string.h>    // for errno details
# include <time.h>      // using time too seed srand
# include <pthread.h>   // for threads, also don't forget -lpthread for compilation

# define EAT_MEAN     9
# define EAT_STDEV    3
# define THINK_MEAN   11
# define THINK_STDEV  7
# define PHILO_NUM    5
# define CHOP_NUM     5
# define TIME_OVER    100
# define _POSIX_SOURCE

int randomGaussian_r(int mean, int stddev, unsigned int* state);
void* philoFunc(void* p);
void eat(int sec, int ph);
void think(int sec, int ph);
int leftChop(int ph);
int rightChop(int ph);
int chopArray[5];
pthread_mutex_t mutex;

/* ********** MAIN ********** */
int main()
{
  int ph[5]; 
  for(int i=0; i<5; i++) {
    ph[i]=i; 
    chopArray[i]=0;
  }
  pthread_mutex_init(&mutex, NULL);
   // initialize the thread and pass its values (philo number)
  if(pthread_create(&thread0, NULL, philoFunc, &ph[0])) {
    fprintf(stderr, "Error creating thread 0\n"); 
    return 1;
  }
  if(pthread_create(&thread1, NULL, philoFunc, &ph[1])) {
    fprintf(stderr, "Error creating thread 1\n"); 
    return 1;
  }
  if(pthread_create(&thread2, NULL, philoFunc, &ph[2])) {
    fprintf(stderr, "Error creating thread 2\n"); 
    return 1;
  }
  if(pthread_create(&thread3, NULL, philoFunc, &ph[3])) {
    fprintf(stderr, "Error creating thread 3\n"); 
    return 1;
  }
  if(pthread_create(&thread4, NULL, philoFunc, &ph[4])) {
    fprintf(stderr, "Error creating thread 4\n"); 
    return 1;
  }

  if(pthread_join(thread0, NULL)) {
    fprintf(stderr, "Error joining thread 0\n"); 
    return 2;
  }
  if(pthread_join(thread1, NULL)) {
    fprintf(stderr, "Error joining thread 1\n"); 
    return 2;
  }
  if(pthread_join(thread2, NULL)) {
    fprintf(stderr, "Error joining thread 2\n"); 
    return 2;
  }
  if(pthread_join(thread3, NULL)) {
    fprintf(stderr, "Error joining thread 3\n"); 
    return 2;
  }
  if(pthread_join(thread4, NULL)) {
    fprintf(stderr, "Error joining thread 4\n"); 
    return 2;
  }

  return 0;
}

/* ********** ********** ********** ********** ********** */
/* ********** FUNCTIONS ********** */

/* ********** PHILOSOPHER'S MAIN FUNCTION ********** */
void* philoFunc(void* p)
{
  int ph= *((int*)p);
//  int ph = *p;
  srand(time(NULL));
  unsigned int myRand = rand();
  int eatSum=0, thinkSum=0, eatTime, thinkTime;
  while (eatSum < TIME_OVER)
  {
    eatTime = randomGaussian_r(EAT_MEAN, EAT_STDEV, &myRand);
    if (eatTime<0)  eatTime=0;
    eat (eatTime, ph);
    eatSum = eatSum + eatTime;

    myRand = rand();
    thinkTime = randomGaussian_r(THINK_MEAN, THINK_STDEV, &myRand);
    if (thinkTime<0)  thinkTime=0;
    think (thinkTime, ph);
    thinkSum = thinkSum + thinkTime;
  }
  printf("Philosopher %d ate for %2d seconds, thought for %2d seconds, and left the table. (PID: %d)\n", ph, eatSum, thinkSum, getpid());
  return NULL;
}

/* ********** EAT ********** */
void eat(int sec, int ph)
{
  while (1)
  {
    pthread_mutex_lock(&mutex );
    if (chopArray[leftChop(ph)]==1 || chopArray[rightChop(ph)]==1) // if any of the 2 chopstics is in use
    {
      pthread_mutex_unlock(&mutex);
      sleep(1);
    }
    else
      break;
  }
  chopArray[leftChop(ph)] = chopArray[rightChop(ph)] = 1;  // Grab the chopsticks
  pthread_mutex_unlock(&mutex);                            // Release the access to chopsticks
  printf("Philosopher %d eats for %d seconds.\n", ph, sec);
  sleep(sec);
  chopArray[leftChop(ph)] = chopArray[rightChop(ph)] = 0;  // Release the chopsticks
  return;
}

/* ********** THINK ********** */
void think(int sec, int ph)
{
  printf("Philosopher %d thinks for %d seconds.\n", ph, sec);
  sleep(sec);
}

/* ********** LEFT CHOPSTICK INDEX ********** */
int leftChop(int ph){  return ((ph)%5);}

/* ********** RIGHT CHOPSTICK INDEX ********** */
int rightChop(int ph){  return ((ph+1)%5);}


/* successive calls to randomGaussian produce integer return values */
/* having a gaussian distribution with the given mean and standard  */
/* deviation.  Return values may be negative.                       */
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
