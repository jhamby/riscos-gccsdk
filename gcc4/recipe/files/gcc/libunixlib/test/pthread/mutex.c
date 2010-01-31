#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>


/* Define this to print lots of debug info */
#define LOTS_OF_DEBUG

/* A global variable to fool the compiler into not optimising out the timewaste() routine */
int sum = 0;
/* A little counter, perhaps this could do with a mutex at some point? It does get written after all */
int threads_made = 0;

extern int __pthread_num_running_threads;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* new_thread(void* arg);

#define MAX_FAC 17

int res[MAX_FAC];


int threadunsafe_factorial(int num)
{
	int i;
	clock_t t;

	sum = 1;

	printf("calculating factorial %d...\n",num);
	for (i=1;i<=num;i++) {
		sum*=i;
	    t=clock();
		while (clock()-t<1);
	}
	printf("finished: factorial %d is %d\n",num,sum);

	return sum;
}

int main(int argc,char **argv)
{
	pthread_t thread;
	int i;
	int thread_count;
	int now;
	char temp[256];
	pthread_mutex_t test;

	if (argc == 2) {
		now = atoi(argv[1]);
	} else {
		now = (int)clock();
	}
	srand(now);
	sprintf(temp,"%d",now);
	setenv("pthread$seed",temp,1);

	pthread_mutex_init(&test, NULL);
	if (memcmp(&test, &mutex, sizeof(pthread_mutex_t)) != 0) {
		printf("Error: static initialization does not match dynamic\nFAIL\n");
		exit(1);
	}

	i=0;

	for (i=0;i<MAX_FAC;i++) res[i]=threadunsafe_factorial(i);

	printf("Pthread test 1\nCreating lots of threads, this may take a while\n");
#ifdef LOTS_OF_DEBUG
	printf("Original thread stack at %x\n", (int) &i);
#endif
	if (pthread_create(&thread, NULL, new_thread, (void *)0xdeadbeef))
	{
		printf("Error: creating new thread\nFAIL\n");
		exit(0);
	}

	pthread_create(&thread, NULL, new_thread, (void *)0xdeadbeef);
	pthread_create(&thread, NULL, new_thread, (void *)0xdeadbeef);
	pthread_create(&thread, NULL, new_thread, (void *)0xdeadbeef);

	thread_count = 0;
	while(__pthread_num_running_threads > 1)
	{
		if (thread_count != __pthread_num_running_threads)
		{
			printf("Running threads = %d\n",__pthread_num_running_threads);
			thread_count = __pthread_num_running_threads;
		}
		else
		{
			pthread_yield(); /* Yield for a short time to give the other threads more time */
		}
	}

	printf("PASS\n");

	exit(0);
}

void* new_thread(void* arg)
{
	int i;
	int rnd = (rand() % 10) + 10;
	pthread_t thread;
#ifdef LOTS_OF_DEBUG
	char *blah;
#endif

#ifdef LOTS_OF_DEBUG
	printf("New thread was passed arg address %x\n", (int) arg);
	printf("New thread stack at %x\n", (int) &i);
#endif

	for (i=0;i<rnd;i++)
	{
		int lres;
		int num=i%MAX_FAC;
		pthread_mutex_lock(&mutex);
		lres=threadunsafe_factorial(num);
		pthread_mutex_unlock(&mutex);
		if (lres!=res[num]) {
			printf("FAIL\n");
			exit(1);
		}
		if (! (rand() & 255)  && threads_made < 127)
		{
#ifdef LOTS_OF_DEBUG
			printf("Making new thread from inside a thread\n");
#endif
			threads_made++;
			/* We won't worry if this runs out of threads or memory just now, it's not that
			   important to the test. As long as the first thread was created. */
			pthread_create(&thread, NULL, new_thread, (void *) (0xdeadbeef + threads_made));
		}
	}

#ifdef LOTS_OF_DEBUG
	printf("Trying a simple calloc and free\n");
	blah = calloc(1,32);
	/* We could fail to calloc here but it's not fatal */
	if (blah)
	{
		free(blah);
	}
#endif

	/* Randomly test the pthread_exit call or a lazy thread return */
	if (rand() & 1)
	{
		/* Force an exit now using this routine - This should not return */
		pthread_exit(NULL);
		printf("pthread_exit() returned from a thread exit call!FAIL\n");
		exit(0);
	}

#ifdef LOTS_OF_DEBUG
	printf("Thread arg %x coming to a lazy stop\n", (int) arg);
#endif

	return(NULL);
}

