--- vala/valacodecontext.vala.orig	2009-10-28 18:31:22.000000000 +0100
+++ vala/valacodecontext.vala	2009-11-05 00:12:17.000000000 +0100
@@ -302,6 +302,12 @@
 		var path = get_file_path (pkg + ".vapi", "vala/vapi", directories);
 
 		if (path == null) {
+			var filename = Path.build_filename ("ValaVapi:", pkg + ".vapi");
+			if (FileUtils.test (filename, FileTest.EXISTS)) {
+				path = filename;
+			}
+		}
+		if (path == null) {
 			/* last chance: try the package compiled-in vapi dir */
 			var filename = Path.build_filename (Config.PACKAGE_DATADIR, "vapi", pkg + ".vapi");
 			if (FileUtils.test (filename, FileTest.EXISTS)) {
