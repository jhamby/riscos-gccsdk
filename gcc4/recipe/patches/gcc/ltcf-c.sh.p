--- ltcf-c.sh.orig	2007-07-08 16:22:31.000000000 +0100
+++ ltcf-c.sh	2007-07-08 16:19:30.000000000 +0100
@@ -185,6 +185,10 @@
     fi
     ;;
 
+  *riscos)
+    ld_shlibs=yes
+    ;;
+
   solaris* | sysv5*)
     if $LD -v 2>&1 | egrep 'BFD 2\.8' > /dev/null; then
       ld_shlibs=no
