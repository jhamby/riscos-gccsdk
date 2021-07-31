--- Source_Files/Misc/thread_priority_sdl_posix.cpp.orig	2008-10-24 02:52:50.000000000 +0100
+++ Source_Files/Misc/thread_priority_sdl_posix.cpp	2009-12-10 10:47:34.278998400 +0000
@@ -41,7 +41,12 @@
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
