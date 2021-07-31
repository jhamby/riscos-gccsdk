--- net.h.orig	2012-04-14 04:50:56.000000000 +0100
+++ net.h	2012-04-14 04:50:40.000000000 +0100
@@ -238,7 +238,7 @@
 extern int hostCacheCount;
 extern hostcache_t hostcache[HOSTCACHESIZE];
 
-#if !defined(_WIN32 ) && !defined (__linux__) && !defined (__sun__)
+#if !defined(_WIN32 ) && !defined (__linux__) && !defined (__sun__) && !defined (__riscos__)
 #ifndef htonl
 extern unsigned long htonl (unsigned long hostlong);
 #endif
