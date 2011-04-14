Index: gcc/testsuite/ada/acats/run_acats
===================================================================
--- gcc/testsuite/ada/acats/run_acats	(revision 161055)
+++ gcc/testsuite/ada/acats/run_acats	(working copy)
@@ -37,6 +37,12 @@
    exit 1
 fi
 
+# JOTY: hack to use the cross-compiler.
+for f in `ls $BASE/*-cross` ; do
+  f=`echo $f | sed s/-cross$//`
+  cp $f-cross $f
+done
+
 if [ ! -f $BASE/gnatchop ]; then
    echo gnattools missing, exiting.
    exit 1
@@ -48,7 +54,9 @@
 fi
 
 GCC_DRIVER="$BASE/xgcc"
-GCC="$BASE/xgcc -B$BASE/"
+# JOTY: hack to use the cross-compiler
+#GCC="$BASE/xgcc -B$BASE/"
+GCC="`echo $GCC_FOR_TARGET | sed s#^./xgcc#$GCC_DRIVER#` -isystem $testdir/../../../../libunixlib/include"
 export PATH ADA_INCLUDE_PATH ADA_OBJECTS_PATH GCC_DRIVER GCC LD_LIBRARY_PATH
 
 echo '#!/bin/sh' > host_gnatchop
