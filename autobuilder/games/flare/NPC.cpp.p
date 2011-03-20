--- flare/src/NPC.cpp.orig	2011-03-20 12:05:02.000000000 +0000
+++ flare/src/NPC.cpp	2011-03-20 12:07:46.000000000 +0000
@@ -48,7 +48,7 @@
 	string val;
 	string starts_with;
 	
-	infile.open(("npcs/" + npc_id + ".txt").c_str(), ios::in);
+	infile.open(("/<Flare$Dir>/npcs/" + npc_id + ".txt").c_str(), ios::in);
 
 	if (infile.is_open()) {
 		while (!infile.eof()) {
