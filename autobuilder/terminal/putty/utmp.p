--- unix/uxpty.c.orig	2005-06-02 12:15:28.000000000 +0100
+++ unix/uxpty.c	2005-06-02 12:15:42.000000000 +0100
@@ -12,7 +12,7 @@
 #include <fcntl.h>
 #include <termios.h>
 #include <grp.h>
-#include <utmp.h>
+//#include <utmp.h>
 #include <pwd.h>
 #include <time.h>
 #include <sys/types.h>
