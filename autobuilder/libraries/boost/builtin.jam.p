--- tools/build/v2/tools/builtin.jam.orig	2009-10-28 07:47:51.000000000 +0000
+++ tools/build/v2/tools/builtin.jam	2011-02-09 14:53:51.931595700 +0000
@@ -39,7 +39,7 @@
 
 
 .os-names = aix bsd cygwin darwin freebsd hpux iphone linux netbsd
-    openbsd osf qnx qnxnto sgi solaris unix unixware windows 
+    openbsd osf qnx qnxnto riscos sgi solaris unix unixware windows 
     elf # Not actually an OS -- used for targeting bare metal where
         # object format is ELF.  This catches both -elf and -eabi gcc
         # targets and well as other compilers targeting ELF. It is not
