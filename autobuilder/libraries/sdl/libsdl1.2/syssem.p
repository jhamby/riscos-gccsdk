Index: src/thread/riscos/SDL_syssem.c
===================================================================
RCS file: /home/sdlweb/libsdl.org/cvs/SDL12/src/thread/riscos/SDL_syssem.c,v
retrieving revision 1.1
diff -u -r1.1 SDL_syssem.c
--- src/thread/riscos/SDL_syssem.c	17 Sep 2004 13:20:10 -0000	1.1
+++ src/thread/riscos/SDL_syssem.c	14 Jul 2005 11:32:42 -0000
@@ -20,14 +20,13 @@
     slouken@libsdl.org
 */
 
-/* RISC OS doesn't have semiphores so use code based on generic implementation */
+/* RISC OS semiphores based on linux code */
 
 #ifdef SAVE_RCSID
 static char rcsid =
  "@(#) $Id: SDL_syssem.c,v 1.1 2004/09/17 13:20:10 slouken Exp $";
 #endif
 
-/* An implementation of semaphores using mutexes and condition variables */
 
 #include <stdlib.h>
 
@@ -81,51 +80,40 @@
 
 #else
 
-struct SDL_semaphore
-{
-	Uint32 count;
-	Uint32 waiters_count;
-	SDL_mutex *count_lock;
-	SDL_cond *count_nonzero;
+
+#include <stdio.h>
+#include <stdlib.h>
+#include <unistd.h>			/* For getpid() */
+#include <pthread.h>
+#include <semaphore.h>
+
+struct SDL_semaphore {
+	sem_t *sem;
+	sem_t sem_data;
 };
 
+/* Create a semaphore, initialized with value */
 SDL_sem *SDL_CreateSemaphore(Uint32 initial_value)
 {
-	SDL_sem *sem;
-
-	sem = (SDL_sem *)malloc(sizeof(*sem));
-	if ( ! sem ) {
+	SDL_sem *sem = (SDL_sem *) malloc(sizeof(SDL_sem));
+	if ( sem ) {
+		if ( sem_init(&sem->sem_data, 0, initial_value) < 0 ) {
+			SDL_SetError("sem_init() failed");
+			free(sem);
+			sem = NULL;
+		} else {
+			sem->sem = &sem->sem_data;
+		}
+	} else {
 		SDL_OutOfMemory();
-		return(0);
-	}
-	sem->count = initial_value;
-	sem->waiters_count = 0;
-
-	sem->count_lock = SDL_CreateMutex();
-	sem->count_nonzero = SDL_CreateCond();
-	if ( ! sem->count_lock || ! sem->count_nonzero ) {
-		SDL_DestroySemaphore(sem);
-		return(0);
 	}
-
-	return(sem);
+	return sem;
 }
 
-/* WARNING:
-   You cannot call this function when another thread is using the semaphore.
-*/
 void SDL_DestroySemaphore(SDL_sem *sem)
 {
 	if ( sem ) {
-		sem->count = 0xFFFFFFFF;
-		while ( sem->waiters_count > 0) {
-			SDL_CondSignal(sem->count_nonzero);
-			SDL_Delay(10);
-		}
-		SDL_DestroyCond(sem->count_nonzero);
-		SDL_mutexP(sem->count_lock);
-		SDL_mutexV(sem->count_lock);
-		SDL_DestroyMutex(sem->count_lock);
+		sem_destroy(sem->sem);
 		free(sem);
 	}
 }
@@ -138,15 +126,26 @@
 		SDL_SetError("Passed a NULL semaphore");
 		return -1;
 	}
-
 	retval = SDL_MUTEX_TIMEDOUT;
-	SDL_LockMutex(sem->count_lock);
-	if ( sem->count > 0 ) {
-		--sem->count;
+	if ( sem_trywait(sem->sem) == 0 ) {
 		retval = 0;
 	}
-	SDL_UnlockMutex(sem->count_lock);
+	return retval;
+}
+
+int SDL_SemWait(SDL_sem *sem)
+{
+	int retval;
+
+	if ( ! sem ) {
+		SDL_SetError("Passed a NULL semaphore");
+		return -1;
+	}
 
+	retval = sem_wait(sem->sem);
+	if ( retval < 0 ) {
+		SDL_SetError("sem_wait() failed");
+	}
 	return retval;
 }
 
@@ -159,58 +158,53 @@
 		return -1;
 	}
 
-	/* A timeout of 0 is an easy case */
+	/* Try the easy cases first */
 	if ( timeout == 0 ) {
 		return SDL_SemTryWait(sem);
 	}
+	if ( timeout == SDL_MUTEX_MAXWAIT ) {
+		return SDL_SemWait(sem);
+	}
 
-	SDL_LockMutex(sem->count_lock);
-	++sem->waiters_count;
-	retval = 0;
-	while ( (sem->count == 0) && (retval != SDL_MUTEX_TIMEDOUT) ) {
-		retval = SDL_CondWaitTimeout(sem->count_nonzero,
-		                             sem->count_lock, timeout);
-	}
-	--sem->waiters_count;
-	--sem->count;
-	SDL_UnlockMutex(sem->count_lock);
+	/* Ack!  We have to busy wait... */
+	timeout += SDL_GetTicks();
+	do {
+		retval = SDL_SemTryWait(sem);
+		if ( retval == 0 ) {
+			break;
+		}
+		SDL_Delay(1);
+	} while ( SDL_GetTicks() < timeout );
 
 	return retval;
 }
 
-int SDL_SemWait(SDL_sem *sem)
-{
-	return SDL_SemWaitTimeout(sem, SDL_MUTEX_MAXWAIT);
-}
-
 Uint32 SDL_SemValue(SDL_sem *sem)
 {
-	Uint32 value;
-	
-	value = 0;
+	int ret = 0;
 	if ( sem ) {
-		SDL_LockMutex(sem->count_lock);
-		value = sem->count;
-		SDL_UnlockMutex(sem->count_lock);
+		sem_getvalue(sem->sem, &ret);
+		if ( ret < 0 ) {
+			ret = 0;
+		}
 	}
-	return value;
+	return (Uint32)ret;
 }
 
 int SDL_SemPost(SDL_sem *sem)
 {
+	int retval;
+
 	if ( ! sem ) {
 		SDL_SetError("Passed a NULL semaphore");
 		return -1;
 	}
 
-	SDL_LockMutex(sem->count_lock);
-	if ( sem->waiters_count > 0 ) {
-		SDL_CondSignal(sem->count_nonzero);
+	retval = sem_post(sem->sem);
+	if ( retval < 0 ) {
+		SDL_SetError("sem_post() failed");
 	}
-	++sem->count;
-	SDL_UnlockMutex(sem->count_lock);
-
-	return 0;
+	return retval;
 }
 
 #endif /* DISABLE_THREADS */
