--- flare/src/PowerManager.cpp.orig	2011-03-20 12:04:37.000000000 +0000
+++ flare/src/PowerManager.cpp	2011-03-20 12:12:01.000000000 +0000
@@ -57,7 +57,7 @@
 	string starts_with;
 	int input_id = 0;
 	
-	infile.open("powers/powers.txt", ios::in);
+	infile.open("/<Flare$Dir>/powers/powers.txt", ios::in);
 
 	if (infile.is_open()) {
 		while (!infile.eof()) {
