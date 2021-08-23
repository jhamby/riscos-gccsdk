--- src/corelib/time/qtimezoneprivate_tz.cpp.orig	2021-08-22 14:36:25.049457714 -0700
+++ src/corelib/time/qtimezoneprivate_tz.cpp	2021-08-22 14:37:29.714073439 -0700
@@ -1230,12 +1230,14 @@
         // If defined, at runtime it can only be greater than this, so this is a safe bet:
         return SYMLOOP_MAX;
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
 #  ifdef MAXSYMLINKS
