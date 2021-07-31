--- term.h.orig	2002-03-13 07:41:11.000000000 +0100
+++ term.h	2015-04-19 19:20:36.000000000 +0200
@@ -39,9 +39,9 @@
  * term.h: terminal support definitions
  *   Edit this file depending on the set of terminals you wish to support.
  * Comment out the terminal types that you don't want or don't have, and
- * uncomment those that you want included. Be aware that some terminal 
- * types (eg, SUN, UNIXPLOT) will require changes in the makefile 
- * LIBS definition. 
+ * uncomment those that you want included. Be aware that some terminal
+ * types (eg, SUN, UNIXPLOT) will require changes in the makefile
+ * LIBS definition.
  */
 
 /*
@@ -78,7 +78,7 @@
  * SVGA.BGI drivers. This also triggers the inclusion of Super VGA
  * support */
 /* #   define BGI_NAME "svga256" */
-#   include "pc.trm"		
+#   include "pc.trm"
 #  endif /* PC */
 # else /* WIN16 */
 #  include "win.trm"
@@ -91,7 +91,7 @@
 # ifdef HAVE_LIBGD
 #  include "gif.trm"
 # endif
-# include "hpgl.trm"
+/*# include "hpgl.trm"  doesn't work with gcc4.7.4 MRB 19.04.15 */
 # include "pbm.trm"
 /* NOTE THAT PDF REQUIRES A SEPARATE LIBRARY : see term/pdf.trm */
 # ifdef HAVE_LIBPDF
@@ -183,7 +183,7 @@
 /* Apple MacOs X Server (Openstep Unix) */
 #if defined(__APPLE__) && defined(__MACH__)
 # include "openstep.trm"
-#endif 
+#endif
 
 /* OS/2 */
 #ifdef OS2
@@ -352,7 +352,7 @@
 #include "hp500c.trm"
 
 /* HP7475, HP7220 plotters, and (hopefully) lots of others */
-#include "hpgl.trm"
+/*# include "hpgl.trm"  doesn't work with gcc4.7.4 MRB 19.04.15 */
 
 /* HP Laserjet II */
 #include "hpljii.trm"
@@ -456,5 +456,11 @@
 /* METAPOST */
 #include "metapost.trm"
 
+/* RISC OS draw */
+#include "rodraw.trm"
+
+/* RISC OS sprite */
+#include "rosprite.trm"
+
 #endif /* DOS16 || WIN16 special short termlist */
 #endif /* !SHORT_TERMLIST */
