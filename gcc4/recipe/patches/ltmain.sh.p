--- ltmain.sh.orig	2008-12-13 13:55:24.000000000 +0100
+++ ltmain.sh	2008-12-13 13:54:51.000000000 +0100
@@ -48,6 +48,12 @@ EOF
   exit 0
 fi
 
+# Following is a hack: apparently with some version combination(s) of Autoconf,
+# libtool and perhaps a few other relevant packages, $echo is empty at this
+# point which makes the 'progname' determination a fork bomb line.
+# Needed for (at least) Ubuntu 8.10 (Intrepid) with a (forced) Autoconf 2.59
+# and libtool 2.2.4.
+echo=${echo-$ECHO}
 # The name of this program.
 progname=`$echo "$0" | sed 's%^.*/%%'`
 modename="$progname"
