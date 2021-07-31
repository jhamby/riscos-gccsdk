Index: gcc/testsuite/ada/acats/run_all.sh
===================================================================
--- gcc/testsuite/ada/acats/run_all.sh	(revision 161055)
+++ gcc/testsuite/ada/acats/run_all.sh	(working copy)
@@ -154,7 +154,9 @@
 fi
 ./macrosub > macrosub.out 2>&1
 
-gcc -c cd300051.c
+# JOTY: hack to force using the build compiler.
+#gcc -c cd300051.c
+PATH=/usr/bin:$PATH gcc -c cd300051.c
 host_gnatmake -q -gnatws widechr.adb
 if [ $? -ne 0 ]; then
    display "**** Failed to compile widechr"
