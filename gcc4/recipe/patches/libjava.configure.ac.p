--- libjava/configure.ac.orig	2006-09-26 01:56:42.000000000 +0200
+++ libjava/configure.ac	2006-09-26 01:57:07.000000000 +0200
@@ -787,8 +787,8 @@
 case "$THREADS" in
  posix)
     case "$host" in
-     *-*-cygwin*)
-	# Don't set THREADLIBS here.  Cygwin doesn't have -lpthread.
+     *-*-cygwin* | arm*-riscos*)
+	# Don't set THREADLIBS here.  These environments do not have -lpthread.
 	;;
      *-*-freebsd[[1234]]*)
 	# Before FreeBSD 5, it didn't have -lpthread (or any library which
