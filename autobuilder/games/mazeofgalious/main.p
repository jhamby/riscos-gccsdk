--- main.cpp.orig	2009-12-02 20:03:01.000000000 -0800
+++ main.cpp	2009-12-02 20:03:37.000000000 -0800
@@ -52,9 +52,9 @@
 char **s_paths=0;
 int n_s_paths=0,act_s_path=0;
 char *s_path=0;
-
-char *confdir;
-char *datadir="/usr/share/games/mazeofgalious/";
+
+char *confdir;
+char *datadir="/<MazeOfGalious$Dir>/data/";
 
 extern int music_volume,sfx_volume;
 extern int fighting_demon;
 
 
