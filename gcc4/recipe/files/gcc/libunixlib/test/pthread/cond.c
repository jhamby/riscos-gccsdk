#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>


/* Define this to print lots of debug info */
#define LOTS_OF_DEBUG

#define BUF_LEN 70

int global = 0;
int temp;

extern int __pthread_num_running_threads;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int buffer[BUF_LEN];
int bufferhead, buffertail;

int waitcount = 0;
int writeractive = 0;

void *reader(void *arg)
{
	int i,j;
	int ret;

#ifdef LOTS_OF_DEBUG
	printf("New thread was passed arg address %x\n", (int) arg);
	printf("New thread stack at %x\n", (int) &i);
#endif

	for (j=0;j<2;j++)
	{
		int tailbefore;
		clock_t until;
		ret = pthread_mutex_lock(&mutex);
		if (ret) {
			printf("mutex_lock failed %d for %p\nFAIL\n",ret,arg);
			exit(1);
		}
#ifdef LOTS_OF_DEBUG
		printf("Got mutex for reading %p\n",arg);
#endif
		waitcount++;
		ret = pthread_cond_wait(&cond,&mutex);
		if (ret) {
			printf("cond_wait failed %d for %p\nFAIL\n",ret,arg);
			exit(1);
		}
#ifdef LOTS_OF_DEBUG
		printf("Returned from condition wait %p\n",arg);
#endif
		waitcount--;
		tailbefore = buffertail;
		if (buffer[buffertail] == 0) {
			printf("buffer[buffertail] is invalid %p, %x\nFAIL\n",arg, buffer[buffertail]);
			exit(1);
		}

		until = clock() + 5;
		while (clock() < until) {
			/* waste time */
			for (i=0;i<20000;i++) temp = i;
		}

		if (buffer[buffertail] == 0) {
			printf("buffer[buffertail] is invalid after timewaste %p\nFAIL\n",arg);
			exit(1);
		}
		if (tailbefore != buffertail) {
			printf("buffertail changed while we have mutex %p\nFAIL\n",arg);
			exit(1);
		}
		buffer[buffertail++] = 0;
		if (buffertail >= BUF_LEN) buffertail = 0;
#ifdef LOTS_OF_DEBUG
		printf("Releasing reading mutex %p\n",arg);
#endif
		ret = pthread_mutex_unlock(&mutex);
		if (ret) {
			printf("mutex_unlock failed %d for %p\nFAIL\n",ret,arg);
			exit(1);
		}

	}

#ifdef LOTS_OF_DEBUG
	printf("Thread arg %x coming to a lazy stop\n", (int) arg);
#endif

	return NULL;
}

void *writer(void *arg)
{
	int i;
	int num;
	int signalled = 0;
	int ret;

#ifdef LOTS_OF_DEBUG
	printf("New thread was passed arg address %x\n", (int) arg);
	printf("New thread stack at %x\n", (int) &i);
#endif

	writeractive = 1;
	while (signalled < 60) {
		int headbefore;
		clock_t until;

		until = clock() + 80;
		while (clock() < until) {
			/* waste time */
			for (i=0;i<20000;i++) temp = i;
		}

		ret = pthread_mutex_lock(&mutex);
		if (ret) {
			printf("mutex_lock failed %d for %p\nFAIL\n",ret,arg);
			exit(1);
		}
#ifdef LOTS_OF_DEBUG
		printf("Got mutex for writing %p\n",arg);
#endif
		num = (rand() % 3) + 1;
		if (num == 3) num = 30;
		if (num > waitcount) num = waitcount;
		for (i = 0; i < num; i++) {
			buffer[bufferhead] = rand() + 1;
			++bufferhead;
			if (bufferhead >= BUF_LEN) bufferhead = 0;
			if (bufferhead == buffertail) {
				printf("buffer full for %p\nFAIL\n",arg);
				exit(1);
			}
		}
		headbefore = bufferhead;

		until = clock() + 5;
		while (clock() < until) {
			/* waste time */
			for (i=0;i<20000;i++) temp = i;
		}

		if (bufferhead != headbefore) {
			printf("bufferhead changed while we have mutex lock %p\nFAIL\n",arg);
			exit(1);
		}
		if (num > 3) {
#ifdef LOTS_OF_DEBUG
			printf("Broadcasting condition var %p\n",arg);
#endif
			ret = pthread_cond_broadcast(&cond);
			if (ret) {
				printf("cond_broadcast failed %d for %p\nFAIL\n",ret,arg);
				exit(1);
			}
		} else {
			for (i=0; i<num; i++) {
#ifdef LOTS_OF_DEBUG
				printf("Signalling condition var %p\n",arg);
#endif
				ret = pthread_cond_signal(&cond);
				if (ret) {
					printf("cond_signal failed %d for %p\nFAIL\n",ret,arg);
					exit(1);
				}
			}
		}
		signalled += num;
#ifdef LOTS_OF_DEBUG
		printf("Releasing writer mutex %p\n",arg);
#endif
		ret = pthread_mutex_unlock(&mutex);
		if (ret) {
			printf("unlock failed %d for %p\nFAIL\n",ret,arg);
			exit(1);
		}
	}

#ifdef LOTS_OF_DEBUG
	printf("Thread arg %x coming to a lazy stop\n", (int) arg);
#endif

	writeractive = 0;

	return NULL;
}

int main(int argc,char **argv)
{
	pthread_t thread;
	int i;
	int thread_count;
	int now;
	char temp[256];
	pthread_cond_t test;

	if (argc == 2) {
		now = atoi(argv[1]);
	} else {
		now = (int)clock();
	}
	srand(now);
	sprintf(temp,"%d",now);
	setenv("pthread$seed",temp,1);
        printf("seed=%s\n",temp);

	pthread_cond_init(&test, NULL);
	if (memcmp(&test, &cond, sizeof(pthread_cond_t)) != 0) {
		printf("Error: static initialization does not match dynamic\nFAIL\n");
		exit(1);
	}

	for (i=0;i<BUF_LEN;i++) buffer[i] = 0;
	bufferhead = buffertail = 0;

	printf("Pthread cond test\nCreating lots of threads, this may take a while\n");
#ifdef LOTS_OF_DEBUG
	printf("Original thread stack at %x\n", (int) &i);
#endif
	for (i=0;i<30;i++) {
		if (pthread_create(&thread, NULL, reader, (void *)i)) {
			printf("Error: creating new thread\nFAIL\n");
			exit(1);
		}
	}

	if (pthread_create(&thread, NULL, writer, (void *)i)) {
		printf("Error: creating new thread\nFAIL\n");
		exit(1);
	}

	thread_count = 0;
	while(__pthread_num_running_threads > 1) {
		if (thread_count != __pthread_num_running_threads) {
			printf("Running threads = %d\n",__pthread_num_running_threads);
			thread_count = __pthread_num_running_threads;
		} else {
			printf("Main loop, writeractive = %d\n",writeractive);
			pthread_yield(); /* Yield for a short time to give the other threads more time */
		}
	}

	printf("PASS\n");

	exit(0);
}

