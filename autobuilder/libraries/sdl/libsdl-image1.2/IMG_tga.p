--- IMG_tga.c.orig	2006-10-20 13:38:49.734375000 +0100
+++ IMG_tga.c	2006-10-20 13:31:18.843750000 +0100
@@ -108,7 +108,9 @@
     }
     start = SDL_RWtell(src);
 
-    if(!SDL_RWread(src, &hdr, sizeof(hdr), 1)) {
+	/* Have to use size of header and not sizeof(hdr) as
+	   the structure will be padded on some machines */
+    if(!SDL_RWread(src, &hdr, 18, 1)) {
         error = "Error reading TGA data";
 	goto error;
     }
