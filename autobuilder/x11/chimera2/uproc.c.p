--- common/uproc.c.old	2003-01-31 13:39:40.000000000 +0000
+++ common/uproc.c	2003-01-31 13:38:52.000000000 +0000
@@ -45,7 +45,7 @@
   * It would probably be better to use the POSIX mechanism here,but I have not
   * checked into it.  This gets us off the ground with SYSV.  RSE@GMI
   */
-#if defined(WNOHANG) && !defined(SYSV) && !defined(SVR4) && !defined(__QNX__) && !defined(__EMX__)
+#if defined(WNOHANG) && !defined(SYSV) && !defined(SVR4) && !defined(__QNX__) && !defined(__EMX__) && !defined(__riscos__)
   union wait st;
 
   do 
