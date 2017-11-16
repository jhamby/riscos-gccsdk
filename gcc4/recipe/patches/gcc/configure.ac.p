Index: configure.ac
===================================================================
--- configure.ac	(revision 254830)
+++ configure.ac	(working copy)
@@ -152,6 +152,7 @@
 # Note that libiberty is not a target library.
 target_libraries="target-libgcc \
 		target-libgloss \
+		target-libunixlib \
 		target-newlib \
 		target-libgomp \
 		target-libitm \
@@ -1060,6 +1061,14 @@
     ;;
 esac
 
+case "${target}" in
+  arm*-*-riscos)
+    ;;
+  *)
+    noconfigdirs="$noconfigdirs target-libunixlib"
+    ;;
+esac
+
 # If we aren't building newlib, then don't build libgloss, since libgloss
 # depends upon some newlib header files.
 case "${noconfigdirs}" in
@@ -1570,7 +1579,7 @@
     LIBS="$saved_LIBS"
   fi
 
-  ppllibs="$ppllibs -lppl_c -lppl $pwllib -lgmpxx"
+  ppllibs="$ppllibs -lppl_c -lppl $pwllib -lgmpxx -lm"
 
   if test "$enable_ppl_version_check" != no; then
     saved_CFLAGS="$CFLAGS"
@@ -1577,7 +1586,7 @@
     CFLAGS="$CFLAGS $pplinc $gmpinc"
     AC_MSG_CHECKING([for version 0.11 (revision 0 or later) of PPL])
     AC_TRY_COMPILE([#include "ppl_c.h"],[
-    #if PPL_VERSION_MAJOR != 0 || PPL_VERSION_MINOR < 11
+    #if PPL_VERSION_MAJOR == 0 && PPL_VERSION_MINOR < 11
     choke me
     #endif
     ], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no]); ppllibs= ; pplinc= ; with_ppl=no ])
@@ -2895,6 +2904,10 @@
    ;;
   esac
   ;;
+
+ *" libunixlib "*)
+  FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -B$$r/$(TARGET_SUBDIR)/libunixlib/ -B$$r/$(TARGET_SUBDIR)/libunixlib/.libs/ -isystem $$s/libunixlib/include'
+  ;;
 esac
 
 case "$target" in
