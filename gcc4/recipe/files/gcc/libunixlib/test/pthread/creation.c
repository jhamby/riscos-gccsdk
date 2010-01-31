#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>

/* Define this to print lots of debug info */
#define LOTS_OF_DEBUG

/* A global variable to fool the compiler into not optimising out the timewaste() routine */
#ifdef TEST_FLOAT
float sum = 0;
#else
int sum = 0;
#endif
/* A counter, should really have a mutex, but it is not important to the test */
int threads_made = 0;

extern int __pthread_num_running_threads;

void* new_thread(void* arg);

void timewaste(int blah)
{
#ifdef TEST_FLOAT
	float i;
	double test = blah;
#else
	int i;
#endif
	int dummy[6000];

	sum = 0;

	for (i=0;i<blah;i++)
	{
		dummy[24] = sum += rand();
#ifdef TEST_FLOAT
		test -= 1.0;
#endif
	}
#ifdef TEST_FLOAT
	if (test != 0) {
		printf("test double = %f\nFAIL\n",test);
		exit(1);
	}
#endif
}

int main(int argc,char **argv)
{
	pthread_t thread;
	int i;
	int thread_count;
	int now;
	char temp[256];

	if (argc == 2) {
		now = atoi(argv[1]);
	} else {
		now = (int)clock();
	}
	srand(now);
	sprintf(temp,"%d",now);
	setenv("pthread$seed",temp,1);
        printf("seed=%s\n",temp);

	i=0;
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
		/*if (gbl->__pthread_callback_semaphore != 1) __pthread_fatal_error("sema set in main");*/
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
	int rnd = (rand() & 2047) + 64;
	pthread_t thread;
	int *mem1 = alloca(4);
#ifdef LOTS_OF_DEBUG
	char *blah;
#endif

#ifdef LOTS_OF_DEBUG
	printf("New thread was passed arg address %x\n", (int) arg);
	printf("New thread stack at %x\n", (int) &i);
#endif

	*mem1 = 0xFF887654;

	for (i=0;i<rnd;i++)
	{
		int *mem = alloca(4);
		jmp_buf buf;
		int ret;
		*mem = i;

		if (*mem!=i) printf("1mem = %p, *mem = %d, i = %d\nFAIL\n",mem,*mem,i);
		ret = setjmp(buf);
		timewaste(i);
		if (*mem!=i) printf("2mem = %p, *mem = %d, i = %d\nFAIL\n",mem,*mem,i);
		if (ret == 0 && (rnd % 32 == 0)) longjmp(buf, 1);
		if (*mem!=i) printf("3mem = %p, *mem = %d, i = %d\nFAIL\n",mem,*mem,i);

		if (rnd % 4 == 0) pthread_exit(0); /* Test an exit with alloca blocks still active */

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
		if (*mem != i) {
			printf("alloca memory overwritten mem = %p, *mem = %d, i = %d\nFAIL\n",mem,*mem,i);
			exit(1);
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

	if (*mem1!=0xFF887654) printf("mem1 = %p, *mem1 = %d\nFAIL\n",mem1,*mem1);

	/* Randomly test the pthread_exit call or a lazy thread return */
	if (rand() & 1)
	{
		/* Force an exit now using this routine - This should not return */
		pthread_exit(NULL);
		printf("pthread_exit() returned from a thread exit call!FAIL\n");
		exit(0);
	}

	/* Lazy return */
#ifdef LOTS_OF_DEBUG
	printf("Thread arg %x coming to a lazy stop\n", (int) arg);
#endif

	return(NULL);
}

