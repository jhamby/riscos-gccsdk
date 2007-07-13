--- src/game.cpp.orig	2007-06-02 08:48:47.000000000 +0100
+++ src/game.cpp	2007-07-09 12:50:24.585000000 +0100
@@ -1574,6 +1574,7 @@
 		}
 
 		if(val == "--help" || val == "-h") {
+/*
 			std::cout << "usage: " << argv[0]
 			<< " [OPTIONS] [DATA-DIRECTORY]\n"
 			<< "  --bpp number                 sets BitsPerPixel value. Example: --bpp 32\n"
@@ -1616,6 +1617,7 @@
 			<< "  --turns=value                sets the number of turns. The default is \"50\".\n"
 			<< "  --exit-at-end                exit Wesnoth at end of scenario.\n"
 			;
+*/
 			return 0;
 		} else if(val == "--version" || val == "-v") {
 			std::cout << _("Battle for Wesnoth") << " " << game_config::version
@@ -1688,7 +1690,6 @@
 			return 0;
 		}
 	}
-
 	srand(time(NULL));
 
 	game_controller game(argc,argv);
