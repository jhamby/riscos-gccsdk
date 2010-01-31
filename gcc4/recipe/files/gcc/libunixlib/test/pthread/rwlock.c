#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>


/* Define this to print lots of debug info */
#define LOTS_OF_DEBUG


int global = 0;
int temp;

extern int __pthread_num_running_threads;

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *new_thread(void *arg)
{
	int i,j;
	int rnd = rand() % 10;
	int ret;

#ifdef LOTS_OF_DEBUG
	printf("New thread was passed arg address %x\n", (int) arg);
	printf("New thread stack at %x\n", (int) &i);
#endif

	for (j=0;j<rnd;j++)
	{
		int varbefore;
		clock_t until;
		if (((int)arg % 10) == 0) {
			ret = pthread_rwlock_wrlock(&rwlock);
			if (ret) {
				printf("wrlock failed %d for %p\nFAIL\n",ret,arg);
				exit(1);
			}
#ifdef LOTS_OF_DEBUG
			printf("Got write lock %p\n",arg);
#endif
			varbefore = ++global;

			until = clock() + 50;
			while (clock() < until) {
				/* waste time */
				for (i=0;i<20000;i++) temp = i;
			}

			if (global != varbefore) {
				printf("Global changed while we have write lock %p\nFAIL\n",arg);
				exit(1);
			}
#ifdef LOTS_OF_DEBUG
			printf("Releasing write lock %p\n",arg);
#endif
			ret = pthread_rwlock_unlock(&rwlock);
			if (ret) {
				printf("unlock failed %d for %p\nFAIL\n",ret,arg);
				exit(1);
			}
			until = clock() + 80;
			while (clock() < until) {
				/* waste time */
				for (i=0;i<20000;i++) temp = i;
			}
		} else {
			ret = pthread_rwlock_rdlock(&rwlock);
			if (ret) {
				printf("rdlock failed %d for %p\nFAIL\n",ret,arg);
				exit(1);
			}
#ifdef LOTS_OF_DEBUG
			printf("Got read lock %p\n",arg);
#endif
			varbefore = global;

			until = clock() + 50;
			while (clock() < until) {
				/* waste time */
				for (i=0;i<20000;i++) temp = i;
			}

			if (global != varbefore) {
				printf("Global changed while we have read lock %p\nFAIL\n",arg);
				exit(1);
			}
#ifdef LOTS_OF_DEBUG
			printf("Releasing read lock %p\n",arg);
#endif
			ret = pthread_rwlock_unlock(&rwlock);
			if (ret) {
				printf("unlock failed %d for %p\nFAIL\n",ret,arg);
				exit(1);
			}

		}
	}

#ifdef LOTS_OF_DEBUG
	printf("Thread arg %x coming to a lazy stop\n", (int) arg);
#endif

	return NULL;
}


int main(int argc,char **argv)
{
	pthread_t thread;
	int i;
	int thread_count;
	int now;
	char temp[256];
	pthread_rwlock_t test;

	if (argc == 2) {
		now = atoi(argv[1]);
	} else {
		now = (int)clock();
	}
	srand(now);
	sprintf(temp,"%d",now);
	setenv("pthread$seed",temp,1);

	pthread_rwlock_init(&test, NULL);
	if (memcmp(&test, &rwlock, sizeof(pthread_rwlock_t)) != 0) {
		printf("Error: static initialization does not match dynamic\nFAIL\n");
		exit(1);
	}


	printf("Pthread rwlock test\nCreating lots of threads, this may take a while\n");
#ifdef LOTS_OF_DEBUG
	printf("Original thread stack at %x\n", (int) &i);
#endif
	for (i=0;i<30;i++) {
		if (pthread_create(&thread, NULL, new_thread, (void *)i)) {
			printf("Error: creating new thread\nFAIL\n");
			exit(1);
		}
	}

	thread_count = 0;
	while(__pthread_num_running_threads > 1) {
		if (thread_count != __pthread_num_running_threads) {
			printf("Running threads = %d\n",__pthread_num_running_threads);
			thread_count = __pthread_num_running_threads;
		} else {
			pthread_yield();
		}
	}

	printf("PASS\n");

	exit(0);
}

