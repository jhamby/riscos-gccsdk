--- gcc/mklibgcc.in.orig	2010-04-18 18:53:15.595875380 +0200
+++ gcc/mklibgcc.in	2010-04-18 18:52:50.543376465 +0200
@@ -73,7 +73,8 @@ fi
 # Build lines.
 
 gcc_compile='$(GCC_FOR_TARGET) $(LIBGCC2_CFLAGS) $(INCLUDES)'
-gcc_s_compile="$gcc_compile -DSHARED"
+# arm-unknown-riscos needs -fPIC when creating shared libraries:
+gcc_s_compile="$gcc_compile -DSHARED -fPIC"
 make_compile='$(MAKE) GCC_FOR_TARGET="$(GCC_FOR_TARGET)" \
 	  AR_FOR_TARGET="$(AR_FOR_TARGET)" \
 	  AR_CREATE_FOR_TARGET="$(AR_CREATE_FOR_TARGET)" \
@@ -173,7 +174,7 @@ for ml in $MULTILIBS; do
   if [ "$LIBUNWIND" ]; then
     libunwind_a=$dir/libunwind.a
   fi
-  if [ "$SHLIB_LINK" ]; then
+  if [ -n "$SHLIB_LINK" -a -z "`echo $flags | grep -- "-mlibscl"`" ]; then
     libgcc_eh_a=$dir/libgcc_eh.a
     libgcc_s_so=$dir/libgcc_s${SHLIB_EXT}
     if [ "$LIBUNWIND" ]; then
@@ -827,7 +828,7 @@ for ml in $MULTILIBS; do
   echo '	$(INSTALL_DATA)' ${dir}/libgcov.a ${ldir}/
   echo '	$(RANLIB_FOR_TARGET)' ${ldir}/libgcov.a
 
-  if [ "$SHLIB_LINK" ]; then
+  if [ -n "$SHLIB_LINK" -a -z "`echo $flags | grep -- "-mlibscl"`" ]; then
     echo '	$(INSTALL_DATA)' ${dir}/libgcc_eh.a ${ldir}/
     echo '	$(RANLIB_FOR_TARGET)' ${ldir}/libgcc_eh.a
 
