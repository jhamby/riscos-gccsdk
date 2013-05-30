--- atk.pc.in.orig	2013-05-30 15:30:48.112873759 +0100
+++ atk.pc.in	2013-05-30 15:31:09.444919885 +0100
@@ -7,5 +7,5 @@
 Description: Accessibility Toolkit
 Version: @VERSION@
 Requires: @GLIB_PACKAGES@
-Libs: -L${libdir} -latk-@ATK_API_VERSION@
+Libs: -L${libdir} -latk-@ATK_API_VERSION@ -lffi
 Cflags: -I${includedir}/atk-1.0
