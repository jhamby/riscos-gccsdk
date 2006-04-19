--- configure.in.orig	2006-04-19 19:29:16.114081528 +0200
+++ configure.in	2006-04-19 20:14:29.310000000 +0200
@@ -216,7 +216,7 @@
 
 #check the return type of sprintf
 case $SYSTEM_TYPE in
-  *netware*)
+  *netware*|*riscos*)
     AC_DEFINE(SPRINTF_RETURNS_INT) AC_MSG_RESULT("int")
     ;;
   *)
@@ -768,7 +768,7 @@
 
 # For compress in zlib
 case $SYSTEM_TYPE in
-  *netware* | *modesto*)
+  *netware* | *modesto* | *riscos*)
     AC_DEFINE(HAVE_COMPRESS)
     ;;
   *)
@@ -902,7 +902,7 @@
 
 AC_MSG_CHECKING([for int8])
 case $SYSTEM_TYPE in
-  *netware)
+  *netware | *riscos*)
     AC_MSG_RESULT([no])
     ;;
   *)
@@ -1280,7 +1280,7 @@
 if test "$with_named_thread" = "no"
 then
   AC_MSG_CHECKING("SCO threads")
-  if expr "$SYSTEM_TYPE" : ".*sco.*" > /dev/null
+  if false
   then
     if test -f /usr/lib/libgthreads.a -o -f /usr/lib/libgthreads.so
     then
@@ -1320,7 +1320,7 @@
     then
       AC_MSG_RESULT("no")
       AC_MSG_CHECKING("SCO UnixWare 7.1.x native threads")
-      if expr "$SYSTEM_TYPE" : ".*sco.*" > /dev/null
+      if false
       then
         if test -f /usr/lib/libthread.so -o -f /usr/lib/libthreadT.so
         then
@@ -1585,7 +1585,7 @@
 
 # System characteristics
 case $SYSTEM_TYPE in
-  *netware* | *modesto*) ;;
+  *netware* | *modesto* | *riscos* ) ;;
   *)
 AC_SYS_RESTARTABLE_SYSCALLS
     ;;
