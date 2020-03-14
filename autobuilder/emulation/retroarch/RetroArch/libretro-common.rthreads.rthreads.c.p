--- libretro-common/rthreads/rthreads.c.orig	2019-06-01 09:26:10.341803450 +0100
+++ libretro-common/rthreads/rthreads.c	2019-06-01 09:27:09.257803510 +0100
@@ -216,7 +216,9 @@
       struct sched_param sp;
       memset(&sp, 0, sizeof(struct sched_param));
       sp.sched_priority = thread_priority;
+#ifndef __riscos__
       pthread_attr_setschedpolicy(&thread_attr, SCHED_RR);
+#endif
       pthread_attr_setschedparam(&thread_attr, &sp);
 
       thread_attr_needed = true;
