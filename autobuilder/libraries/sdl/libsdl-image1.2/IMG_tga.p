--- IMG_tga.c.orig	2009-11-09 12:25:07.000000000 +1300
+++ IMG_tga.c	2009-11-09 12:27:32.000000000 +1300
@@ -108,7 +108,9 @@
     }
     start = SDL_RWtell(src);
 
-    if(!SDL_RWread(src, &hdr, sizeof(hdr), 1)) {
+    /* Have to use size of header and not sizeof(hdr) as
+       the structure will be padded on some machines */
+    if(!SDL_RWread(src, &hdr, 18, 1)) {
         error = "Error reading TGA data";
 	goto error;
     }
