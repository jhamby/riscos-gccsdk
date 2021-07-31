--- src/config/dirs.c.orig	2009-08-13 08:12:16.000000000 -0700
+++ src/config/dirs.c	2009-08-13 08:12:42.000000000 -0700
@@ -114,7 +114,7 @@
     if (!*homedir)
     {
         const char *psz_localhome = getenv( "HOME" );
-#if defined(HAVE_GETPWUID_R)
+#if defined(HAVE_GETPWUID_R) && !defined(__riscos__)
         char buf[sysconf (_SC_GETPW_R_SIZE_MAX)];
         if (psz_localhome == NULL)
         {
