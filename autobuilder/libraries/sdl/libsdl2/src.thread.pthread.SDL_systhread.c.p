--- src/thread/pthread/SDL_systhread.c.orig	2016-10-20 04:56:26.000000000 +0100
+++ src/thread/pthread/SDL_systhread.c	2016-11-20 17:41:21.593470897 +0000
@@ -183,7 +183,7 @@
 int
 SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
 {
-#if __NACL__ 
+#if defined(__NACL__) || defined(__riscos__)
     /* FIXME: Setting thread priority does not seem to be supported in NACL */
     return 0;
 #elif __LINUX__
