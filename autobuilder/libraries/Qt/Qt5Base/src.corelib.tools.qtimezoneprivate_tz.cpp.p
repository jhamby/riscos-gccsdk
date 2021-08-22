--- src/corelib/tools/qtimezoneprivate_tz.cpp.orig	2021-05-17 23:43:52.000000000 -0700
+++ src/corelib/tools/qtimezoneprivate_tz.cpp	2021-08-16 19:51:26.996796720 -0700
@@ -1056,12 +1056,14 @@
 #if defined(SYMLOOP_MAX)
     return SYMLOOP_MAX; // if defined, at runtime it can only be greater than this, so this is a safe bet
 #else
+#if !defined(Q_OS_RISCOS)
     errno = 0;
     long result = sysconf(_SC_SYMLOOP_MAX);
     if (result >= 0)
         return result;
     // result is -1, meaning either error or no limit
     Q_ASSERT(!errno); // ... but it can't be an error, POSIX mandates _SC_SYMLOOP_MAX
+#endif
 
     // therefore we can make up our own limit
 #  if defined(MAXSYMLINKS)
