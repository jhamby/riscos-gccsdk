--- flare/src/ItemDatabase.cpp.orig	2011-03-20 12:44:36.000000000 +0000
+++ flare/src/ItemDatabase.cpp	2011-03-20 12:45:00.000000000 +0000
@@ -51,7 +51,7 @@
 	int id = 0;
 	string s;
 	
-	infile.open("items/items.txt", ios::in);
+	infile.open("/<Flare$Dir>/items/items.txt", ios::in);
 
 	if (infile.is_open()) {
 		while (!infile.eof()) {
