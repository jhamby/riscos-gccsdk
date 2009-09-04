--- vala/valacodecontext.vala.orig	2009-05-09 19:22:22.000000000 +0200
+++ vala/valacodecontext.vala	2009-09-04 21:14:46.000000000 +0200
@@ -301,6 +301,11 @@
 			}
 		}
 
+		filename = Path.build_filename ("ValaVapi:", "", basename);
+		if (FileUtils.test (filename, FileTest.EXISTS)) {
+			return filename;
+		}
+
 		filename = Path.build_filename (Config.PACKAGE_DATADIR, "vapi", basename);
 		if (FileUtils.test (filename, FileTest.EXISTS)) {
 			return filename;
