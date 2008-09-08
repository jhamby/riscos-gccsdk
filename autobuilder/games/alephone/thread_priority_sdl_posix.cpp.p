--- Source_Files/Misc/thread_priority_sdl_posix.cpp.orig	2008-09-04 11:40:36.804000000 +0100
+++ Source_Files/Misc/thread_priority_sdl_posix.cpp	2008-09-04 11:26:54.011000000 +0100
@@ -28,7 +28,12 @@
       return false;
     
     theSchedulingParameters.sched_priority = 
+#ifdef __riscos__
+/* sched_get_priority_max not implemented so guess at 99 */
+      99;
+#else
       sched_get_priority_max(theSchedulingPolicy);
+#endif
     
     if(pthread_setschedparam(theTargetThread, theSchedulingPolicy, &theSchedulingParameters) != 0)
       return false;
