--- flare/src/StatBlock.cpp.orig	2011-03-20 10:58:35.000000000 +0000
+++ flare/src/StatBlock.cpp	2011-03-20 10:58:43.000000000 +0000
@@ -109,9 +109,12 @@
 	string key;
 	string val;
 	string starts_with;
+	string ropath("/<Flare$Dir>/");
 	int num;
+
+	ropath.append(filename);
 	
-	infile.open(filename.c_str(), ios::in);
+	infile.open(ropath.c_str(), ios::in);
 
 	if (infile.is_open()) {
 		while (!infile.eof()) {
