--- magic/Makefile.in.old	2003-05-19 15:14:05.000000000 +0100
+++ magic/Makefile.in	2003-05-19 14:13:46.000000000 +0100
@@ -475,10 +475,10 @@
 	done >> $@
 
 magic.mgc: magic
-	$(top_builddir)/src/file -C -m $(srcdir)/magic
+	file -C -m $(srcdir)/magic
 
 magic.mime.mgc: magic.mime
-	$(top_builddir)/src/file -C -m $(srcdir)/magic.mime
+	file -C -m $(srcdir)/magic.mime
 # Tell versions [3.59,3.63) of GNU make to not export all variables.
 # Otherwise a system limit (for SysV at least) may be exceeded.
 .NOEXPORT:
