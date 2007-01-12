--- gcc/mklibgcc.in.orig	2006-12-28 21:34:29.000000000 +0000
+++ gcc/mklibgcc.in	2006-12-28 21:34:00.000000000 +0000
@@ -104,7 +104,7 @@
 # Build lines.
 
 gcc_compile='$(GCC_FOR_TARGET) $(LIBGCC2_CFLAGS) $(INCLUDES)'
-gcc_s_compile="$gcc_compile -DSHARED"
+gcc_s_compile="$gcc_compile -DSHARED -fPIC -fcall-used-v5"
 make_compile='$(MAKE) GCC_FOR_TARGET="$(GCC_FOR_TARGET)" \
 	  AR_FOR_TARGET="$(AR_FOR_TARGET)" \
 	  AR_CREATE_FOR_TARGET="$(AR_CREATE_FOR_TARGET)" \
