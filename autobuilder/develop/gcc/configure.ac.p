--- configure.ac.orig	2020-07-23 07:35:16.916379838 +0100
+++ configure.ac	2020-07-24 17:16:30.737915350 +0100
@@ -147,6 +147,7 @@
 target_libraries="target-libgcc \
 		target-libbacktrace \
 		target-libgloss \
+		target-libunixlib \
 		target-newlib \
 		target-libgomp \
 		target-liboffloadmic \
@@ -1299,6 +1300,14 @@
     ;;
 esac
 
+case "${target}" in
+  arm*-*-riscos | *riscos-gnueabihf)
+    ;;
+  *)
+    noconfigdirs="$noconfigdirs target-libunixlib"
+    ;;
+esac
+
 # If we aren't building newlib, then don't build libgloss, since libgloss
 # depends upon some newlib header files.
 case "${noconfigdirs}" in
@@ -3290,6 +3299,10 @@
    ;;
   esac
   ;;
+
+ *" libunixlib "*)
+  FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -B$$r/$(TARGET_SUBDIR)/libunixlib/ -B$$r/$(TARGET_SUBDIR)/libunixlib/.libs/ -isystem $$s/libunixlib/include'
+  ;;
 esac
 
 case "$target" in
