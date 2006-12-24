--- libiberty/configure.ac.orig	2006-12-23 16:25:15.000000000 -0800
+++ libiberty/configure.ac	2006-12-20 20:24:18.000000000 -0800
@@ -4,6 +4,8 @@
 AC_INIT
 AC_CONFIG_SRCDIR([xmalloc.c])
 
+export GCCSDK_NOE1F=
+
 # This works around the fact that libtool configuration may change LD
 # for this particular configuration, but some shells, instead of
 # keeping the changes in LD private, export them just because LD is
