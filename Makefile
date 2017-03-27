#OPTIMIZE=-03 -DNDEBUG
DEBUGGABLE=-g
CFLAGS = -std=c99 -Wall -pedantic $(DEBUGGABLE) $(OPTIMIZE) -c -g
philo: random_r.o philo.o
	gcc -pthread random_r.o philo.o -o philo -lm

philo.o: philo.c random_r.h
	gcc $(CFLAGS) philo.c

random_r.o: random_r.c random_r.h
	gcc $(CFLAGS) random_r.c

clean:
	rm -f *.o philo
	
