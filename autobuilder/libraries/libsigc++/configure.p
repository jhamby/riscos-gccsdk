--- configure.org	2006-02-19 17:57:04.000000000 +0100
+++ configure	2006-02-19 17:58:32.000000000 +0100
@@ -6903,7 +6903,7 @@
 if test "$cross_compiling" = yes; then
   
 echo "$ac_t""cross" 1>&6
-  if test ! -f ac_thread.h; then
+  if test ! -f ac_thread.sh; then
     echo '
 ac_thread_cond_attr_size=
 ac_thread_cond_impl_size=
