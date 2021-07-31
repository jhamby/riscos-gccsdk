--- src/dungeon.c.orig	2010-06-27 21:32:13.000000000 +0100
+++ src/dungeon.c	2010-06-27 21:31:31.000000000 +0100
@@ -684,8 +684,8 @@
 	 * mix with the raw messages that might be already on the screen
 	 */
 	if (iflags.window_inited) clear_nhwindow(WIN_MAP);
-	if (!check_version(&vers_info, DUNGEON_FILE, TRUE))
-	    panic("Dungeon description not valid.");
+/*	if (!check_version(&vers_info, DUNGEON_FILE, TRUE))
+	    panic("Dungeon description not valid."); */
 
 	/*
 	 * Read in each dungeon and transfer the results to the internal
