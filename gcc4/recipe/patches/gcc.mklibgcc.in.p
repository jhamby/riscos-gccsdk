--- gcc/mklibgcc.in.orig	2007-01-16 23:44:01.000000000 +0100
+++ gcc/mklibgcc.in	2007-01-16 23:45:06.000000000 +0100
@@ -73,7 +73,7 @@
 # Build lines.
 
 gcc_compile='$(GCC_FOR_TARGET) $(LIBGCC2_CFLAGS) $(INCLUDES)'
-gcc_s_compile="$gcc_compile -DSHARED"
+gcc_s_compile="$gcc_compile -DSHARED -fPIC -fcall-used-v5"
 make_compile='$(MAKE) GCC_FOR_TARGET="$(GCC_FOR_TARGET)" \
 	  AR_FOR_TARGET="$(AR_FOR_TARGET)" \
 	  AR_CREATE_FOR_TARGET="$(AR_CREATE_FOR_TARGET)" \
