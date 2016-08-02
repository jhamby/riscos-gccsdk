--- src/thread/pthread/SDL_systhread.c.orig	2016-01-02 19:56:31.000000000 +0000
+++ src/thread/pthread/SDL_systhread.c	2016-06-22 11:16:58.790481403 +0100
@@ -188,7 +188,7 @@
 int
 SDL_SYS_SetThreadPriority(SDL_ThreadPriority priority)
 {
-#if __NACL__ 
+#if defined(__NACL__) || defined(__riscos__)
     /* FIXME: Setting thread priority does not seem to be supported in NACL */
     return 0;
 #elif __LINUX__
