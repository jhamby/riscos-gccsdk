Index: libgcc/configure.ac
===================================================================
--- libgcc/configure.ac	(revision 177068)
+++ libgcc/configure.ac	(working copy)
@@ -37,6 +37,15 @@
 AC_SUBST(libgcc_topdir)
 AC_CONFIG_AUX_DIR($libgcc_topdir)
 
+case "${host}" in
+  arm-unknown-riscos)
+    # Don't build a shared library for SCL:
+    if [[ -n "`echo $CC | grep -- -mlibscl`" ]]; then
+      enable_shared=no
+    fi
+    ;;
+esac
+
 AC_ARG_ENABLE(shared,
 [  --disable-shared        don't provide a shared libgcc],
 [
