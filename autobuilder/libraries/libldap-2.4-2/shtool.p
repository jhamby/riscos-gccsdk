--- build/shtool.orig	2008-01-11 19:53:11.000000000 -0800
+++ build/shtool	2008-01-11 19:54:11.000000000 -0800
@@ -981,7 +981,7 @@
             if [ ".$opt_t" = .yes ]; then
                 echo "strip $dsttmp" 1>&2
             fi
-            strip $dsttmp || shtool_exit $?
+            #strip $dsttmp || shtool_exit $?
         fi
         if [ ".$opt_o" != . ]; then
             if [ ".$opt_t" = .yes ]; then
