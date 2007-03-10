#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Define this to print lots of debug info */
#define LOTS_OF_DEBUG

volatile int temp;

extern int __pthread_num_running_threads;

void *joinee(void *arg)
{
	clock_t until;
	int i;

#ifdef LOTS_OF_DEBUG
	printf("Joinee thread arg %p starting\n", arg);
#endif

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	until = clock() + (time_t)arg + 1;
	while (clock() < until) {
		/* waste time */
		for (i=0;i<20000;i++) temp = i;
	}
#ifdef LOTS_OF_DEBUG
	printf("Joinee thread arg %p coming to a lazy stop\n", arg);
#endif

	return arg;
}

void *joiner(void *arg)
{
	clock_t until;
	int i;
	int err;
	void *status;
	pthread_t thread;
	pthread_attr_t attr;
#ifdef LOTS_OF_DEBUG
	printf("Joiner thread arg %p starting\n", arg);
#endif

	pthread_detach(pthread_self());

	pthread_attr_init(&attr);
	

	err=pthread_create(&thread, &attr, joinee, arg);
	if (err) {
		printf("Joiner thread %p Error: creating new thread %d\nFAIL\n",arg,err);
		exit(1);
	}

	pthread_attr_destroy(&attr);

	until = clock() + 150;
	while (clock() < until) {
		/* waste time */
		for (i=0;i<20000;i++) temp = i;
	}

	if (((int)arg % 3)==0) {
		pthread_cancel(thread);
	}

	until = clock() + 150;
	while (clock() < until) {
		/* waste time */
		for (i=0;i<20000;i++) temp = i;
	}

#ifdef LOTS_OF_DEBUG
	printf("Joiner thread arg %p about to join\n", arg);
#endif

	err = pthread_join(thread, &status);
	if (err != 0 || status != arg) {
		if (err != 0 || !(((int)arg % 3)==0 && status == PTHREAD_CANCELED)) {
			printf("pthread_join(%p)  for joiner %p returned %d, status %p\nFAIL\n", thread, arg, err, status);
			exit(1);
		}
	}


#ifdef LOTS_OF_DEBUG
	printf("Joiner thread arg %p exiting\n", arg);
#endif

	return NULL;
}

int main(void)
{
	pthread_t thread = NULL;
	int i;

	printf("Pthread test 2\nJoining of threads\n");

	for (i=0;i<600;i+=10) {
		if (pthread_create(&thread, NULL, joiner, (void *)i)) {
			printf("Error: creating new thread\nFAIL\n");
			exit(1);
		}
	}

	/* Yield here for a bit... So the other threads can have a chance */
	while (__pthread_num_running_threads > 1) pthread_yield();

	printf("PASS\n");

	exit(0);
}

