--- SDL_rotozoom.c.orig	2012-04-24 00:01:00.000000000 +0100
+++ SDL_rotozoom.c	2012-04-24 00:01:51.000000000 +0100
@@ -51,9 +51,16 @@
 /*!
 \brief A 8bit Y/palette pixel.
 */
+
+#ifdef __riscos__
+/* A.Buckley 23/9/06 - redefined tColorY as definition of it as
+   a structure makes it 32 bits long*/
+   typedef Uint8 tColorY; 
+#else
 typedef struct tColorY {
 	Uint8 y;
 } tColorY;
+#endif
 
 /*! 
 \brief Returns maximum of two numbers a and b.
