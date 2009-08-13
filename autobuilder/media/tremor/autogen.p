Index: autogen.sh
===================================================================
--- autogen.sh	(revision 11827)
+++ autogen.sh	(working copy)
@@ -117,4 +117,4 @@
 autoconf || exit 1
 
 cd $olddir
-$srcdir/configure --enable-maintainer-mode "$@" && echo
+#$srcdir/configure --enable-maintainer-mode "$@" && echo
