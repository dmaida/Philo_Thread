
DEBUGGABLE=-g
CFLAGS = -std=c99 -Wall -pedantic $(DEBUGGABLE) -c -g
philo: philo.o
	gcc -pthread philo.o -o philo -lm
philo.o: philo.c
	gcc $(CFLAGS) philo.c
clean:
	rm -f *.o philo
