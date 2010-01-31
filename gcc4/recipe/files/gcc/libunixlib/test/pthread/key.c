#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>


/* Define this to print lots of debug info */
#define LOTS_OF_DEBUG


extern int __pthread_num_running_threads;
int threads_made=0;

pthread_key_t key1, key2;

struct data {
	void *arg;
};

void get_key(void *arg)
{
	struct data *data;

	data = pthread_getspecific(key1);
	if ((int)arg % 4 == 0) {
		if (data != NULL) {
			printf("Error: key data returned when there shouldn't be any\nFAIL\n");
			exit(1);
		}
	} else {
		if (data == NULL) {
			printf("Error: no key data returned when there should be some data\nFAIL\n");
			exit(1);
		}
		if (data->arg != arg) {
			printf("Error: key data returned does not match expected data\nFAIL\n");
			exit(1);
		}
	}

	data = pthread_getspecific(key2);
	if ((int)arg % 4 == 0) {
		if (data != NULL) {
			printf("Error: key data returned when there shouldn't be any\nFAIL\n");
			exit(1);
		}
	} else {
		if (data == NULL) {
			printf("Error: no key data returned when there should be some data\nFAIL\n");
			exit(1);
		}
		if (data->arg != (char*)arg + 1) {
			printf("Error: key data returned does not match expected data\nFAIL\n");
			exit(1);
		}
	}
}

pthread_once_t once_control = PTHREAD_ONCE_INIT;
int oncevar = 0;

void oncefunc(void)
{
#ifdef LOTS_OF_DEBUG
	printf("oncefunc called\n");
#endif
	oncevar++;
}

void *new_thread(void *arg)
{
	int i;
	int ret;
	int rnd = (rand() % 10) + 10;
	pthread_t thread;
	struct data *data;
#ifdef LOTS_OF_DEBUG
	char *blah;
#endif

#ifdef LOTS_OF_DEBUG
	printf("New thread was passed arg address %x\n", (int) arg);
	printf("New thread stack at %x\n", (int) &i);
#endif

	ret = pthread_once(&once_control, oncefunc);
	if (ret != 0) {
		printf("pthread_once failed: %d\nFAIL\n",ret);
		exit(1);
	}
	if ((int)arg % 4) {
		data = malloc(sizeof(struct data));
		if (data == NULL) {
			printf("Out of memory\nFAIL\n");
			exit(1);
		}
		data->arg = arg;
		ret = pthread_setspecific(key1,data);
		if (ret) {
			printf("pthread_setspecific returned %d\nFAIL\n",ret);
			exit(1);
		}
		data = malloc(sizeof(struct data));
		if (data == NULL) {
			printf("Out of memory\nFAIL\n");
			exit(1);
		}
		data->arg = (char*)arg + 1;
		ret = pthread_setspecific(key2,data);
		if (ret) {
			printf("pthread_setspecific returned %d\nFAIL\n",ret);
			exit(1);
		}
	}
	for (i=0;i<rnd;i++) {
		get_key(arg);

		if (! (rand() & 255)  && threads_made < 127)
		{
#ifdef LOTS_OF_DEBUG
			printf("Making new thread from inside a thread\n");
#endif
			threads_made++;
			/* We won't worry if this runs out of threads or memory just now, it's not that
			   important to the test. As long as the first thread was created. */
			pthread_create(&thread, NULL, new_thread, (void*)rand());
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
		printf("pthread_exit() returned from a thread exit call!\nFAIL\n");
		exit(1);
	}

#ifdef LOTS_OF_DEBUG
	printf("Thread arg %x coming to a lazy stop\n", (int) arg);
#endif

	return NULL;
}

void destructor(void *data)
{
#ifdef LOTS_OF_DEBUG
	printf("Destructor called for %p\n",data);
#endif
	free(data);
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

	ret = pthread_key_create(&key1,destructor);
	if (ret) {
		printf("pthread_key_create returned %d\nFAIL\n",ret);
		exit(1);
	}
	ret = pthread_key_create(&key2,destructor);
	if (ret) {
		printf("pthread_key_create returned %d\nFAIL\n",ret);
		exit(1);
	}

	printf("Pthread test 1\nCreating lots of threads, this may take a while\n");
#ifdef LOTS_OF_DEBUG
	printf("Original thread stack at %x\n", (int) &i);
#endif
	for (i=0;i<50;i++) {
		if (pthread_create(&thread, NULL, new_thread, (void*) i)) {
			printf("Error: creating new thread\nFAIL\n");
			exit(1);
		}
	}

	thread_count = 0;
	while(__pthread_num_running_threads > 1)
	{
		/*__os_print("In loop\r\n");*/
		if (thread_count != __pthread_num_running_threads)
		{
			printf("Running threads = %d\n",__pthread_num_running_threads);
			thread_count = __pthread_num_running_threads;
		}
		else
		{
			pthread_yield();
		}
	}

	ret = pthread_key_delete(key1);
	if (ret) {
		printf("pthread_key_delete returned %d\nFAIL\n",ret);
		exit(1);
	}
	ret = pthread_key_delete(key2);
	if (ret) {
		printf("pthread_key_delete returned %d\nFAIL\n",ret);
		exit(1);
	}

	if (oncevar != 1) {
		printf("oncevar is %d\nFAIL\n",oncevar);
		exit(1);
	}

	printf("PASS\n");

	exit(0);
}
