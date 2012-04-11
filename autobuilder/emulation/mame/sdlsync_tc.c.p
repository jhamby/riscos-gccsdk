--- src/osd/sdl/sdlsync_tc.c.orig	2012-04-09 15:02:13.000000000 +0100
+++ src/osd/sdl/sdlsync_tc.c	2012-04-09 15:02:30.000000000 +0100
@@ -305,7 +305,7 @@
 
 	thread = (osd_thread *)calloc(1, sizeof(osd_thread));
 	pthread_attr_init(&attr);
-	pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
+//	pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
 	if ( pthread_create(&thread->thread, &attr, callback, cbparam) != 0 )
 	{
 		free(thread);
