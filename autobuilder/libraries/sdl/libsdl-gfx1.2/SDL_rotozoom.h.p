--- SDL_rotozoom.h.orig	2004-12-21 13:43:05.000000000 +0000
+++ SDL_rotozoom.h	2006-09-23 10:05:02.000000000 +0100
@@ -37,10 +37,15 @@
 	Uint8 a;
     } tColorRGBA;
 
+#ifdef __riscos__
+/* A.Buckley 23/9/06 - redefined tColorY as definition of it as
+   a structure makes it 32 bits long*/
+   typedef Uint8 tColorY; 
+#else
     typedef struct tColorY {
 	Uint8 y;
     } tColorY;
-
+#endif
 
 /* ---- Prototypes */
 
