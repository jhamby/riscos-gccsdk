--- src/graphics/x11/x11.c.orig	2005-06-29 21:55:37.000000000 +0100
+++ src/graphics/x11/x11.c	2005-06-29 21:55:51.000000000 +0100
@@ -552,10 +552,10 @@
 				((int)(pal[i * 3 + 2] & 0x3e) >> 1);
 			break;
 		case 24:
-			rgb_palette[i] =
-				((int) pal[i * 3] << 18) |
-				((int) pal[i * 3 + 1] << 10) |
-				((int)pal[i * 3 + 2]) << 2;
+			rgb_palette[i] = color[i].pixel;
+//				((int) pal[i * 3] << 18) |
+//				((int) pal[i * 3 + 1] << 10) |
+//				((int)pal[i * 3 + 2]) << 2;
 			break;
 		}
 	}
