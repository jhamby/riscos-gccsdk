--- flare/src/TileSet.cpp.orig	2011-03-20 12:04:16.000000000 +0000
+++ flare/src/TileSet.cpp	2011-03-20 12:08:34.000000000 +0000
@@ -45,7 +45,7 @@
 	string starts_with;
 	unsigned short index;
 
-	infile.open(("tilesetdefs/" + filename).c_str(), ios::in);
+	infile.open(("/<Flare$Dir>/tilesetdefs/" + filename).c_str(), ios::in);
 
 	if (infile.is_open()) {
 		string img;
