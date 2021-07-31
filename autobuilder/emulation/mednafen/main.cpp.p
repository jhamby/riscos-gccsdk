--- src/drivers/main.cpp.orig	2015-02-24 23:28:55.000000000 +0000
+++ src/drivers/main.cpp	2015-05-26 10:01:31.834366807 +0100
@@ -15,6 +15,8 @@
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  */
 
+#include <unixlib/local.h>
+
 #include "main.h"
 
 #ifdef WIN32
@@ -47,8 +47,10 @@
 #include "input.h"
 #include "Joystick.h"
 #include "video.h"
+#ifndef __riscos__
 #include "opengl.h"
+#endif
 #include "shader.h"
 #include "sound.h"
 #include "netplay.h"
 #include "cheat.h"
@@ -733,7 +735,7 @@
 	}
 	else
 	{
-         if(!(tmp=MDFNI_LoadGame(force_module, path)))
+         if(!(tmp=MDFNI_LoadGame(force_module, __unixify(path, 0, NULL, 0, 0))))
 	  return 0;
 	}
 
