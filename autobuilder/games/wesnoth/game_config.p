--- src/game_config.cpp.orig	2007-06-02 08:48:47.000000000 +0100
+++ src/game_config.cpp	2007-07-09 14:49:25.861000000 +0100
@@ -85,6 +85,8 @@
 
 #ifdef __AMIGAOS4__
 	std::string path = "PROGDIR:";
+#elif __riscos__
+	std::string path = "Wesnoth:";
 #else
 #ifdef WESNOTH_PATH
 	std::string path = WESNOTH_PATH;
