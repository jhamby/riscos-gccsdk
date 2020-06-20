--- configure.ac.orig	2018-09-13 10:59:15.000000000 +0100
+++ configure.ac	2018-11-19 10:30:03.689945343 +0000
@@ -448,11 +448,7 @@
 CFLAGS="$CFLAGS_SAVED"
 
 AC_MSG_RESULT([ ...done.])
-dnl ----------------------------
-dnl check for big/little endian
-dnl ----------------------------
-
-AC_C_BIGENDIAN([BIGENDIAN=1], [BIGENDIAN=0])
+BIGENDIAN=0
 
 dnl --------------------------------------------------
 dnl check for sse2 intrinsics
@@ -2107,6 +2103,7 @@
 MISC_FDEVS='ccr cif inferno mgr4 mgr8 mgrgray2 mgrgray4 mgrgray8 mgrmono miff24 plan9bm bit bitrgb bitrgbtags bitcmyk devicen spotcmyk xcf plib plibg plibm plibc plibk gprf display'
 
 XPSDEV=$XPSWRITEDEVICE
+SPRITE_DEVS='spr spr1 spr8c spr8m sprm'
 
 while test -n "$drivers"; do
         if echo $drivers |grep "," >/dev/null; then
@@ -2136,7 +2133,7 @@
                 fi
                 ;;
         FILES)
-                F_DEVS0="$F_DEVS0 $BMP_DEVS $FAX_DEVS $JPEG_DEVS $TIFFDEVS $PCX_DEVS $PBM_DEVS $PS_DEVS $MISC_FDEVS $XPSDEV"
+                F_DEVS0="$F_DEVS0 $BMP_DEVS $FAX_DEVS $JPEG_DEVS $TIFFDEVS $PCX_DEVS $PBM_DEVS $PS_DEVS $MISC_FDEVS $XPSDEV $SPRITE_DEVS"
                 CUPS_DEVS0="$CUPSDEV"
                 JBIG2_DEVS="$JBIG2FILEDEVS"
                 PNG_DEVS0="$PNGDEVS"
@@ -2185,6 +2182,10 @@
                 # PBM file formats
                 F_DEVS0="$F_DEVS0 $PBM_DEVS"
                 ;;
+        SPRITE)
+                # Sprite file formats
+                F_DEVS0="$F_DEVS0 $SPRITE_DEVS"
+                ;;
         CUPS)
                 # CUPS file format
                 CUPS_DEVS0="$CUPSDEV"
@@ -2365,7 +2366,7 @@
   PDL_DYNAMIC_LDFLAGS="-shared -Wl,\$(LD_SET_DT_SONAME)\$(LDFLAGS_SO_PREFIX)\$(GPDL_SONAME_MAJOR)"
   if test $ac_cv_prog_gcc = yes; then
     # GCC high level flag
-    DYNAMIC_LIBS="-rdynamic"
+    DYNAMIC_LIBS=""
   else
     DYNAMIC_LIBS=""
   fi
@@ -2380,7 +2381,7 @@
       PDL_DYNAMIC_LDFLAGS="-shared -Wl,\$(LD_SET_DT_SONAME)\$(LDFLAGS_SO_PREFIX)\$(GPDL_SONAME_MAJOR)"
       if test $ac_cv_prog_gcc = yes; then
         # GCC high level flag
-        DYNAMIC_LIBS="-rdynamic"
+        DYNAMIC_LIBS=""
       else
         DYNAMIC_LIBS=""
       fi
@@ -2592,17 +2593,9 @@
         ;;
 esac
 
-dnl --------------------------------------------------
-dnl set big/little endian for LCMS
-dnl --------------------------------------------------
 LCMS_ENDIAN=
 LCMS2_ENDIAN=
 
-if test x"$BIGENDIAN" != x"0"; then
-  LCMS_ENDIAN="-DUSE_BIG_ENDIAN=$BIGENDIAN"
-  LCMS2_ENDIAN="-DCMS_USE_BIG_ENDIAN=$BIGENDIAN"
-fi
-
 AC_SUBST(LCMS_ENDIAN)
 AC_SUBST(LCMS2_ENDIAN)
 
