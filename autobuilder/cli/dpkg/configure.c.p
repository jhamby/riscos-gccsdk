--- main/configure.c.old	2004-02-04 13:44:17.000000000 +0000
+++ main/configure.c	2004-02-04 13:44:20.000000000 +0000
@@ -509,7 +509,9 @@
 	} else {
 		fputs(_("Don't forget to foreground (`fg') this "
 					"process when you're done !\n"), stderr);
+#ifndef __riscos__
 		kill(-getpgid(0),SIGTSTP);
+#endif
 	}
 }
 
