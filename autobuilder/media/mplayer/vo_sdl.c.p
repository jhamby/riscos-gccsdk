--- libvo/vo_sdl.c.orig	2012-02-19 17:53:29.000000000 +0000
+++ libvo/vo_sdl.c	2012-02-19 17:53:56.000000000 +0000
@@ -810,6 +810,7 @@
 	}
         else {
 		if((strcmp(priv->driver, "x11") == 0)
+		||(strcmp(priv->driver, "riscos") == 0)
 		||(strcmp(priv->driver, "windib") == 0)
 		||(strcmp(priv->driver, "directx") == 0)
 		||(strcmp(priv->driver, "Quartz") == 0)
