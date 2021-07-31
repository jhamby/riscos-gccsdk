--- configure.in.orig	2012-03-02 14:04:07.000000000 +0000
+++ configure.in	2012-11-22 17:10:41.659275486 +0000
@@ -291,6 +291,9 @@
   *netware*)
     AC_DEFINE(SPRINTF_RETURNS_INT, [1]) AC_MSG_RESULT("int")
     ;;
+  *riscos*)
+    AC_DEFINE(SPRINTF_RETURNS_INT, [1]) AC_MSG_RESULT("int")
+    ;;
   *)
 AC_TRY_RUN([
   int main()
@@ -1061,6 +1064,9 @@
   *netware)
     AC_MSG_RESULT([no])
     ;;
+  *riscos)
+    AC_MSG_RESULT([no])
+    ;;
   *)
 AC_TRY_RUN([
 #ifdef HAVE_STDLIB_H
@@ -1489,7 +1495,7 @@
 if test "$with_named_thread" = "no"
 then
   AC_MSG_CHECKING("SCO threads")
-  if expr "$SYSTEM_TYPE" : ".*sco.*" > /dev/null
+  if false
   then
     if test -f /usr/lib/libgthreads.a -o -f /usr/lib/libgthreads.so
     then
@@ -1529,7 +1535,7 @@
     then
       AC_MSG_RESULT("no")
       AC_MSG_CHECKING("SCO UnixWare 7.1.x native threads")
-      if expr "$SYSTEM_TYPE" : ".*sco.*" > /dev/null
+      if false
       then
         if test -f /usr/lib/libthread.so -o -f /usr/lib/libthreadT.so
         then
@@ -1751,7 +1757,7 @@
 
 # System characteristics
 case $SYSTEM_TYPE in
-  *netware*) ;;
+  *netware*|*riscos*) ;;
   *)
 AC_SYS_RESTARTABLE_SYSCALLS
     ;;
@@ -2777,9 +2783,9 @@
 
 if test "$THREAD_SAFE_CLIENT" = "no"
 then
-  sql_client_dirs="strings regex mysys dbug extra libmysql client"
+  sql_client_dirs="strings regex mysys extra libmysql client"
 else
-  sql_client_dirs="strings regex mysys dbug extra libmysql libmysql_r client"
+  sql_client_dirs="strings regex mysys extra libmysql libmysql_r client"
   linked_client_targets="$linked_client_targets linked_libmysql_r_sources"
   AC_CONFIG_FILES(libmysql_r/Makefile)
   AC_DEFINE([THREAD_SAFE_CLIENT], [1], [Should be client be thread safe])
