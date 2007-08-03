--- src/blackbox.c.orig	2001-09-17 14:11:48.000000000 +0100
+++ src/blackbox.c	2007-08-02 13:05:05.705717900 +0100
@@ -22,6 +22,11 @@
     karlb@gmx.net                                                               
 */                                                                              
 
+#ifdef __riscos__
+// Set the directory to find the data files for RISC OS
+#define DATADIR  "<BlackBox$Dir>/data/"
+#endif
+
 // This ports the game to SFont...
 #define Text( dest, d1, x, y, d2, text, d3) PutString( dest, x, y, text)
 
