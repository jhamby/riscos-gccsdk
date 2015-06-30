Index: libcpp/configure.ac
===================================================================
--- libcpp/configure.ac	(revision 221600)
+++ libcpp/configure.ac	(working copy)
@@ -150,6 +150,7 @@
 case $target in
 	alpha*-*-* | \
 	arm*-*-*eabi* | \
+	arm*-*-riscos | \
 	arm*-*-rtems[.0-9]* | \
 	arm*-*-symbianelf* | \
 	x86_64-*-* | \
