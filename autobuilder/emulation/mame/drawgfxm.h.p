--- src/emu/drawgfxm.h.orig	2012-04-09 17:09:10.000000000 +0100
+++ src/emu/drawgfxm.h	2012-04-09 17:10:35.000000000 +0100
@@ -56,7 +56,7 @@
 
 
 /* special priority type meaning "none" */
-typedef struct { char dummy[3]; } NO_PRIORITY;
+typedef struct { char dummy[3]; } __attribute__((packed)) NO_PRIORITY;
 
 extern bitmap_ind8 drawgfx_dummy_priority_bitmap;
 #define DECLARE_NO_PRIORITY bitmap_t &priority = drawgfx_dummy_priority_bitmap;
