--- configure.old	2003-01-14 17:04:23.000000000 +0000
+++ configure	2003-01-14 17:04:46.000000000 +0000
@@ -13258,7 +13258,7 @@
   if test "x$no_esd" = x ; then
      echo "$ac_t""yes" 1>&6
      cat >> confdefs.h <<\EOF
-#define HAVE_ESD 1
+#undef HAVE_ESD 1
 EOF
      
   else
