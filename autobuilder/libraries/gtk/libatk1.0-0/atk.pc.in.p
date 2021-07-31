--- atk.pc.in.orig	2020-07-25 22:14:59.242774677 +0100
+++ atk.pc.in	2020-07-26 01:09:00.752436484 +0100
@@ -7,5 +7,5 @@
 Description: Accessibility Toolkit
 Version: @VERSION@
 Requires: gobject-2.0
-Libs: -L${libdir} -latk-@ATK_API_VERSION@
+Libs: -L${libdir} -latk-@ATK_API_VERSION@ -lffi
 Cflags: -I${includedir}/atk-1.0
