--- ./icedax/semshm.c.orig	2020-12-09 18:40:26.421415908 +0000
+++ ./icedax/semshm.c	2020-12-09 18:43:42.909530772 +0000
@@ -73,9 +73,11 @@
 
 #if defined(HAVE_SHMAT) && (HAVE_SHMAT == 1)
 #include <sys/types.h>
+#ifndef __riscos__
 #include <sys/ipc.h>
 #include <sys/shm.h>
 #endif
+#endif
 
 #ifdef  USE_MMAP
 #if defined(HAVE_SMMAP) && defined(USE_MMAP)
@@ -278,6 +280,7 @@
 /* request a shared memory block */
 static int shm_request_nommap(int size, unsigned char **memptr)
 {
+#ifndef __riscos__
   int   ret_val;
   int   shmflag;
   int   SHMEM_ID;
@@ -322,6 +325,7 @@
 
   fprintf(stderr, "Shared memory from %p to %p (%d bytes)\n", start_of_shm, end_of_shm, size);
 #endif
+#endif
   return 0;
 }
 
