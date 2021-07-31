--- base/w_wad.c.orig	2012-04-26 10:15:34.000000000 +0100
+++ base/w_wad.c	2012-04-26 10:15:48.000000000 +0100
@@ -740,7 +740,7 @@
 		ST_Message ("\nIt appears that you are using a 4-player-only Version 1.0 hexen.wad.\n");
 		ST_Message ("Running HHexen without a Version 1.1 wadfile can cause many problems.\n");
 		ST_Message ("\nPress <ENTER> to continue.\n");
-		getchar();
+//		getchar();
 	}
 #endif
 }
