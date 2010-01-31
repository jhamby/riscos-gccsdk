--- gcc/testsuite/ada/acats/run_all.sh.orig	2008-05-24 13:28:11.000000000 +0200
+++ gcc/testsuite/ada/acats/run_all.sh	2008-05-25 15:07:50.000000000 +0200
@@ -144,7 +144,9 @@
 fi
 ./macrosub > macrosub.out 2>&1
 
-gcc -c cd300051.c
+# JOTY: hack to force using the build compiler.
+#gcc -c cd300051.c
+PATH=/usr/bin:$PATH gcc -c cd300051.c
 host_gnatmake -q -gnatws widechr.adb
 if [ $? -ne 0 ]; then
    display "**** Failed to compile widechr"
