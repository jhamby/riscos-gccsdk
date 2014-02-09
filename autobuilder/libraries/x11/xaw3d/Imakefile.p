--- lib/Xaw3d/Imakefile	2009-10-12 12:55:55.000000000 +1300
+++ lib/Xaw3d/Imakefile	2009-10-12 12:56:41.000000000 +1300
@@ -22,7 +22,7 @@
 #define IncSubSubdir Xaw3d

 XCOMM When building outside an X11 source tree:
-EXTRA_INCLUDES = -I.
+EXTRA_INCLUDES = -I. -Iexports/include

 #ifdef SharedXawReqs
 REQUIREDLIBS = SharedXawReqs
