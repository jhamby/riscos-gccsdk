--- src/glad.cpp.orig	2010-08-29 12:43:51.000000000 +0100
+++ src/glad.cpp	2010-08-29 12:43:14.000000000 +0100
@@ -61,7 +61,7 @@
 int main(int argc, char *argv[])
 {
 	char * filepath;
-	filepath = get_file_path("openglad.cfg");
+	filepath = get_file_path("/<OpenGlad$Dir>/openglad.cfg");
 	cfg.parse(filepath);
 	delete filepath;
 	cfg.commandline(argc, argv);
