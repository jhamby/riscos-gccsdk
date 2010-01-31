--- gcc/ada/Makefile.in.orig	2008-04-27 20:34:28.000000000 +0200
+++ gcc/ada/Makefile.in	2008-04-28 03:07:07.000000000 +0200
@@ -386,6 +386,26 @@
 # $(strip STRING) removes leading and trailing spaces from STRING.
 # If what's left is null then it's a match.
 
+ 
+ifeq ($(strip $(filter-out arm% riscos%,$(arch) $(osys))),)
+  LIBGNAT_TARGET_PAIRS = \
+  a-intnam.ads<a-intnam-riscos.ads \
+  s-inmaop.adb<s-inmaop-posix.adb \
+  s-intman.adb<s-intman-posix.adb \
+  s-auxdec.ads<s-auxdec-empty.ads \
+  s-auxdec.adb<s-auxdec-empty.adb \
+  s-osinte.adb<s-osinte-posix.adb \
+  s-osinte.ads<s-osinte-riscos.ads \
+  s-osprim.adb<s-osprim-posix.adb \
+  s-taprop.adb<s-taprop-riscos.adb \
+  s-taspri.ads<s-taspri-posix.ads \
+  s-tpopsp.adb<s-tpopsp-posix.adb \
+  g-soccon.ads<g-soccon-riscos.ads \
+  system.ads<system-riscos-arm.ads
+
+  EH_MECHANISM=-gcc
+endif
+
 ifeq ($(strip $(filter-out m68k% wrs vx%,$(targ))),)
   LIBGNAT_TARGET_PAIRS = \
   a-intnam.ads<a-intnam-vxworks.ads \
