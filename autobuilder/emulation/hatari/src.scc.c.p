--- src/scc.c.orig	2020-09-07 08:26:54.107438954 +0100
+++ src/scc.c	2020-09-07 08:28:54.677090108 +0100
@@ -292,7 +292,9 @@
 
 	switch (value)
 	{
+#ifndef __riscos__
 	 case 230400:	new_speed = B230400;	break;
+#endif
 	 case 115200:	new_speed = B115200;	break;
 	 case 57600:	new_speed = B57600;	break;
 	 case 38400:	new_speed = B38400;	break;
