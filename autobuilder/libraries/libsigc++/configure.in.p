--- configure.in.org	2006-02-19 17:54:47.000000000 +0100
+++ configure.in	2006-02-19 17:54:55.000000000 +0100
@@ -585,7 +585,7 @@
 ],
 [
 AC_MSG_RESULT(cross)
-  if test ! -f ac_thread.h; then
+  if test ! -f ac_thread.sh; then
     echo '
 ac_thread_cond_attr_size=
 ac_thread_cond_impl_size=
