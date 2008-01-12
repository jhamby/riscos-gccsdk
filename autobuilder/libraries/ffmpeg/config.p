--- configure.orig	2008-01-12 09:59:36.000000000 -0800
+++ configure	2008-01-12 10:01:34.000000000 -0800
@@ -1439,7 +1439,7 @@
             add_ldflags "-mfdpic"
         ;;
         arm*)
-            add_cflags "-mcpu=$cpu"
+            add_cflags "-march=$cpu"
         ;;
         *)
             echo "WARNING: Unknown CPU \"$cpu\", ignored."
