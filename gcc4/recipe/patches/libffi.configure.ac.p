--- libffi/configure.ac.orig	2006-09-26 01:56:42.000000000 +0200
+++ libffi/configure.ac	2006-09-26 01:57:07.000000000 +0200
@@ -74,6 +74,7 @@
 rs6000-*-aix*) TARGET=POWERPC_AIX; TARGETDIR=powerpc;;
 arm*-*-linux-*) TARGET=ARM; TARGETDIR=arm;;
 arm*-*-netbsdelf* | arm*-*-knetbsd*-gnu) TARGET=ARM; TARGETDIR=arm;;
+arm*-*-riscos*) TARGET=ARM; TARGETDIR=arm;;
 arm*-*-rtems*) TARGET=ARM; TARGETDIR=arm;;
 cris-*-*) TARGET=LIBFFI_CRIS; TARGETDIR=cris;;
 s390-*-linux-*) TARGET=S390; TARGETDIR=s390;;
