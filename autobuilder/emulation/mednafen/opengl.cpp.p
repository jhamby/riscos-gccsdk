--- src/drivers/opengl.cpp.orig	2012-03-27 14:33:50.000000000 +0100
+++ src/drivers/opengl.cpp	2012-03-27 14:34:17.000000000 +0100
@@ -15,6 +15,7 @@
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  */
 
+#ifndef __riscos__
 #include "main.h"
 
 #include <string.h>
@@ -785,4 +786,4 @@
   p_glClear(GL_COLOR_BUFFER_BIT);
  //}
 }
-
+#endif
