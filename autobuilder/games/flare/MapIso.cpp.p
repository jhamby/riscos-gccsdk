--- flare/src/MapIso.cpp.orig	2011-03-20 11:10:00.000000000 +0000
+++ flare/src/MapIso.cpp	2011-03-20 11:14:22.000000000 +0000
@@ -83,7 +83,7 @@
 	// only load from file if the requested soundfx isn't already loaded
 	if (filename != sfx_filename) {
 		if (sfx) Mix_FreeChunk(sfx);
-		sfx = Mix_LoadWAV(filename.c_str());
+		sfx = Mix_LoadWAV(("/<Flare$Dir>/" + filename).c_str());
 		sfx_filename = filename;
 	}
 	if (sfx) Mix_PlayChannel(-1, sfx, 0);	
@@ -107,7 +107,7 @@
   
     event_count = 0;
   
-	infile.open(("maps/" + filename).c_str(), ios::in);
+	infile.open(("/<Flare$Dir>/maps/" + filename).c_str(), ios::in);
 
 	if (infile.is_open()) {
 		while (!infile.eof()) {
