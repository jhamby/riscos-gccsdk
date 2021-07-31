--- ximage.C.orig	2008-08-07 13:04:10.370156200 +0100
+++ ximage.C	2008-08-07 13:05:03.236156200 +0100
@@ -11,7 +11,9 @@
 #include "ximage.H"
 #include "color_mapper.H"
 #include "gif_image.H"
+#ifndef __riscos__
 #include "ppm_image.H"
+#endif
 #include "jpeg_image.H"
 
 XjigXImage::XjigXImage(class Port *port,const char *filename,int autocrop)
@@ -21,10 +23,12 @@
 					  !strcasecmp(suffix, ".jpeg") ||
 					  !strcasecmp(suffix, ".jpe")))
 		img=new jpegImage(filename, autocrop);
+#ifndef __riscos__
 	else if(suffix && (!strcasecmp(suffix, ".ppm") ||
 							 !strcasecmp(suffix, ".pgm") ||
 							 !strcasecmp(suffix, ".pbm")))
 		img=new ppmImage(filename, autocrop);
+#endif
 	else
 		img=new GifImage(filename, autocrop);
 	xwidth  = Width();
