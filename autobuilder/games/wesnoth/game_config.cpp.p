--- src/game_config.cpp.orig	2010-02-12 20:26:25.000000000 +0000
+++ src/game_config.cpp	2011-04-21 13:29:25.683212400 +0100
@@ -131,6 +131,8 @@
 
 #ifdef __AMIGAOS4__
 	std::string path = "PROGDIR:";
+#elif __riscos__
+	std::string path = "/Wesnoth:";
 #else
 #ifdef WESNOTH_PATH
 	std::string path = WESNOTH_PATH;
