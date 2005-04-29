--- wartool.c.old	2004-06-27 20:28:46.000000000 +0100
+++ wartool.c	2005-01-07 23:12:19.150000000 +0000
@@ -84,11 +84,29 @@
 	return s;
 }
 #else
+#ifdef __riscos__
+#define FetchLE16(p) ((unsigned short)( *((unsigned char *)p) \
+                     + ((*((unsigned char *)p+1) << 8) ))); p += 2
+#define FetchLE32(p) ((unsigned int)( *((unsigned char *)p) \
+                   + (*((unsigned char *)p+1) << 8) \
+                   + (*((unsigned char *)p+2) << 16) \
+                   + (*((unsigned char *)p+3) << 24))); p += 4
+#else
 #define FetchLE16(p) (*((unsigned short*)(p))); p += 2
 #define FetchLE32(p) (*((unsigned int*)(p))); p += 4
 #endif
+#endif
+#ifdef __riscos__
+#define AccessLE16(p) ((unsigned short)( *((unsigned char *)p) \
+                     + ((*((unsigned char *)p+1) << 8) )))
+#define AccessLE32(p)  ((unsigned int)( *((unsigned char *)p) \
+                   + (*((unsigned char *)p+1) << 8) \
+                   + (*((unsigned char *)p+2) << 16) \
+                   + (*((unsigned char *)p+3) << 24)))
+#else
 #define AccessLE16(p) (*((unsigned short*)(p)))
 #define AccessLE32(p) (*((unsigned int*)(p)))
+#endif
 #define ConvertLE16(v) (v)
 #else
 static inline unsigned short Swap16(unsigned short D) {
@@ -1659,6 +1677,17 @@
 	png_infop info_ptr;
 	unsigned char** lines;
 	int i;
+#ifdef __riscos__
+       png_color ropalette[256];
+       int palindex;
+       for (palindex = 0; palindex < 256; palindex++)
+       {
+           ropalette[palindex].red = pal[palindex*3];
+           ropalette[palindex].green = pal[palindex*3+1];
+           ropalette[palindex].blue = pal[palindex*3+2];
+       }
+       pal = (unsigned char *)ropalette;
+#endif
 
 	if (!(fp = fopen(name, "wb"))) {
 		printf("%s:", name);
@@ -1796,6 +1825,7 @@
 	close(f);
 
 	cp = buf;
+
 	i = FetchLE32(cp);
 //	printf("Magic\t%08X\t", i);
 	if (i != 0x19) {
