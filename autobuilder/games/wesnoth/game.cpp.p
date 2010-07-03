--- src/game.cpp.orig	2010-06-27 16:06:33.000000000 +0100
+++ src/game.cpp	2010-06-27 16:09:38.000000000 +0100
@@ -269,7 +269,7 @@
 #endif
 	)
 	{
-		game_config::path = get_cwd() + '/' + game_config::path;
+		game_config::path = "/Wesnoth:";
 		font_manager_.update_font_path();
 	}
 
