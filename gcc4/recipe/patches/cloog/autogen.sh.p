--- ../../srcdir.orig/cloog-ppl-0.15.10/autogen.sh	2010-03-23 22:29:47.000000000 +0100
+++ autogen.sh	2011-01-26 03:57:00.202470485 +0100
@@ -1,5 +1,5 @@
 #!/bin/sh
 libtoolize -c --force
-aclocal
+aclocal -I m4
 automake -a -c --foreign
 autoconf
