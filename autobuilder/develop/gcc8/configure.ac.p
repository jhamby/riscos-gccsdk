--- configure.ac.orig	2018-02-13 13:01:33.000000000 +0000
+++ configure.ac	2018-05-11 20:48:14.572573822 +0100
@@ -148,6 +148,7 @@
 target_libraries="target-libgcc \
 		target-libbacktrace \
 		target-libgloss \
+		target-libunixlib \
 		target-newlib \
 		target-libgomp \
 		target-liboffloadmic \
@@ -1196,6 +1197,14 @@
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
@@ -3159,6 +3168,10 @@
    ;;
   esac
   ;;
+
+ *" libunixlib "*)
+  FLAGS_FOR_TARGET=$FLAGS_FOR_TARGET' -B$$r/$(TARGET_SUBDIR)/libunixlib/ -B$$r/$(TARGET_SUBDIR)/libunixlib/.libs/ -isystem $$s/libunixlib/include'
+  ;;
 esac
 
 case "$target" in
