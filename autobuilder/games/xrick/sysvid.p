--- src/sysvid.c.orig	2007-09-05 15:22:16.670607300 +0100
+++ src/sysvid.c	2007-09-05 15:28:14.606607300 +0100
@@ -277,8 +277,11 @@
     p0 = sysvid_fb;
     p0 += rects->x + rects->y * SYSVID_WIDTH;
     q0 = (U8 *)screen->pixels;
+#ifdef __riscos__
+    q0 += (rects->x + rects->y * screen->pitch) * zoom;
+#else
     q0 += (rects->x + rects->y * SYSVID_WIDTH * zoom) * zoom;
-
+#endif
     for (y = rects->y; y < rects->y + rects->height; y++) {
       for (yz = 0; yz < zoom; yz++) {
 	p = p0;
@@ -290,7 +293,11 @@
 	  }
 	  p++;
 	}
+#ifdef __riscos__
+        q0 += screen->pitch;
+#else
 	q0 += SYSVID_WIDTH * zoom;
+#endif
       }
       p0 += SYSVID_WIDTH;
     }
