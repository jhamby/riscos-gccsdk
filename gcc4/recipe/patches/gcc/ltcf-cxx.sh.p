--- ltcf-cxx.sh.orig	2007-07-08 15:05:43.000000000 +0100
+++ ltcf-cxx.sh	2007-07-08 15:01:55.000000000 +0100
@@ -578,6 +578,9 @@
     # FIXME: insert proper C++ library support
     ld_shlibs=no
     ;;
+  *riscos)
+    ld_shlibs=yes
+    ;;
   sco*)
     case $cc_basename in
       CC)
