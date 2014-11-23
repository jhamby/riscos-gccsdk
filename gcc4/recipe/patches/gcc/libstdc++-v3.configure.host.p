Index: libstdc++-v3/configure.host
===================================================================
--- libstdc++-v3/configure.host	(revision 217970)
+++ libstdc++-v3/configure.host	(working copy)
@@ -290,6 +290,9 @@
     os_include_dir="os/qnx/qnx6.1"
     c_model=c
     ;;
+  riscos)
+    os_include_dir="os/riscos"
+    ;;
   solaris2)
     # This too-vague configuration does not provide enough information
     # to select a ctype include, and thus os_include_dir is a crap shoot.
