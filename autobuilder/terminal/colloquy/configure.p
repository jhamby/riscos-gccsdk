--- configure.old	2003-04-24 08:50:49.000000000 +0100
+++ configure	2003-04-24 07:56:58.000000000 +0100
@@ -36,6 +36,10 @@
     CC=`which qcc`
     CFLAGS=-O2
     LIBS="-lm -lsocket"
+    ;;
+  *:RISCOS)
+    CC=gcc
+    CFLAGS="-O2 -L/home/riscos/env/lib"
 esac
 
 if [ -z $CC ]; then
@@ -82,7 +86,7 @@
   for i in "/usr/local/include/lua40" "/usr/local/include/lua" "/usr/local/include" "/usr/include/lua40" "/usr/include/lua" "/usr/include"; do
     for l in "" "-L/usr/local/lib"; do
       for s in "40" ""; do
-        $CC -o conftest conftest.c -I$i $l -llua$s -llualib$s $LIBS > /dev/null 2>&1
+        $CC $CFLAGS -o conftest conftest.c -I$i $l -llua$s -llualib$s $LIBS > /dev/null 2>&1
         if [ $? = 0 ]; then
           rm conftest.c
           return
@@ -108,7 +112,7 @@
   LUAINC="-I$i"
   echo "found."
   echo -n "Checking for popen support... "
-  if [ "`./conftest;rm ./conftest`" != "popen present" ]; then
+  if [ "`./conftest;rm -f ./conftest`" != "popen present" ]; then
     echo "none!  You must run colloquy with noFork = 1"
   else
     echo "found."
