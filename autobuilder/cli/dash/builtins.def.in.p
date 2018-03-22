--- src/builtins.def.in.orig	2014-09-28 21:19:32.000000000 +1300
+++ src/builtins.def.in	2018-03-16 18:27:38.432838249 +1300
@@ -46,7 +46,7 @@
  */
 
 #ifndef JOBS
-#define JOBS 1
+#define JOBS 0
 #endif
 
 #if JOBS
@@ -59,6 +59,7 @@
 #endif
 
 breakcmd	-s break -s continue
+catcmd      cat
 cdcmd		-u cd chdir
 commandcmd	-u command
 dotcmd		-s .
@@ -72,10 +73,12 @@
 hashcmd		hash
 jobscmd		-u jobs
 localcmd	-as local
+mvcmd       mv
 printfcmd	printf
 pwdcmd		pwd
 readcmd		-u read
 returncmd	-s return
+rmcmd       rm
 setcmd		-s set
 shiftcmd	-s shift
 timescmd	-s times
