Index: gcc/ada/gcc-interface/Makefile.in
===================================================================
--- gcc/ada/gcc-interface/Makefile.in	(revision 196237)
+++ gcc/ada/gcc-interface/Makefile.in	(working copy)
@@ -1088,6 +1088,26 @@
   LIBRARY_VERSION := $(LIB_VERSION)
 endif
 
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
+  # FIXME: GCCSDK: most probably needs updating !
+  EH_MECHANISM=-gcc
+endif
+
 ifeq ($(strip $(filter-out %86 linux%,$(arch) $(osys))),)
   LIBGNAT_TARGET_PAIRS = \
   a-intnam.ads<a-intnam-linux.ads \
