--- flare/src/SaveLoad.cpp.orig	2011-03-20 12:05:16.000000000 +0000
+++ flare/src/SaveLoad.cpp	2011-03-20 12:08:08.000000000 +0000
@@ -92,7 +92,7 @@
 	}
 
 	// TODO: change to hero name?
-	infile.open("saves/save1.txt", ios::in);
+	infile.open("/<Flare$Dir>/saves/save1.txt", ios::in);
 
 	if (infile.is_open()) {
 		while (!infile.eof()) {
