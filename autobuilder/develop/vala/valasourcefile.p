--- vala/valasourcefile.vala.orig	2009-09-26 13:57:39.000000000 +0200
+++ vala/valasourcefile.vala	2009-09-28 19:39:54.000000000 +0200
@@ -62,8 +62,6 @@
 
 	private Gee.ArrayList<string> source_array = null;
 
-	private MappedFile mapped_file = null;
-
 	private string? _content = null;
 
 	/**
@@ -258,28 +256,26 @@
 	}
 
 	public char* get_mapped_contents () {
-		if (content != null) {
-			return (char*) content;
-		}
-
-		if (mapped_file == null) {
+		if (content == null) {
 			try {
-				mapped_file = new MappedFile (filename, false);
+				string cont;
+				FileUtils.get_contents (filename, out cont);
+				content = cont;
 			} catch (FileError e) {
-				Report.error (null, "Unable to map file `%s': %s".printf (filename, e.message));
+				Report.error (null, "Unable to read file `%s': %s".printf (filename, e.message));
 				return null;
 			}
 		}
 
-		return mapped_file.get_contents ();
+		return content;
 	}
 	
 	public size_t get_mapped_length () {
 		if (content != null) {
-			return content.length;
+			return content.size();
 		}
 
-		return mapped_file.get_length ();
+		return 0;
 	}
 
 	public bool check (SemanticAnalyzer analyzer) {
