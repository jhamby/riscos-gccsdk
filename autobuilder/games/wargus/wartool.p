--- wartool.c.orig	2007-04-08 23:14:53.000000000 -0700
+++ wartool.c	2009-08-22 08:55:41.000000000 -0700
@@ -1771,6 +1771,17 @@
 	png_infop info_ptr;
 	unsigned char** lines;
 	int i;
+#ifdef __riscos__
+        png_color ropalette[256];
+        int palindex;
+        for (palindex = 0; palindex < 256; palindex++)
+        {
+           ropalette[palindex].red = pal[palindex*3];
+           ropalette[palindex].green = pal[palindex*3+1];
+           ropalette[palindex].blue = pal[palindex*3+2];
+        }
+        pal = (unsigned char *)ropalette;
+#endif
 
 	if (!(fp = fopen(name, "wb"))) {
 		printf("%s:", name);
