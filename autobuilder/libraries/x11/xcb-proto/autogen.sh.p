--- autogen.sh.orig	2012-05-05 17:03:37.000000000 +0100
+++ autogen.sh	2012-05-05 17:04:50.000000000 +0100
@@ -6,7 +6,7 @@
 ORIGDIR=`pwd`
 cd $srcdir

-autoreconf -v --install || exit 1
+autoreconf -v --install || autoreconf2.64 -v --install || exit 1
 cd $ORIGDIR || exit $?

 $srcdir/configure --enable-maintainer-mode "$@"
