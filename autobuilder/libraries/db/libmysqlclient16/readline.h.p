--- cmd-line-utils/libedit/readline/readline.h.orig	2012-11-22 17:03:31.520641443 +0000
+++ cmd-line-utils/libedit/readline/readline.h	2012-11-22 17:03:49.272584936 +0000
@@ -66,7 +66,7 @@

 #ifndef CTRL
 #include <sys/ioctl.h>
-#if !defined(__sun) && !defined(__hpux) && !defined(_AIX) && !defined(__QNXNTO__) && !defined(__USLC__)
+#if !defined(__sun) && !defined(__hpux) && !defined(_AIX) && !defined(__QNXNTO__) && !defined(__USLC__) && !defined(__riscos__)
 #include <sys/ttydefaults.h>
 #endif
 #ifndef CTRL
