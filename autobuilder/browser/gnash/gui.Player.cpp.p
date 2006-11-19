--- gui/Player.cpp	11 Nov 2006 15:25:27 -0000	1.28
+++ gui/Player.cpp	18 Nov 2006 17:34:00 -0000
@@ -29,6 +29,9 @@
 # elif defined(GUI_SDL)
 #  include "sdlsup.h"
 #  define GUI_CLASS SDLGui
+# elif defined(GUI_RISCOS)
+#  include "riscossup.h"
+#  define GUI_CLASS RiscosGui
 # endif
 #else
 # ifdef HAVE_KDE
