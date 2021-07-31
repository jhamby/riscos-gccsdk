--- src/sfnt/pngshim.c.orig	2020-06-20 09:30:36.700460357 +0100
+++ src/sfnt/pngshim.c	2020-06-20 09:31:52.831085434 +0100
@@ -48,7 +48,7 @@
     return ( temp + ( temp >> 8 ) ) >> 8;
   }
 
-
+#pragma GCC optimize ("O0")
   /* Premultiplies data and converts RGBA bytes => BGRA. */
   static void
   premultiply_data( png_structp    png,
@@ -158,7 +158,7 @@
     }
   }
 
-
+#pragma GCC optimize ("O3")
   /* Converts RGBx bytes to BGRA. */
   static void
   convert_bytes_to_data( png_structp    png,
