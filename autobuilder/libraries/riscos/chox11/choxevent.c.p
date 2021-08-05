--- choxevent.c.orig	2021-08-03 23:14:51.828492522 -0700
+++ choxevent.c	2021-08-03 23:15:11.724597572 -0700
@@ -29,10 +29,10 @@
 
 #include <pthread.h>
 
-#include "Desklib:Time.h"
-#include "Desklib:Kbd.h"
-#include "Desklib:Coord.h"
-#include "Desklib:KeyCodes.h"
+#include "DeskLib/Time.h"
+#include "DeskLib/Kbd.h"
+#include "DeskLib/Coord.h"
+#include "DeskLib/KeyCodes.h"
 
 #include "chox11.h"
 
