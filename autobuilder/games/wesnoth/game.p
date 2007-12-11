--- src/game.cpp.orig	2007-12-04 13:37:18.383736300 +0000
+++ src/game.cpp	2007-12-05 14:06:14.727135600 +0000
@@ -1561,9 +1561,11 @@
 	}
 }
 
+static int play_game_loop(game_controller &game);
+
+
 int play_game(int argc, char** argv)
 {
-	const int start_ticks = SDL_GetTicks();
 
 	//parse arguments that shouldn't require a display device
 	int arg;
@@ -1757,13 +1759,22 @@
 	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
 #endif
 
-	int ntip = -1;
-	config tips_of_day;
 
 	loadscreen::global_loadscreen->set_progress(100, _("Loading title screen."));
 	delete loadscreen::global_loadscreen;
 	loadscreen::global_loadscreen = 0;
 
+      return play_game_loop(game);
+}
+
+// Had to split play_game into two to stop an out of memory error
+// when cross-compiling for RISC OS with gcc3.4.6r2
+int play_game_loop(game_controller &game)
+{
+	const int start_ticks = SDL_GetTicks();
+	int ntip = -1;
+	config tips_of_day;
+
 	for(;;) {
 		//make sure the game config is always set to how it should be at the title screen
 		game.reset_game_cfg();
