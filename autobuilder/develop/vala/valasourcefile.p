--- vala/valasourcefile.vala.orig	2009-04-12 18:25:40.000000000 +0200
+++ vala/valasourcefile.vala	2009-04-22 20:33:08.000000000 +0200
@@ -64,8 +64,6 @@
 
 	private Gee.ArrayList<string> source_array = null;
 
-	private MappedFile mapped_file = null;
-
 	private string? _content = null;
 
 	/**
@@ -273,28 +271,25 @@
 	}
 
 	public char* get_mapped_contents () {
-		if (content != null) {
-			return content;
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
-
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
