--- configure.in.orig	2005-07-08 10:30:38.000000000 +0100
+++ configure.in	2005-07-08 10:30:44.000000000 +0100
@@ -36,7 +36,7 @@
 	;;
 
 
-	i*86*|powerpc)
+	i*86*|powerpc|arm)
 		AC_DEFINE(SIZEOF_SHORT, 2)
 		AC_DEFINE(SIZEOF_INT, 4)
 		AC_DEFINE(SIZEOF_LONG, 4)
