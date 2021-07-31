--- src/surfaceDB.cc.orig	2004-08-19 17:24:20.000000000 +0100
+++ src/surfaceDB.cc	2017-05-17 13:29:50.780009000 +0100
@@ -42,6 +42,10 @@
 }
 
 SDL_Surface *SurfaceDB::loadSurface( string fn, bool alpha ) {
+#ifdef __riscos__
+if (fn[0] == '.') fn.replace(0,1,"/<AlienBlaster$Dir>");
+else if (fn[0] != '/') fn.insert(0, "/<AlienBlaster$Dir>/");
+#endif
 
   SDL_Surface *searchResult = getSurface( fn );
   if ( searchResult ) {
