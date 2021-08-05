--- chox11.h.orig	2021-08-03 23:11:47.519575463 -0700
+++ chox11.h	2021-08-03 23:14:26.624360970 -0700
@@ -32,17 +32,17 @@
 #  define puts(x) do { } while(0)
 #endif
 
-#include "Desklib:Window.h"
-#include "Desklib:Wimp.h"
-#include "Desklib:SWI.h"
-#include "Desklib:Sprite.h"
-#include "Desklib:Event.h"
-#include "Desklib:Error.h"
-#include "Desklib:Handler.h"
-#include "Desklib:GFX.h"
-#include "Desklib:ColourTran.h"
-#include "Desklib:Screen.h"
-#include "Desklib:Font.h"
+#include "DeskLib/Window.h"
+#include "DeskLib/Wimp.h"
+#include "DeskLib/SWI.h"
+#include "DeskLib/Sprite.h"
+#include "DeskLib/Event.h"
+#include "DeskLib/Error.h"
+#include "DeskLib/Handler.h"
+#include "DeskLib/GFX.h"
+#include "DeskLib/ColourTran.h"
+#include "DeskLib/Screen.h"
+#include "DeskLib/Font.h"
 
 #define X_APP_NAME          "X Windows Program"
 #define X_WIN_TITLE         "X Window"
