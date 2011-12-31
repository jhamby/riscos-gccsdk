--- configure.in.orig   2007-11-15 06:06:32.000000000 -0800
+++ configure.in        2008-01-12 07:42:12.000000000 -0800
@@ -1413,7 +1413,7 @@
 if test "$with_named_thread" = "no"
 then
   AC_MSG_CHECKING("SCO threads")
-  if expr "$SYSTEM_TYPE" : ".*sco.*" > /dev/null
+  if false
   then
     if test -f /usr/lib/libgthreads.a -o -f /usr/lib/libgthreads.so
     then
@@ -1453,7 +1453,7 @@
     then
       AC_MSG_RESULT("no")
       AC_MSG_CHECKING("SCO UnixWare 7.1.x native threads")
-      if expr "$SYSTEM_TYPE" : ".*sco.*" > /dev/null
+      if false
       then
         if test -f /usr/lib/libthread.so -o -f /usr/lib/libthreadT.so
         then
@@ -1675,7 +1675,7 @@

 # System characteristics
 case $SYSTEM_TYPE in
-  *netware*) ;;
+  *netware*|*riscos*) ;;
   *)
 AC_SYS_RESTARTABLE_SYSCALLS
     ;;
