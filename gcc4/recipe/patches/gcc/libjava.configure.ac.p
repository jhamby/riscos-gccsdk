Index: libjava/configure.ac
===================================================================
--- libjava/configure.ac	(revision 184613)
+++ libjava/configure.ac	(working copy)
@@ -1067,8 +1067,8 @@
 case "$THREADS" in
  posix)
     case "$host" in
-     *-*-cygwin*)
-	# Don't set THREADLIBS here.  Cygwin doesn't have -lpthread.
+     *-*-cygwin* | arm*-*-riscos)
+	# Don't set THREADLIBS here.  These environments do not have -lpthread.
 	;;
      *-*-freebsd[[34]].*)
 	# Before FreeBSD 5, it didn't have -lpthread (or any library which
