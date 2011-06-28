Index: configure.ac
===================================================================
--- configure.ac	(revision 175593)
+++ configure.ac	(working copy)
@@ -189,6 +189,7 @@
 # Note that libiberty is not a target library.
 target_libraries="target-libgcc \
 		target-libgloss \
+		target-libunixlib \
 		target-newlib \
 		target-libgomp \
 		target-libstdc++-v3 \
@@ -1059,6 +1060,14 @@
     ;;
 esac
 
+case "${target}" in
+  arm-riscos*-* | arm-*-riscos)
+    ;;
+  *)
+    noconfigdirs="$noconfigdirs target-libunixlib"
+    ;;
+esac
+
 # If we aren't building newlib, then don't build libgloss, since libgloss
 # depends upon some newlib header files.
 case "${noconfigdirs}" in
@@ -3109,6 +3118,10 @@
    ;;
   esac
   ;;
+
+ *" libunixlib "*)
+  FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -B$$r/$(TARGET_SUBDIR)/libunixlib/ -B$$r/$(TARGET_SUBDIR)/libunixlib/.libs/ -isystem $$s/libunixlib/include'
+  ;;
 esac
 
 case "$target" in
