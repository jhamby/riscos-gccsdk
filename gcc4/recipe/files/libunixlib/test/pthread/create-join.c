#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile bool quit = false;

void *t1(void *args)
{
	while (!quit) {
		printf("Thread 1\n");
		fflush(stdout);
		sleep(3);
	}

	return NULL;
}

void *t2(void *args)
{
	while (!quit) {
		printf("Thread 2\n");
		fflush(stdout);
		sleep(2);
	}

	return NULL;
}

int main(void)
{
	pthread_t a, b;

	if (pthread_create(&a, NULL, t1, NULL) != 0)
		return 1;

	if (pthread_create(&b, NULL, t2, NULL) != 0) {
		quit = true;
		pthread_join(a, NULL);
		return 1;
	}

	while ((rand() % 25) != 5) {
		printf("Main thread\n");
		fflush(stdout);
		sleep(2);
	}

	quit = true;

	pthread_join(a, NULL);
	pthread_join(b, NULL);

	return 0;
}
