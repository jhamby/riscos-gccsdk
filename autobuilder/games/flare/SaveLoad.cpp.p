--- flare/src/SaveLoad.cpp.orig	2011-03-25 18:43:27.000000000 +0000
+++ flare/src/SaveLoad.cpp	2011-03-25 18:43:51.000000000 +0000
@@ -22,7 +22,7 @@
 void GameEngine::saveGame() {
 	ofstream outfile;
 
-	outfile.open("saves/save1.txt", ios::out);
+	outfile.open("/<Flare$Dir>/saves/save1.txt", ios::out);
 
 	if (outfile.is_open()) {
 	
@@ -92,7 +92,7 @@
 	}
 
 	// TODO: change to hero name?
-	infile.open("saves/save1.txt", ios::in);
+	infile.open("/<Flare$Dir>/saves/save1.txt", ios::in);
 
 	if (infile.is_open()) {
 		while (!infile.eof()) {
