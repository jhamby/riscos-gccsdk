--- win/tty/termcap.c.orig	2009-08-27 10:57:11.000000000 -0700
+++ win/tty/termcap.c	2009-08-27 10:57:21.000000000 -0700
@@ -836,7 +836,7 @@
 #include <curses.h>
 
 #ifndef LINUX
-extern char *tparm();
+//extern char *tparm();
 #endif
 
 #  ifdef COLOR_BLACK	/* trust include file */
