#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


/* Define this to print lots of debug info */
#define LOTS_OF_DEBUG


extern int __pthread_num_running_threads;
int threads_made=0;

void pre1(void)
{
	printf("Pre1 called\n");
}

void pre2(void)
{
	printf("Pre2 called\n");
}

void parent1(void)
{
	printf("Parent1 called\n");
}

void parent2(void)
{
	printf("Parent2 called\n");
}

void child1(void)
{
	printf("child1 called\n");
}

void child2(void)
{
	printf("child2 called\n");
}

int temp = 0;
int statuses[5]={0,0,0,0,0};

void *new_thread(void *arg)
{
	int i;
	int ret;
	clock_t until;
#ifdef LOTS_OF_DEBUG
	char *blah;
#endif
statuses[(int)arg] = 1;
#ifdef LOTS_OF_DEBUG
	printf("New thread was passed arg address %x\n", (int) arg);
	printf("New thread stack at %x\n", (int) &i);
#endif

	until = clock() + 5;
	while (clock() < until) {
		/* waste time */
		for (i=0;i<20000;i++) temp = i;
	}
statuses[(int)arg] = 2;

	if ((int)arg == 3) {
statuses[(int)arg] = 7;
		ret = fork();
statuses[(int)arg] = 8;
		printf("fork() returned %d\n",ret);
		if (ret<0) {
			printf("FAIL\n");
			exit(1);
		}
statuses[(int)arg] = 9;

		until = clock() + 5;
		while (clock() < until) {
			/* waste time */
			for (i=0;i<20000;i++) temp = i;
		}
statuses[(int)arg] = 10;

		if (ret == 0) {
			printf("Child exiting...\n");
			_exit(0);
		}
statuses[(int)arg] = 11;
	} else if ((int)arg == 4) {
statuses[(int)arg] = 12;
		system("cat");
statuses[(int)arg] = 13;
	}
statuses[(int)arg] = 3;


#ifdef LOTS_OF_DEBUG
	printf("Trying a simple calloc and free\n");
	blah = calloc(1,32);
	/* We could fail to calloc here but it's not fatal */
	if (blah)
	{
		free(blah);
	}
#endif
statuses[(int)arg] = 4;

	/* Randomly test the pthread_exit call or a lazy thread return */
	if (rand() & 1)
	{
		/* Force an exit now using this routine - This should not return */
statuses[(int)arg] = 5;
		pthread_exit(NULL);
		printf("pthread_exit() returned from a thread exit call!\nFAIL\n");
		exit(1);
	}

	/* Lazy return */
#ifdef LOTS_OF_DEBUG
	printf("Thread arg %x coming to a lazy stop\n", (int) arg);
#endif
statuses[(int)arg] = 6;

	return NULL;
}

int main(int argc,char **argv)
{
	int i;
	int ret;
	pthread_t thread;
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

	ret = pthread_atfork(pre1,parent1,child1);
	if (ret) {
		printf("pthread_atfork returned %d\nFAIL\n",ret);
		exit(1);
	}
	ret = pthread_atfork(pre2,parent2,child2);
	if (ret) {
		printf("pthread_atfork returned %d\nFAIL\n",ret);
		exit(1);
	}

	printf("Pthread test 1\nCreating lots of threads, this may take a while\n");
#ifdef LOTS_OF_DEBUG
	printf("Original thread stack at %x\n", (int) &i);
#endif
	for (i=0;i<5;i++) {
		if (pthread_create(&thread, NULL, new_thread, (void*) i)) {
			printf("Error: creating new thread\nFAIL\n");
			exit(1);
		}
	}
	thread_count = 0;
	while(__pthread_num_running_threads > 1)
	{
	int j;
printf("in loop, statuses =");
for (j=0;j<5;j++) printf(" %d",statuses[j]);
printf("\n");
		if (thread_count != __pthread_num_running_threads)
		{
			printf("Running threads = %d\n",__pthread_num_running_threads);
			thread_count = __pthread_num_running_threads;
		}
		else
		{
			pthread_yield(); /* Let's yield for a short time to give the other threads more time */
		}
	}

	printf("PASS\n");

	exit(0);
}


