--- !DLSources/Libraries/Icon/BarIcon.c.orig	2023-08-11 15:33:36.782978523 -0700
+++ !DLSources/Libraries/Icon/BarIcon.c	2023-08-11 16:02:28.672183840 -0700
@@ -38,10 +38,12 @@
   int              xeigfactor;
   int              yeigfactor;
   os_error         *swierr;
+  size_t           spritenamelen = strnlen(spritename, sprite_MAXNAME);
 
   icreate.window = pos;
   icreate.icondata.flags.value = 0x1700301a;       /* Click-able sprite icon */
-  strncpy(icreate.icondata.data.spritename, spritename, sprite_MAXNAME);
+  memcpy(icreate.icondata.data.spritename, spritename, spritenamelen);
+  memset(icreate.icondata.data.spritename + spritenamelen, 0, sprite_MAXNAME - spritenamelen);
 
   icreate.icondata.workarearect.min.x =
     icreate.icondata.workarearect.min.y = 0;
