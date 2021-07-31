--- server/parser/button_character_def.h	4 Nov 2006 14:29:31 -0000	1.8
+++ server/parser/button_character_def.h	18 Nov 2006 17:34:09 -0000
@@ -14,6 +14,10 @@
 #include "character_def.h"
 #include "sound.h"
 
+#ifndef UNUSED
+#define UNUSED(x) ((x) = (x))
+#endif
+
 // Forward declarations
 namespace gnash {
 	class sprite_instance;
