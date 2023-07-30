--- src/builtins.def.in.orig	2020-07-07 23:58:29.000000000 -0700
+++ src/builtins.def.in	2023-07-30 14:00:13.815131371 -0700
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
+catcmd		-u cat
 cdcmd		-u cd chdir
 commandcmd	-u command
 dotcmd		-s .
@@ -72,10 +73,13 @@
 hashcmd		hash
 jobscmd		-u jobs
 localcmd	-as local
+lscmd		-u ls
+mvcmd		-u mv
 printfcmd	printf
 pwdcmd		-u pwd
 readcmd		-u read
 returncmd	-s return
+rmcmd		-u rm
 setcmd		-s set
 shiftcmd	-s shift
 timescmd	-s times
