--- ./src/pingus/screens/pingus_menu.cpp.orig	2011-09-08 14:01:33.696032200 +0100
+++ ./src/pingus/screens/pingus_menu.cpp	2011-09-08 14:29:08.236974000 +0100
@@ -29,6 +29,10 @@
 #include "pingus/screens/start_screen.hpp"
 #include "pingus/worldmap/worldmap_screen.hpp"
 
+#ifdef __riscos__
+#include "config.h"
+#endif
+
 PingusMenu::PingusMenu() :
   is_init(),
   hint(),
