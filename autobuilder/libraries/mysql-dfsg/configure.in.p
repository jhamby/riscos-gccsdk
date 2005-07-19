diff -ur mysql-dfsg-4.0.24/configure.in mysql-dfsg-4.0.24-ro/configure.in
--- configure.in	2005-03-05 00:38:15.000000000 +0000
+++ configure.in	2005-07-18 06:14:30.000000000 +0100
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
@@ -1588,7 +1588,7 @@
 
 # System characteristics
 case $SYSTEM_TYPE in
-  *netware* | *modesto*) ;;
+  *netware* | *modesto* | *riscos* ) ;;
   *)
 AC_SYS_RESTARTABLE_SYSCALLS
     ;;
