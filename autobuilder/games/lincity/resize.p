--- splash.c.orig	2004-12-24 21:34:09.000000000 +0000
+++ splash.c	2004-12-24 21:29:12.000000000 +0000
@@ -197,7 +197,8 @@
 
 	{
 	  XNextEvent (display.dpy, &xev);
-	  HandleEvent (&xev);
+	  if (xev.type != ConfigureNotify)
+	    HandleEvent (&xev);
 	}
 
       c = x_key_value;
