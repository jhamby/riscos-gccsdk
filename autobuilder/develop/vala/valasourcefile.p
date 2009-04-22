--- vala/valasourcefile.vala.orig	2009-04-12 18:25:40.000000000 +0200
+++ vala/valasourcefile.vala	2009-04-21 00:26:13.000000000 +0200
@@ -64,7 +64,8 @@
 
 	private Gee.ArrayList<string> source_array = null;
 
-	private MappedFile mapped_file = null;
+//	private MappedFile mapped_file = null;
+	private string? file_content = null;
 
 	private string? _content = null;
 
@@ -277,6 +278,17 @@
 			return content;
 		}
 
+		if (file_content == null) {
+			try {
+				FileUtils.get_contents (filename, out file_content);
+			} catch (FileError e) {
+				Report.error (null, "Unable to read file `%s': %s".printf (filename, e.message));
+				return null;
+			}
+		}
+//stdout.printf(file_content);
+		return file_content;
+/*
 		if (mapped_file == null) {
 			try {
 				mapped_file = new MappedFile (filename, false);
@@ -287,6 +299,7 @@
 		}
 
 		return mapped_file.get_contents ();
+*/
 	}
 	
 	public size_t get_mapped_length () {
@@ -294,7 +307,8 @@
 			return content.length;
 		}
 
-		return mapped_file.get_length ();
+		return file_content.size();
+//		return mapped_file.get_length ();
 	}
 
 	public bool check (SemanticAnalyzer analyzer) {
